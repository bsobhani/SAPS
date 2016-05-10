#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "complex.h"
#include "file_io.h"

#define buffer_size 1024*200

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void downsample(double* x, int N1, double* y, int f);
void upsample(double* x, int N1, double* y, int f);
void fft(complex double* x, complex double* X, int N);
//void pitch(double* x, int N1, int p, double* y){
  

void tempo(double* x, int N1, int p, double* y, int N2){
  int i,j;
  int num,den;
  int p2;
  double factor;
  complex double* cv;
  complex double* xc;
  complex double* ds;
  factor=((float) N2)/((float) N1);
  den=10;
  num=(int) (factor*den);
  xc=(complex double*) malloc(sizeof(complex double)*p);
  ds=(complex double*) malloc(sizeof(complex double)*p*den);
  for(i=0; i<N1; ++i){
    xc[i]=x[i];
  }
  p2=N2/(N1/p);
  fft(xc,cv,N1);
  downsample(ds,N1,den);
  upsample(ds,N1*den,num);
  
  for(i=0; i<(N1/p); ++i){
    for(j=0; j<p2; ++j){
      y[i*p2+j]=x[i*p+(j%p)];
    }
  }
  return;
}

int main(int argc, char** argv){
  struct Header h;
  double* data_buffer;
  double* y;
  int p;
  int N2;
  read_data(&h,&data_buffer);
  fprintf(stderr, "%s\n",argv[1]);
  sscanf(argv[1],"%d",&p);
  N2=(int) (1.5*h.data_size);
  y=(double*) malloc((int) (N2*sizeof(double)));

  tempo(data_buffer, h.data_size, p, y, N2);

  h.data_size=(h.data_size);
  write_data(&h,y);
  //free(y);
  //free(h.string);
  return 0;
}
