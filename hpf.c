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

void hpf(double* buffer, int size, int cutoff){
  int i;
  complex double* x_buffer=(complex double*) malloc(sizeof(complex double)*size);
  complex double* X_buffer=(complex double*) malloc(sizeof(complex double)*size);
  for(i=0; i<size; ++i){
    x_buffer[i]=(complex double) buffer[i];
  }
 
  fft(x_buffer, X_buffer, size);
  
  for(i=1; i<cutoff; ++i){
    X_buffer[i]=0.0;
  }
  for(i=size-cutoff; i<size; ++i){
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

  if(argc!=2){
    fprintf(stderr,"usage: %s cutoffFrequency < instream > outstream\n",argv[0]);
    fprintf(stderr,"example: %s 10000 < horn.wav > hornFiltered.wav\n",argv[0]);
    return 1;
  }
  
  sscanf(argv[1],"%lf",&cf);
  read_data(&h,&data_buffer);
  cutoff=(int) (cf*h.data_size/11025.0);
  hpf(data_buffer, h.data_size, cutoff);
  write_data(&h,data_buffer);
  
  return 0;
}
