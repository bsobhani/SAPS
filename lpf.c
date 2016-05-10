#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "complex.h"
#include "file_io.h"
#include "window_functions.h"
#include "argp.h"

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

void lpf_windowed(double* buffer, int size, int cutoff){
  int i;
  complex double* x_buffer=(complex double*) malloc(sizeof(complex double)*size);
  complex double* X_buffer=(complex double*) malloc(sizeof(complex double)*size);
  for(i=0; i<size; ++i){
    x_buffer[i]=(complex double) buffer[i];
  }
 
  fft(x_buffer, X_buffer, size);
  hannf(X_buffer,size);
  for(i=cutoff; i<size-cutoff; ++i){
    X_buffer[i]=0.0;
  }
  ifft(X_buffer, x_buffer, size);
  for(i=0; i<size; ++i){
    buffer[i]=creal(x_buffer[i]);
  }
}

struct arguments{
  int window;
};

static struct argp_option options[]={
  {"hanning",'n',0,0,"Use Hanning window when filtering"},
  {0}
};

static error_t parse_opt(int key, char* arg, struct argp_state *state){
  struct arguments *arguments=state->input;
  switch(key){
  case 'n':
    arguments->window=1;
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp={options,parse_opt,0,0};

int main(int argc, char** argv){
  struct Header h;
  double* data_buffer;
  double cf=800.0;
  int cutoff;
  /*
  if(argc!=2){
    fprintf(stderr,"usage: %s cutoffFrequency < instream > outstream\n",argv[0]);
    fprintf(stderr,"example: %s 10000 < horn.wav > hornFiltered.wav\n",argv[0]);
    return 1;
  }
  */

  int arg_index;
  struct arguments arguments;
  arguments.window=0;
  argp_parse (&argp, argc, argv, 0, &arg_index, &arguments);
  
  sscanf(argv[arg_index],"%lf",&cf);
  read_data(&h,&data_buffer);
  cutoff=(int) (cf*h.data_size/11025.0);
  switch(arguments.window){
  case 0:
    lpf(data_buffer, h.data_size, cutoff);
    break;
  case 1:
    lpf_windowed(data_buffer, h.data_size, cutoff);
    break;
  }
  write_data(&h,data_buffer);
  
  return 0;
}
