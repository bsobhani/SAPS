#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "complex.h"
#include "window_functions.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void fft(complex double* x, complex double* X, int N);

/* Hanning window */
void hannf(complex double* X, int N){
  complex double* w;
  complex double* W;
  int n;
  w=(complex double*) malloc(N*sizeof(complex double));
  W=(complex double*) malloc(N*sizeof(complex double));
  for(n=0; n<N; ++n){
    w[n]=.5-.5*cos((2*M_PI*n)/(N-1));
  }
  fft(w,W,N);
  for(n=0; n<1000; ++n){
    X[n]=X[n]*W[n];
    /*fprintf(stderr,"%f %d\n",creal(w[n]),N);*/
  }
  return;
}
