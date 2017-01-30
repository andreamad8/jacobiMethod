#include "bar.hpp"
#include "helper.hpp"
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
using namespace jac;
float err;

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
