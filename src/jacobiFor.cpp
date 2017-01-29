#include "helper.hpp"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <climits>
#include <cmath>
#include <condition_variable> // std::condition_variable
#include <cstdio>
#include <cstdlib>
#include <ff/parallel_for.hpp>
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
using namespace ff;
using namespace jac;

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

  ParallelFor pf;

  // printMAT(A, N);
  // printVEC(b, N);
  // JACOBI METHOD
  k = 0;
  while (k <= maxiter) {
    pf.parallel_for(0, N,
                    [&](const long i) {
                      c[i] = b[i];
                      for (int j = 0; j < N; j++) {
                        if (i != j)
                          c[i] = c[i] - A[i][j] * x[j];
                      }
                      c[i] = c[i] / A[i][i];
                    },
                    thread_num);
    swap(c, x);
    err = errorVEC(c, x, N);
    if (err < epsilon)
      break;
    k++;
  }

  printf("Error:%f ", error(A, x, b, N));
  printf("Conv:%f \n", errorVEC(c, x, N));
}
