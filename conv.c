#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "complex.h"
#include "file_io.h"
#include "window_functions.h"
#include "argp.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void fft(complex double* x, complex double* X, int N);
void ifft(complex double* x, complex double* X, int N);

void conv(double* x1, int N1, double* x2, int N2, double* y){
  int Ny=N2+N1-1;
  int i, j;
  double s1;
  for(i=0; i<Ny; ++i){
    s1=0;
    for(j=0; j<N1 && (i-j)>=0; ++j){
      if((i-j)>=N2){
	continue;
      }
      s1+=x1[j]*x2[i-j];
    }
    y[i]=s1;
  }
  return;
}
