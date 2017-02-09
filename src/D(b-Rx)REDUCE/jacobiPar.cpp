#include <chrono>
#include <cmath>
#include <condition_variable> // std::condition_variable
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

float err;
vector<float> sumR(270);

chrono::time_point<chrono::system_clock> startconv, endconv;

void printMAT(vector<vector<float>> A, int N) {
  printf("PRINTING A NEW MAT\n");
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++)
      printf("%f \t", A[i][j]);
    printf("\n");
  }
}

void printVEC(vector<float> x, int N) {
  printf("PRINTING A NEW VEC\n");
  for (int i = 0; i < N; i++)
    printf("%f \t", x[i]);
  printf("\n");
}

float error(vector<vector<float>> &R, vector<float> &D, vector<float> &x,
            vector<float> &b, int N) {
  float err = 0;
  float sum = 0;
  for (int i = 0; i < N; i++) {
    sum = 0;
    for (int j = 0; j < N; j++)
      if (i != j)
        sum += R[i][j] * x[j];
      else
        sum += D[i] * x[j];
    err += abs(b[i] - sum);
  }
  return err / N;
}

float errorVEC(vector<float> &x1, vector<float> &x2, int N) {
  float sum = 0;
  for (int i = 0; i < N; i++) {
    sum += (x1[i] - x2[i]) * (x1[i] - x2[i]);
  }
  return sqrt(sum);
}

chrono::duration<double> eTime(chrono::time_point<chrono::system_clock> start,
                               chrono::time_point<chrono::system_clock> end) {
  return end - start;
}

class barrier {
private:
  const int N_THREADS;
  int counts[2];
  int current;
  mutex lock;
  condition_variable condition;

public:
  barrier(int n);
  bool await(function<void()> cb);
};

barrier::barrier(int n)
    : N_THREADS(n), counts{0, 0}, current(0), lock(), condition() {}

bool barrier::await(function<void()> cb) {
  unique_lock<mutex> _lock(lock);
  int my = current;
  counts[my]++;

  if (counts[my] < N_THREADS) {
    condition.wait(_lock, [&] { return counts[my] == N_THREADS; });
    return false;
  } else {
    current ^= 1;
    counts[current] = 0;
    cb();
    condition.notify_all();
    return true;
  }
}

void iter(const vector<vector<float>> &R, const vector<float> &D,
          const vector<float> &b, vector<float> &x1, vector<float> &x2,
          const int from, const int to, barrier &bar, const int maxiter,
          const float epsilon, const size_t N, const size_t id,
          const size_t thread_num) {
  float sum;
  for (size_t k = 0; k <= maxiter and err >= epsilon; k++) {
    for (size_t i = from; i <= to; i++) {
      sum = 0.0;
      for (size_t j = 0; j < N; j++) {
        sum = sum + R[i][j] * x1[j];
      }
      x2[i] = (b[i] - sum) / D[i];
    }
    bar.await([&] {});
    startconv = chrono::system_clock::now();
    sum = 0.0;
    for (size_t i = from; i <= to; i++) {
      sum += (x1[i] - x2[i]) * (x1[i] - x2[i]);
    }
    sumR[id] = sum;
    bar.await([&] {
      swap(x2, x1);
      sum = 0;
      for (size_t k = 0; k < thread_num; k++) {
        sum += sumR[k];
      }
      err = sqrt(sum);
      // err = errorVEC(x2, x1, N);
    });
    endconv = chrono::system_clock::now();
  }
}

/*** MAIN ***/
int main(int argc, char const *argv[]) {
  size_t N = atoi(argv[1]);
  size_t maxiter = atoi(argv[2]);
  float epsilon = atof(argv[3]);
  size_t W = atoi(argv[4]);
  size_t steps = atoi(argv[5]);

  size_t i, j, k, thread_num;
  float temp, conv, sum;
  // INIT
  vector<vector<float>> R(N, vector<float>(N));
  vector<float> D(N);
  vector<float> b(N);
  vector<float> x1(N);
  vector<float> x2(N);

  chrono::time_point<chrono::system_clock> startFor, endFor;

  // JACOBI METHOD parallel

  for (size_t Worker = 0; Worker < W; Worker += steps) {
    thread_num = Worker;
    if (Worker == 0)
      thread_num = 1;
    conv = 0;
    printf("{'thread_num':%zu,'Tc':[", thread_num);
    for (size_t iavg = 0; iavg < 4; iavg++) {
      /* generate  matrix and vectors: */
      srand(123);
      for (i = 0; i < N; i++) {
        b[i] = rand() % 10;
        x1[i] = 0;
        x2[i] = 0;
      }
      for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
          if (i != j)
            R[i][j] = rand() % 10;
          else {
            R[i][j] = 0;
            D[i] = rand() % 10;
          }

      /* enforce diagonal dominance */
      sum = 0.0;
      for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
          if (i != j) {
            sum += ((R[i][j] >= 0.0) ? R[i][j] : -R[i][j]);
          }
        }
        D[i] = sum + 100;
        sum = 0.0;
      }

      /* code */
      startFor = chrono::system_clock::now();
      vector<thread> t;
      barrier bar(thread_num);
      k = (N / thread_num);
      err = 1;
      for (size_t i = 0; i < thread_num; i++) {
        const size_t start = i * k;
        const size_t end = (i != thread_num - 1 ? start + k : N) - 1;
        // printf("Thread %zu: (%zu,%zu) \t #Row %zu \n", i, start, end,
        // end - start + 1);
        t.push_back(thread(iter, ref(R), ref(D), ref(b), ref(x1), ref(x2),
                           start, end, ref(bar), maxiter, epsilon, N, i,
                           thread_num));
      }
      for (thread &it : t)
        it.join();

      endFor = chrono::system_clock::now();

      // print the time for the post analysis
      if (iavg != 9) {
        printf("%f,", eTime(startFor, endFor).count());
      } else {
        printf("%f", eTime(startFor, endFor).count());
      }

      // for avg Tnorm time
      conv += eTime(startconv, endconv).count();
    }

    printf("],'Tnorm':%f,'Ax-b':%f,'Conv':%f}\n", conv / 10,
           error(R, D, x1, b, N), err);
  }
}
