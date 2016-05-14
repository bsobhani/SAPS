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

void echo(double* x, int Nx, double growth, int spacing, double* y){
  int i,j;
  int numEchos=(Nx+spacing)/spacing;
  double weight;
  double sumWeights;

  sumWeights=0;
  weight=1;
  for(i=0; i<numEchos; ++i){
    sumWeights+=weight;
    weight*=growth;
  }
  /*fprintf(stderr,"%f",sumWeights);*/

  for(i=0; i<Nx; ++i)
    y[i]=0;

  for(i=0; i<Nx; ++i){
    weight=1;
    for(j=0; j<numEchos; ++j){
      if(i>=j*spacing){
	y[i]+=weight*(x[i-j*spacing])/sumWeights;
	weight=weight*growth;
      }
    }
  }

  return;
}
  

int main(int argc, char** argv){
  struct Header h;
  double* data_buffer;
  double* y;
  double factor;
  int data_size;
  int spacing;
  double time_spacing;
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
  sscanf(argv[2],"%lf",&time_spacing);
  y_size=data_size;
  y=(double*) malloc(y_size*sizeof(double));

  spacing=(int) (time_spacing*h.SampleRate/1000.0);
  echo(data_buffer, data_size, factor, spacing, y);
  
  /*conv(data_buffer, data_size, ir, ir_size, y);*/
  /*h.Subchunk2Size=(data_size*den*h.BitsPerSample/8)/num;*/
  write_data(&h,y);
  free(y);
  return 0;
}
