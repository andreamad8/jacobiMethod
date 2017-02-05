#include <chrono>
#include <cmath>
#include <condition_variable> // std::condition_variable
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

float err;
size_t N;
vector<float> sum(255);
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

float error(vector<vector<float>> &A, vector<float> &x, vector<float> &b,
            int N) {
  float err = 0;
  float sum = 0;
  for (int i = 0; i < N; i++) {
    sum = 0;
    for (int j = 0; j < N; j++)
      sum += A[i][j] * x[j];
    err += abs(b[i] - sum);
  }
  return err / N;
}

float errorVEC(vector<float> &x1, vector<float> &x2, int N) {
  float sum = 0;
  for (int i = 0; i < N; i++) {
    sum += pow(x1[i] - x2[i], 2);
  }

  return sqrt(sum);
}

chrono::duration<double> eTime(chrono::time_point<chrono::system_clock> start,
                               chrono::time_point<chrono::system_clock> end) {
  return end - start;
}

class barrier {
private:
  int N_THREADS;
  int counts[2];
  int current;
  mutex lock;
  condition_variable condition;

public:
  barrier(int n);
  bool await(function<void()> cb);
};

barrier::barrier(int n) {
  N_THREADS = n;
  counts[0] = 0;
  counts[1] = 0;
  current = 0;
}

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

void iter(vector<vector<float>> &A, vector<float> &b, vector<float> &x,
          vector<float> &d, vector<float> &y, int from, int to, barrier &bar,
          int maxiter, float epsilon, size_t thid, size_t worker) {

  for (size_t k = 0; k <= maxiter and err >= epsilon; k++) {
    for (size_t i = from; i <= to; i++) {
      d[i] = b[i];
      for (size_t j = 0; j < A.size(); j++) {
        d[i] -= A[i][j] * x[j];
      }
      d[i] /= A[i][i];
      y[i] += d[i];
      sum[thid] += ((d[i] >= 0.0) ? d[i] : -d[i]);
    }

    bar.await([worker, &x, &y] {
      for (size_t i = 0; i <= N; i++) {
        x[i] = y[i];
      }
      startconv = chrono::system_clock::now();
      err = 0;
      for (size_t i = 0; i < worker; i++) {
        err += sum[i];
        sum[i] = 0;
      }
      endconv = chrono::system_clock::now();
    });
  }
}

/*** MAIN ***/
int main(int argc, char const *argv[]) {
  N = atoi(argv[1]);
  size_t maxiter = atoi(argv[2]);
  float epsilon = atof(argv[3]);
  size_t W = atoi(argv[4]);
  size_t steps = atoi(argv[5]);

  size_t i, j, k, thread_num;
  float temp, conv;
  // INIT
  //__declspec(align(16, 0)) vector<vector<float>> A(N, vector<float>(N));
  //__declspec(align(16, 0)) vector<float> x(N);
  //__declspec(align(16, 0)) vector<float> b(N);
  //__declspec(align(16, 0)) vector<float> c(N);
  vector<vector<float>> A(N, vector<float>(N));
  __declspec(align(16, 0)) vector<float> x(N);
  vector<float> b(N);
  vector<float> d(N);
  __declspec(align(16, 0)) vector<float> y(N);

  chrono::time_point<chrono::system_clock> startFor, endFor;

  // JACOBI METHOD parallel

  for (size_t Worker = 0; Worker < W; Worker += steps) {
    thread_num = Worker;
    if (Worker == 0)
      thread_num = 1;
    conv = 0;
    printf("{'thread_num':%zu,'Tc':[", thread_num);
    for (size_t iavg = 0; iavg < 10; iavg++) {
      /* generate  matrix and vectors: */
      srand(123);
      for (i = 0; i < N; i++) {
        b[i] = rand() % 10;
        x[i] = 0;
        d[i] = 0;
        y[i] = 0;
      }
      for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
          A[i][j] = rand() % 10;

      /* enforce diagonal dominance */
      temp = 0.0;
      for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
          if (i != j) {
            temp += abs(A[i][j]);
          }
        }
        A[i][i] = temp + 100;
        temp = 0.0;
      }
      for (size_t i = 0; i < W; i++) {
        sum[i] = 0;
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
        t.push_back(thread(iter, ref(A), ref(b), ref(x), ref(d), ref(y), start,
                           end, ref(bar), maxiter, epsilon, i, thread_num));
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

    printf("],'Tnorm':%f,'Ax-b':%f,'Conv':%f}\n", conv / 10, error(A, x, b, N),
           err);
  }
}
