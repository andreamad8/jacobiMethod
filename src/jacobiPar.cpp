#include <algorithm>
#include <cassert>
#include <chrono>
#include <climits>
#include <cmath>
#include <condition_variable> // std::condition_variable
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <list>
#include <math.h>
#include <mutex>
#include <queue>
#include <thread>
#include <tuple>
#include <unistd.h>
#include <vector>

using namespace std;
float err;
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
  lock;
  condition;
  return;
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
  for (size_t k = 0; k <= maxiter; k++) {
    for (size_t i = from; i <= to; i++) {
      (*c)[i] = (*b)[i];
      for (size_t j = 0; j < A->size(); j++) {
        if (i != j)
          (*c)[i] = (*c)[i] - (*A)[i][j] * (*x)[j];
      }
      (*c)[i] = (*c)[i] / (*A)[i][i];
    }
    bar->await([&] {
      swap(*c, *x);
      err = errorVEC(*c, *x, A->size());

    });
    if (err < epsilon)
      break;
  }
}

/*** MAIN ***/
int main(int argc, char const *argv[]) {
  size_t N = atoi(argv[1]);
  size_t maxiter = atoi(argv[2]);
  float epsilon = atof(argv[3]);
  size_t thread_num = atoi(argv[4]);
  size_t i, j, k;
  float sum, err, conv;
  // INIT
  vector<vector<float>> A(N, vector<float>(N));
  vector<float> x(N);
  vector<float> b(N);
  vector<float> c(N);

  /* generate  matrix and vectors: */
  srand(123);
  for (i = 0; i < N; i++) {
    b[i] = rand() % 10;
    x[i] = 0;
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

  // printMAT(A, N);
  // printVEC(b, N);
  // JACOBI METHOD parallel

  vector<thread> t;
  barrier bar(thread_num);
  k = (N / thread_num);
  for (size_t i = 0; i < thread_num; i++) {
    size_t start = i * k;
    size_t end = (i != thread_num - 1 ? start + k : N) - 1;
    // printf("Thread %zu: (%zu,%zu) \t #Row %zu \n", i, start, end,end - start
    // + 1);
    t.push_back(
        thread(iter, &A, &b, &x, &c, start, end, &bar, maxiter, epsilon));
  }
  for (thread &it : t)
    it.join();

  printf("Error:%f ", error(A, x, b, N));
  printf("Conv:%f \n", errorVEC(c, x, N));
}
