#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "complex.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


/* An implementation of the Cooley-Tukey FFT */
void fft(complex double* x, complex double* X, int N){
  complex double W;
  complex double** A1;
  int j1,j0,k1,k0,r1,r2,i,m,a1;
  complex double** cp1;

  complex double b;
  complex double inc;
  
  /* Determining reasonable values for r1 and r2 */
  r1=(int) sqrt(N);
  while(N%r1!=0)
    r1--;
  r2=N/r1;

  for(i=0; i<N; ++i){
    X[i]=0;
  }
  
  A1=(complex double**) malloc(r1*sizeof(complex double*));
  for(i=0; i<r1; ++i){
    A1[i]=(complex double*) malloc(r2*sizeof(complex double));
    for(m=0; m<r2; ++m){
      A1[i][m]=0;
    }
  }
  
  W=cexp(-2*M_PI*I/N); /* Note: W=cexp(2*M_PI*I/N) in the Cooley-Tukey paper */

  /* Precomputing cpow(W,j0*k1*r2) */
  cp1=(complex double**) malloc(r1*sizeof(complex double*));
  for(j0=0; j0<r1; ++j0){
    cp1[j0]=(complex double*) malloc(r1*sizeof(complex double));
    for(k1=0; k1<r1; ++k1){
      cp1[j0][k1]=cpow(W,j0*k1*r2);
    }
  }

  /* Computing A1 */
  for(j0=0; j0<r1; ++j0){
    for(k0=0; k0<r2; ++k0){
      for(k1=0; k1<r1; ++k1){
	A1[j0][k0]+=x[k1*r2+k0]*cpow(W,j0*k1*r2);
	/*A1[j0][k0]+=x[k1*r2+k0]*cp1[j0][k1];*/
      }
    }  
  }

  /* Computing the output ("A" in the Cooley-Tukey paper) */
  for(j1=0; j1<r2; ++j1){
    a1=j1*r1;
    for(j0=0; j0<r1; ++j0){
      b=1;
      inc=cpow(W,a1);
      for(k0=0; k0<r2; ++k0){
	/*X[j1*r1+j0]+=A1[j0][k0]*cpow(W,(j1*r1+j0)*k0);*/
	/*X[a1]+=A1[j0][k0]*cpow(W,a1*k0);*/
	X[a1]+=A1[j0][k0]*b;
	b*=inc;
      }
      a1+=1;
    }
  }

  /* Freeing data */
  for(i=0; i<r1; ++i)
    free(A1[i]);
  free(A1);

  for(i=0; i<r1; ++i)
    free(cp1[i]);
  free(cp1);
  
  return;
}

/* An implementation of the Cooley-Tukey FFT */
void fft_float(complex float* x, complex float* X, int N){
  complex float W;
  complex float** A1;
  int j1,j0,k1,k0,r1,r2,i,m,a1;
  complex float** cp1;

  complex float b;
  complex float inc;
  
  /* Determining reasonable values for r1 and r2 */
  r1=(int) sqrt(N);
  while(N%r1!=0)
    r1--;
  r2=N/r1;

  for(i=0; i<N; ++i){
    X[i]=0;
  }
  
  A1=(complex float**) malloc(r1*sizeof(complex float*));
  for(i=0; i<r1; ++i){
    A1[i]=(complex float*) malloc(r2*sizeof(complex float));
    for(m=0; m<r2; ++m){
      A1[i][m]=0;
    }
  }
  
  W=cexp(-2*M_PI*I/N); /* Note: W=cexp(2*M_PI*I/N) in the Cooley-Tukey paper */

  /* Precomputing cpow(W,j0*k1*r2) */
  cp1=(complex float**) malloc(r1*sizeof(complex float*));
  for(j0=0; j0<r1; ++j0){
    cp1[j0]=(complex float*) malloc(r1*sizeof(complex float));
    for(k1=0; k1<r1; ++k1){
      cp1[j0][k1]=cpow(W,j0*k1*r2);
    }
  }

  /* Computing A1 */
  for(j0=0; j0<r1; ++j0){
    for(k0=0; k0<r2; ++k0){
      for(k1=0; k1<r1; ++k1){
	A1[j0][k0]+=x[k1*r2+k0]*cpow(W,j0*k1*r2);
	/*A1[j0][k0]+=x[k1*r2+k0]*cp1[j0][k1];*/
      }
    }  
  }

  /* Computing the output ("A" in the Cooley-Tukey paper) */
  for(j1=0; j1<r2; ++j1){
    a1=j1*r1;
    for(j0=0; j0<r1; ++j0){
      b=1;
      inc=cpow(W,a1);
      for(k0=0; k0<r2; ++k0){
	/*X[j1*r1+j0]+=A1[j0][k0]*cpow(W,(j1*r1+j0)*k0);*/
	/*X[a1]+=A1[j0][k0]*cpow(W,a1*k0);*/
	X[a1]+=A1[j0][k0]*b;
	b*=inc;
      }
      a1+=1;
    }
  }

  /* Freeing data */
  for(i=0; i<r1; ++i)
    free(A1[i]);
  free(A1);

  for(i=0; i<r1; ++i)
    free(cp1[i]);
  free(cp1);
  
  return;
}
  

void ifft_float(complex float* X, complex float* x, int N){
  complex float* xr;
  int i;
  xr=(complex float*) malloc(sizeof(complex float)*N);
  fft_float(X, xr, N);
  x[0]=xr[0];
  for(i=1; i<N; ++i){
    x[i]=xr[N-i];
  }
  for(i=0; i<N; ++i){
    x[i]=x[i]/N;
  }
  free(xr);
}

void ifft(complex double* X, complex double* x, int N){
  complex double* xr;
  int i;
  xr=(complex double*) malloc(sizeof(complex double)*N);
  fft(X, xr, N);
  x[0]=xr[0];
  for(i=1; i<N; ++i){
    x[i]=xr[N-i];
  }
  for(i=0; i<N; ++i){
    x[i]=x[i]/N;
  }
  free(xr);
}
