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

/*** MAIN ***/
int main(int argc, char const *argv[]) {
  int N = atoi(argv[1]);
  int maxiter = atoi(argv[2]);
  float epsilon = atof(argv[3]);

  int i, j, k;
  float sum, err, conv;
  // INIT
  vector<vector<float>> A(N, vector<float>(N));
  vector<float> x(N);
  vector<float> b(N);
  vector<float> c(N);
  vector<chrono::duration<double>> innerDuration(N);

  chrono::time_point<chrono::system_clock> startFor, endFor, startHinnerFor,
      endHinnerFor, startError, endError, startconv, endconv;

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
  // JACOBI METHOD
  k = 0;
  float temp;
  while (k <= maxiter) {
    temp = 0.0;
    startFor = chrono::system_clock::now();
    for (int i = 0; i < N; i++) {
      c[i] = b[i];
      startHinnerFor = chrono::system_clock::now();
      for (int j = 0; j < N; j++) {
        if (i != j)
          c[i] = c[i] - A[i][j] * x[j];
      }
      endHinnerFor = chrono::system_clock::now();
      temp += eTime(startHinnerFor, endHinnerFor).count();
      c[i] = c[i] / A[i][i];
    }
    swap(c, x);
    err = errorVEC(c, x, N);
    endFor = chrono::system_clock::now();
    if (err < epsilon)
      break;

    /*
    startError = chrono::system_clock::now();
    err = error(A, x, b, N);
    endError = chrono::system_clock::now();

    startconv = chrono::system_clock::now();
    conv = error(c, x, N);
    endconv = chrono::system_clock::now();

    printf("%d Error:%f ", k, err);
    printf("Conv:%f ", conv);

    printf("ErrorTime: %f  ", eTime(startError, endError).count());
    printf("ConvTime: %f  ", eTime(startconv, endconv).count());

    printf("AvgHinnerFor%f ", temp / N);
    printf("ExternalForTime: %f\n", eTime(startFor, endFor).count());
    */
    k++;
  }

  printf("Error:%f ", error(A, x, b, N));
  printf("Conv:%f \n", errorVEC(c, x, N));
}