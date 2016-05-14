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

/* Add square wave to signal */
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

/* Lowpass filter */
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
  free(x_buffer);
  free(X_buffer);
}

/* Highpass filter*/
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
  free(x_buffer);
  free(X_buffer);
}

/* Bandstop filter */
void bsf(double* buffer, int size, int cutoff1, int cutoff2){
  int i;
  complex double* x_buffer=(complex double*) malloc(sizeof(complex double)*size);
  complex double* X_buffer=(complex double*) malloc(sizeof(complex double)*size);
  for(i=0; i<size; ++i){
    x_buffer[i]=(complex double) buffer[i];
  }
 
  fft(x_buffer, X_buffer, size);
  
  for(i=1+cutoff1; i<cutoff2; ++i){
    X_buffer[i]=0.0;
  }
  for(i=size-cutoff2; i<size-cutoff1; ++i){
    X_buffer[i]=0.0;
  }
  ifft(X_buffer, x_buffer, size);
  for(i=0; i<size; ++i){
    buffer[i]=creal(x_buffer[i]);
  }
  free(x_buffer);
  free(X_buffer);
}

/* Bandpass filter */
void bpf(double* buffer, int size, int cutoff1, int cutoff2){
  int i;
  complex double* x_buffer=(complex double*) malloc(sizeof(complex double)*size);
  complex double* X_buffer=(complex double*) malloc(sizeof(complex double)*size);
  for(i=0; i<size; ++i){
    x_buffer[i]=(complex double) buffer[i];
  }
  
  fft(x_buffer, X_buffer, size);
  for(i=1; i<cutoff1; ++i){
    X_buffer[i]=0.0;
  }
  for(i=cutoff2; i<size-cutoff2; ++i){
    X_buffer[i]=0.0;
  }
  for(i=size-cutoff1; i<size; ++i){
    X_buffer[i]=0.0;
  }
  ifft(X_buffer, x_buffer, size);
  for(i=0; i<size; ++i){
    buffer[i]=creal(x_buffer[i]);
  }
  free(x_buffer);
  free(X_buffer);
}


struct arguments{
  int filter_type;
  double cutoff1;
  double cutoff2;
};

static struct argp_option options[]={
  {"lowpass",'l',0,0,"Remove frequencies above cutoff1"},
  {"highpass",'h',0,0,"Remove frequenies below cutoff1"},
  {"bandstop",'s',0,0,"Remove frequencies cutoff1 and cutoff2"},
  {"bandpass",'p',0,0,"Pass only between cutoff1 and cutoff2"},
  {0}
};

static error_t parse_opt(int key, char* arg, struct argp_state *state){
  struct arguments *arguments=state->input;
  switch(key){
  case 'l':
    arguments->filter_type=0;
    break;
  case 'h':
    arguments->filter_type=1;
    break;
  case 's':
    arguments->filter_type=2;
    break;
  case 'p':
    arguments->filter_type=3;
    break;

    
  case ARGP_KEY_ARG:
    /*fprintf(stderr,"%s\n",arg);*/
    if(arguments->cutoff1!=-1){
      sscanf(arg,"%lf",&(arguments->cutoff2));
    }
    else{
      sscanf(arg,"%lf",&(arguments->cutoff1));
    }
    break;
    
  case ARGP_KEY_END:
    if(state->arg_num < 2 && (arguments->filter_type==2 || arguments->filter_type==3)){
      /*fprintf(stderr,"%d\n",state->arg_num);*/
      argp_usage(state);
    }
    if(arguments->filter_type==-1){
      argp_usage(state);
    }
    if(state->arg_num<1){
      argp_usage(state);
    }
    break;
    
  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp={options,parse_opt,"cutoff1 [cutoff2]",0};

int main(int argc, char** argv){
  struct Header h;
  double* data_buffer;
  int cutoff;
  int arg_index;
  int data_size;
  struct arguments arguments;
  /* Setting default argument values */
  arguments.filter_type=-1;
  arguments.cutoff1=-1;
  arguments.cutoff2=-1;
  argp_parse (&argp, argc, argv, 0, &arg_index, &arguments);

  /* Read data from stdin (see file_io.c) */
  if(read_data(&h,&data_buffer)!=0){
    return 1;
  }
  data_size=h.Subchunk2Size*8/h.BitsPerSample;
  cutoff=(int) (arguments.cutoff1*data_size/h.SampleRate);
  switch(arguments.filter_type){
  case 0:
    lpf(data_buffer, data_size, cutoff);
    break;
  case 1:
    hpf(data_buffer, data_size, cutoff);
    break;
  case 2:
    bsf(data_buffer, data_size, cutoff, (int) (arguments.cutoff2*data_size/h.SampleRate));
    break;
  case 3:
    bpf(data_buffer, data_size, cutoff, (int) (arguments.cutoff2*data_size/h.SampleRate));
    break;
  default:
    break;
  }
  if(arguments.filter_type!=-1)
    write_data(&h,data_buffer);

  free(data_buffer);
  
  return 0;
}
