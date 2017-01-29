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

float error(vector<float> &x1, vector<float> &x2, int N) {
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
/*** MAIN ***/
int main(int argc, char const *argv[]) {
  int N = atoi(argv[1]);
  int maxiter = atoi(argv[2]);
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
    endFor = chrono::system_clock::now();
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
  printf("Conv:%f \n", error(c, x, N));
}
