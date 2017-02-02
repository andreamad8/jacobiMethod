#include <chrono>
#include <cmath>
#include <condition_variable> // std::condition_variable
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

float err = 1;
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

void iter(vector<vector<float>> *A, vector<float> *b, vector<float> *x,
          vector<float> *c, int from, int to, barrier *bar, int maxiter,
          float epsilon) {
  float sum;
  for (size_t k = 0; k <= maxiter or err < epsilon; k++) {
    for (size_t i = from; i <= to; i++) {
      sum = -(*A)[i][i] * (*x)[i];
#pragma vector aligned
#pragma ivdep
      for (size_t j = 0; j < A->size(); j++) {
        sum += (*A)[i][j] * (*x)[j];
      }
      (*c)[i] = ((*b)[i] - sum) / (*A)[i][i];
    }
    bar->await([&] {
      startconv = chrono::system_clock::now();
      swap(*c, *x);
      err = errorVEC(*c, *x, A->size());
      endconv = chrono::system_clock::now();
    });
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
  float sum, err, conv;
  // INIT
  vector<vector<float>> A(N, vector<float>(N));
  vector<float> x(N);
  vector<float> b(N);
  vector<float> c(N);
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
        c[i] = 0;
      }
      for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
          A[i][j] = rand() % 10;

      /* enforce diagonal dominance */
      sum = 0.0;
      for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
          if (i != j) {
            sum += abs(A[i][j]);
          }
        }
        A[i][i] = sum + 100;
        sum = 0.0;
      }
      /* code */
      startFor = chrono::system_clock::now();

      vector<thread> t;
      barrier bar(thread_num);
      k = (N / thread_num);

      for (size_t i = 0; i < thread_num; i++) {
        size_t start = i * k;
        size_t end = (i != thread_num - 1 ? start + k : N) - 1;
        // printf("Thread %zu: (%zu,%zu) \t #Row %zu \n", i, start, end,
        //       end - start + 1);
        t.push_back(
            thread(iter, &A, &b, &x, &c, start, end, &bar, maxiter, epsilon));
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
           errorVEC(c, x, N));
  }
}
