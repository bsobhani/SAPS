#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "complex.h"
#include "file_io.h"
#include "math.h"

#define buffer_size 1024*200

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void downsample(double* x, int N1, double* y, int f){
  int i,j;
  j=0;
  for(i=0; i<N1; ++i){
    for(j=0; j<f; ++j){
      y[i*f+j]=x[i];
    }
  }
}

void upsample(double* x, int N1, double* y, int f){
  int i;
  for(i=0; i<N1/f; ++i){
      y[i]=x[i*f];
  }
}

void atten(double* x, int Nx, double alpha, double* y){
  /* Note alpha=1/(sampling_rate*time_constant) */
  /* exp(-t/time_constant) => exp(-(n/sampling_rate)/time_constant) = exp(-alpha*n) */
  int n;
  
  for(n=0; n<Nx; ++n){
    y[n]=x[n]*exp(-alpha*n);
  }

  return;
}
  

int main(int argc, char** argv){
  struct Header h;
  double* data_buffer;
  double* y;
  double time_constant_ms;
  int data_size;
  int y_size;
  
  if(argc<2){
    fprintf(stderr,"example usage: %s 300 < horn.wav > hornAttenuated.wav\n",argv[0]);
    return 1;
  }
  /* Read data from stdin (see file_io.c) */
  if(read_data(&h,&data_buffer)!=0){
    return 1;
  }
  data_size=(h.Subchunk2Size*8)/(h.BitsPerSample);
  sscanf(argv[1],"%lf",&time_constant_ms);
  y_size=data_size;
  y=(double*) malloc(y_size*sizeof(double));

  atten(data_buffer, data_size, 1000/(h.SampleRate*time_constant_ms), y);
  
  write_data(&h,y);
  free(y);
  return 0;
}
