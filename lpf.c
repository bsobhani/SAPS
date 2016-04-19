#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "complex.h"
#include "file_io.h"

#define buffer_size 1024*200

#ifndef M_PI
#define M_PI
#endif

void fft(complex double* x, complex double* X, int N);
void ifft(complex double* x, complex double* X, int N);

void add_sq(float* buffer, float amp, int freq, int start, int end){
  int sqf;
  int i;
  sqf=0;
  for(i=start; i<end; ++i){
    if(sqf==1){
      buffer[i]+=amp;
    }
    else{
      buffer[i]+=-amp;
    }
    if(i%freq==0){
      if(sqf==0)
	sqf=1;
      else if(sqf==1)
	sqf=0;
    }
  }
}

void lpf(double* buffer, int size, int cutoff){
  int i;
  complex double* x_buffer=(complex double*) malloc(sizeof(complex double)*size);
  complex double* X_buffer=(complex double*) malloc(sizeof(complex double)*size);
  for(i=0; i<size; ++i){
    x_buffer[i]=(complex double) buffer[i];
  }
 
  fft(x_buffer, X_buffer, size);
  
  for(i=cutoff; i<size-cutoff; ++i){
    X_buffer[i]=0.0;
  }
  ifft(X_buffer, x_buffer, size);
  for(i=0; i<size; ++i){
    buffer[i]=creal(x_buffer[i]);
  }
}

int main(int argc, char** argv){
  struct Header h;
  double* data_buffer;
  double cf=800.0;
  int cutoff;
  sscanf(argv[1],"%lf",&cf);
  read_data(&h,&data_buffer);
  cutoff=(int) (cf*h.data_size/11025.0);
  lpf(data_buffer, h.data_size, cutoff);
  write_data(&h,data_buffer);
  
  return 0;
}
