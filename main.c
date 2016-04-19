#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "complex.h"

#define buffer_size 1024*200

void fft_depth(complex float* x, complex float* X, int N, int depth);
void fft(complex double* x, complex double* X, int N);
void ifft(complex double* x, complex double* X, int N);

void add_sq(float* buffer, float amp, int freq, int start, int end){
  int sqf;
  int i;
  sqf=0;
  for(i=start; i<end; ++i){
    //printf("%d %d\n",i, buffer[i]);
    // data manipulation
    //buffer[i]=buffer[i]/6;
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

void dft(complex float* x, complex float* X, int N){
  int n,k;
  complex float s,z;
  z=0;
  for(k=0; k<N; ++k){
    fprintf(stderr,"%d %d\n",k,N);
    s=0;
    z=-2*M_PI*I*k/N;
    for(n=0; n<N; ++n){
      s+=x[n]*cexp(z*n);
    }
    X[k]=s;
  }
  return;
}

void idft(complex float* x, complex float* X, int N){
  int n,k;
  complex float s,z;
  for(n=0; n<N; ++n){
    fprintf(stderr,"%d %d\n",n,N);
    s=0;
    z=2*M_PI*I*n/N;
    for(k=0; k<N; ++k){
      s+=X[k]*cexp(z*k);
    }
    x[n]=s/N;
  }
  return;
}

void lpf(double* buffer, int size, int cutoff){
  int i;
  complex double* x_buffer=(complex double*) malloc(sizeof(complex double)*size);
  complex double* X_buffer=(complex double*) malloc(sizeof(complex double)*size);
  for(i=0; i<size; ++i){
    x_buffer[i]=(complex double) buffer[i];
    //x_buffer[i]=(i)%8+1;
    //fprintf(stderr,"%f\n",creal(x_buffer[i]));
  }
  
  //fprintf(stderr,"test\n");
  //fft_depth(x_buffer, X_buffer, size,6);
 
  fft(x_buffer, X_buffer, size);
  
  for(i=(size/2)-cutoff; i<(size/2)+cutoff; ++i){
    X_buffer[i]=0.0;
  }
  /*for(i=0; i<size; ++i){
    X_buffer[i]=0.0;
  }
  */
  //idft(x_buffer, X_buffer, size);
  ifft(X_buffer, x_buffer, size);
  for(i=0; i<size; ++i){
    fprintf(stderr,"%f %f %f\n",buffer[i],creal(x_buffer[i]),cimag(x_buffer[i]));
    buffer[i]=creal(x_buffer[i]);
  }
}


struct Header{
  int head_size;
  int data_size;
  char* string;
};


void read_data(struct Header *h, double** data_buffer){
  
  unsigned char buffer[buffer_size];
  char head[buffer_size];
  //double buffer_fl[buffer_size];
  double* buffer_fl;
  char* ns;
  int i, df,sqf;
  int data_start_i;
  int data_size;
  unsigned char uc;
  
  df=0;
  sqf=0;
  //scanf("%c",buffer);
  data_size=-1;
  for(i=0; data_size==-1; ++i){
    scanf("%c",head+i);
    if(i>7 && memcmp(head+i-7,"data",4)==0){
      data_size=((int) head[i-3])+256*((int) head[i-2])+256*256*((int) head[i-1])+256*256*256*((int) head[i]);
      h->data_size=data_size;
    }
  }
  h->head_size=i;
  head[h->head_size]='\0';

  h->string=(char*) malloc(h->head_size*sizeof(char));  
  for(i=0; i<h->head_size; ++i){
    h->string[i]=head[i];
  }
  *data_buffer=(double*) malloc(data_size*sizeof(double));
  for(i=h->head_size; i<h->head_size+data_size; ++i){
    scanf("%c",&uc);
    (*data_buffer)[i-h->head_size]=(double) uc;
  }
  return;
}

void write_data(struct Header *h, double* data_buffer){
  int i;
  for(i=0; i<h->head_size; ++i){
    //fwrite(h->string+i,1,1,stdout);
    printf("%c",h->string[i]);
  }
  for(i=0; i<h->data_size; ++i){
    printf("%c",(unsigned char) data_buffer[i]);
  }
  return;
}

int main(){
  struct Header h;
  double* data_buffer;
  //double cf=11025.0-k*11025.0/h.data_size;
  double cf=1000.0;
  int cutoff=(int) ((11025-cf)*h.data_size/11025.0);
  read_data(&h,&data_buffer);
  lpf(data_buffer, h.data_size, cutoff);
  write_data(&h,data_buffer);

  fprintf(stderr,"%d",cutoff);
  
  return 0;
}
