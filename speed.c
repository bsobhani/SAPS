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

/*
void speed_down(double* x, int N1, double* y, int N2);

void speed_down(double* x, int N1, double* y, int N2){
  int i,j;
  int w;
  j=0;
  w=N1/(N2-N1);
  for(i=0; i<N1; ++i){
    y[i+j]=x[i];
    if(i%w==0){
      ++j;
      y[i+j]=x[i];
    }
  }
}
*/

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
  for(i=0; i<N1; ++i){
      y[i]=x[i*f];
  }
}

int main(int argc, char** argv){
  struct Header h;
  double* data_buffer;
  double* y;
  double factor;
  int num, den;
  read_data(&h,&data_buffer);
  fprintf(stderr, "%s\n",argv[1]);
  sscanf(argv[1],"%lf",&factor);
  den=10;
  y=(double*) malloc(den*h.data_size*sizeof(double));
  /*speed_down(data_buffer,h.data_size, y, Ny);*/
  num=(int) (factor*den);
  downsample(data_buffer,h.data_size,y,den);
  free(data_buffer);
  upsample(y,(h.data_size*den)/num,y,num);

  h.data_size=(h.data_size*den)/num;
  write_data(&h,y);
  free(y);
  free(h.string);
  return 0;
}
