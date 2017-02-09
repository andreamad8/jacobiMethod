#include <chrono>
#include <cmath>
#include <condition_variable> // std::condition_variable
#include <ff/parallel_for.hpp>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;
using namespace ff;
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
  vector<vector<float>> R(N, vector<float>(N));
  vector<float> D(N);
  vector<float> b(N);
  vector<float> x1(N);
  vector<float> x2(N);
  chrono::time_point<chrono::system_clock> startFor, endFor, startconv, endconv;

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
      // SPIN, no barrier
      ParallelForReduce<float> pf(thread_num, true, true);

      // printMAT(A, N);
      // printVEC(b, N);
      // JACOBI METHOD

      err = 1;
      startFor = chrono::system_clock::now();
      for (size_t k = 0; k <= maxiter and err >= epsilon; k++) {
        pf.parallel_for(0, N, 1, 0,
                        [&](const long i) {
                          float sum = 0.0;
                          for (size_t j = 0; j < N; j++) {
                            sum = sum - R[i][j] * x1[j];
                          }
                          x2[i] = (b[i] - sum) / D[i];
                        },
                        thread_num);

        startconv = chrono::system_clock::now();
        swap(x2, x1);
        float sumR = 0.0;
        pf.parallel_reduce(
            sumR, 0.0, 0, N, 1, 0,
            [&](const long i, float &mysum) { mysum += pow(x1[i] - x2[i], 2); },
            [](float &s, const float &e) { s += e; }, thread_num);
        err = sqrt(sumR);
        // err = errorVEC(x2, x1, N);
        endconv = chrono::system_clock::now();
      }
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
           error(R, D, x1, b, N), errorVEC(x2, x1, N));
  }
}
