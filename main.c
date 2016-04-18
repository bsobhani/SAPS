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

void lpf(double* buffer, int size, double cutoff){
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
  
  for(i=(size/2)-15000; i<(size/2)+15000; ++i){
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
  


int main(){
  
  unsigned char buffer[buffer_size];
  double buffer_fl[buffer_size];
  char* ns;
  int i, df,sqf;
  int data_start_i;
  int data_size;
  df=0;
  sqf=0;
  //scanf("%c",buffer);
  for(i=0; i<buffer_size; ++i){
    scanf("%c",buffer+i);
  }
  //printf(buffer);
  buffer[buffer_size-1]='\0';
  ns=strstr(buffer,"data");
  //printf("%s\n",strstr(buffer,"data"));
  //printf("%d\n",ns);
  //printf("%c\n",'w');
  for(i=0; i<buffer_size; ++i){
    //printf("%d %d\n",i, buffer[i]);
    if(df==1){
      //buffer_fl[i-data_start_i]=((double) (buffer[i]+128))/256;
      buffer_fl[i-data_start_i]=(double) buffer[i];
    }
    if(memcmp(buffer+i,"data",4)==0){
      //printf("%d\n",i);
      data_size=((int) buffer[i+4])+256*((int) buffer[i+5])+256*256*((int) buffer[i+6])+256*256*256*((int) buffer[i+7]);
      df=1;
      data_start_i=i+8;
      i=data_start_i;
    }
  }
  
  //add_sq(buffer_fl, 6.0, 5, 0, buffer_size);
  //add_sq(buffer_fl, 6.0, 3, 0, buffer_size);
  //add_sq(buffer_fl, 6.0, 4, 0, buffer_size);
  //lpf(buffer_fl, buffer_size-data_start_i, 0.0);
  //lpf(buffer_fl, 4*4, 0.0);
  lpf(buffer_fl, 40000, 0.0);
  //return 0;
  df=0;
  fprintf(stderr,"%d\n",data_size);
  for(i=0; i<buffer_size; ++i){
    //fprintf(stderr,"a");
    //printf("%d %d\n",i, buffer[i]);
    if(df==1){
      //fprintf(stderr,"%d %f\n",buffer[i],buffer_fl[i-data_start_i]);
      //buffer[i]=(char) (buffer_fl[i-data_start_i]*128-128);
      buffer[i]=(unsigned char) buffer_fl[i-data_start_i];
      //fprintf(stderr,"%d %f\n",(int) buffer[i], buffer_fl[i-data_start_i]);
    }
    if(memcmp(buffer+i,"data",4)==0){
      fprintf(stderr,"%d\n", ((int) buffer[i+4])+256*((int) buffer[i+5])+256*256*((int) buffer[i+6])+256*256*256*((int) buffer[i+7]));
      //fprintf(stderr,"%d %d %d %d\n", ((int) buffer[i+4]),((int) buffer[i+5]),((int) buffer[i+6]),((int) buffer[i+7]));
      //fprintf(stderr,"%d\n",buffer[i+4],buffer[i+5]+16*16*buffer[i+6]+16*16*16*buffer[i+7]);
      df=1;
      data_start_i=i+8;
      i=data_start_i;
    }
  }
  fwrite(buffer,sizeof(char),buffer_size,stdout);
  return 0;
}
