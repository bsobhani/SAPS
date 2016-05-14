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
  int i;
  double* ir;
  int ir_size=10240;
  int spacing=1024;
  int y_size;
  
  if(argc<2){
    fprintf(stderr,"example usage: %s 1.5 < beep.wav > beepFast.wav\n",argv[0]);
    return 1;
  }
  /* Read data from stdin (see file_io.c) */
  if(read_data(&h,&data_buffer)!=0){
    return 1;
  }
  data_size=(h.Subchunk2Size*8)/(h.BitsPerSample);
  sscanf(argv[1],"%lf",&factor);

  ir=(double*) malloc(ir_size*sizeof(double));

  for(i=0; i<ir_size; ++i){
    if(i==0){
      ir[i]=.1;
    }
    else if(i%spacing==0){
      ir[i]=ir[i-spacing]*factor;
    }
    else{
      ir[i]=0;
    }
  }

  y_size=data_size+ir_size-1;

  y=(double*) malloc(y_size*sizeof(double));
  
  conv(data_buffer, data_size, ir, ir_size, y);
  //h.Subchunk2Size=(data_size*den*h.BitsPerSample/8)/num;
  write_data(&h,y);
  free(y);
  return 0;
}
