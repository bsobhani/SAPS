#include "file_io.h"

#define buffer_size 1024*200

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
