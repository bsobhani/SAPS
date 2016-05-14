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

int main(int argc, char** argv){
  struct Header h;
  double* data_buffer;
  double* y;
  double factor;
  int num, den;
  int data_size;
  if(argc<2){
    fprintf(stderr,"example usage: %s 1.5 < beep.wav > beepFast.wav\n",argv[0]);
    return 1;
  }
  /* Read data from stdin (see file_io.c) */
  if(read_data(&h,&data_buffer)!=0){
    fprintf(stderr,"Read error: Not a valid WAV file\n");
    return 1;
  }
  data_size=(h.Subchunk2Size*8)/(h.BitsPerSample);
  sscanf(argv[1],"%lf",&factor);
  den=10;
  y=(double*) malloc(den*data_size*sizeof(double));
  num=(int) (factor*den);
  downsample(data_buffer,data_size,y,den);
  free(data_buffer);
  upsample(y,(h.Subchunk2Size*8*den/h.BitsPerSample),y,num);

  h.Subchunk2Size=(data_size*den*h.BitsPerSample/8)/num;
  write_data(&h,y);
  free(y);
  return 0;
}
