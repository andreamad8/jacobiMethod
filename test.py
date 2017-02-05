#pragma vector aligned
#pragma simd

sum = -(*A)[i][i] * (*x)[i];
for (size_t j = 0; j < A->size(); j++) {
    sum += (*A)[i][j] * (*x)[j];
}
(*c)[i] = ((*b)[i] - sum) / (*A)[i][i];


void iter(vector<vector<float>> &A, vector<float> &b, vector<float> &x,
          vector<float> &c, const int from, const int to, barrier &bar,
          const int maxiter, const float epsilon) {
  float sum;
  for (size_t k = 0; k <= maxiter or err < epsilon; k++) {
    for (size_t i = from; i <= to; i++) {
      sum = -A[i][i] * x[i];
#pragma ivdep
#pragma vector aligned
#pragma simd
      for (size_t j = 0; j < A.size(); j++) {
        sum = sum + A[i][j] * x[j];
      }
      c[i] = (b[i] - sum) / A[i][i];
    }
    bar.await([&] {
      startconv = chrono::system_clock::now();
      swap(c, x);
      err = errorVEC(c, x, A.size());
      endconv = chrono::system_clock::now();
    });
  }
}
