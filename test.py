#pragma vector aligned
#pragma simd

sum = -(*A)[i][i] * (*x)[i];
#pragma vector aligned
#pragma ivdep
for (size_t j = 0; j < A->size(); j++) {
    sum += (*A)[i][j] * (*x)[j];
}
(*c)[i] = ((*b)[i] - sum) / (*A)[i][i];
