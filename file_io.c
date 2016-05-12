#include "file_io.h"

#define buffer_size 1024*200

void read_data(struct Header *h, double** data_buffer){
  char head[buffer_size];
  int i;
  int data_size;
  unsigned char uc;
  char sc,sc2;
  
  data_size=-1;
  for(i=0; data_size==-1; ++i){
    scanf("%c",head+i);
    if(i>7 && memcmp(head+i-7,"data",4)==0){
      data_size=((int) head[i-3])+256*((int) head[i-2])+256*256*((int) head[i-1])+256*256*256*((int) head[i]);
      h->data_size=data_size;
    }
    if(i==27){
      h->sample_rate=((unsigned char) head[i-3])+256*((unsigned char) head[i-2])+256*256*((unsigned char) head[i-1])+256*256*256*((unsigned char) head[i]);
      /*fprintf(stderr,"%d %d %d %d\n",(unsigned int) head[i-3],(unsigned int) head[i-2],(int) head[i-1],(int) head[i]);*/
      /*fprintf(stderr,"%d\n",((int) head[i-3])+256*((int) head[i-2])+256*256*((int) head[i-1])+256*256*256*((int) head[i]));*/ 
    }
    if(i==35){
      /* Get bits per sample */
      h->bps=((int) head[i-1])+256*((int) head[i]);
      /*fprintf(stderr,"%d\n",((int) head[i-1])+256*((int) head[i])); */
    }
  }
  /*if(h->sample_rate!=11025 || h->bps!=8)
    fprintf(stderr,"Error: Please use a 11025Hz 8 bit per sample wave file\n");
  */
  h->head_size=i;
  head[h->head_size]='\0';

  h->string=(char*) malloc(h->head_size*sizeof(char));  
  for(i=0; i<h->head_size; ++i){
    h->string[i]=head[i];
  }
  *data_buffer=(double*) malloc(data_size*sizeof(double));

  if(h->bps==8){
    for(i=h->head_size; i<h->head_size+data_size; ++i){
      scanf("%c",&uc);
      (*data_buffer)[i-h->head_size]=(double) uc;
    }
  }
  else if(h->bps==16){
    for(i=h->head_size+1; i<h->head_size+data_size; i+=2){
      scanf("%c",&sc);
      scanf("%c",&sc2);
      (*data_buffer)[i-h->head_size]=((double) sc)+256*((double) sc2);
    }
  }
  return;
}

void read_data_f(FILE* stream, struct Header *h, double** data_buffer){
  char head[buffer_size];
  int i;
  int data_size;
  unsigned char uc;
  char sc,sc2;
  
  data_size=-1;
  for(i=0; data_size==-1; ++i){
    fscanf(stream,"%c",head+i);
    if(i>7 && memcmp(head+i-7,"data",4)==0){
      data_size=((int) head[i-3])+256*((int) head[i-2])+256*256*((int) head[i-1])+256*256*256*((int) head[i]);
      h->data_size=data_size;
    }
    if(i==27){
      h->sample_rate=((unsigned char) head[i-3])+256*((unsigned char) head[i-2])+256*256*((unsigned char) head[i-1])+256*256*256*((unsigned char) head[i]);
      /*fprintf(stderr,"%d %d %d %d\n",(unsigned int) head[i-3],(unsigned int) head[i-2],(int) head[i-1],(int) head[i]);*/
      /*fprintf(stderr,"%d\n",((int) head[i-3])+256*((int) head[i-2])+256*256*((int) head[i-1])+256*256*256*((int) head[i]));*/ 
    }
    if(i==35){
      /* Get bits per sample */
      h->bps=((int) head[i-1])+256*((int) head[i]);
      /*fprintf(stderr,"%d\n",((int) head[i-1])+256*((int) head[i])); */
    }
  }
  /*if(h->sample_rate!=11025 || h->bps!=8)
    fprintf(stderr,"Error: Please use a 11025Hz 8 bit per sample wave file\n");
  */
  h->head_size=i;
  head[h->head_size]='\0';

  h->string=(char*) malloc(h->head_size*sizeof(char));  
  for(i=0; i<h->head_size; ++i){
    h->string[i]=head[i];
  }
  *data_buffer=(double*) malloc(data_size*sizeof(double));

  if(h->bps==8){
    for(i=h->head_size; i<h->head_size+data_size; ++i){
      fscanf(stream,"%c",&uc);
      (*data_buffer)[i-h->head_size]=(double) uc;
    }
  }
  else if(h->bps==16){
    for(i=h->head_size+1; i<h->head_size+data_size; i+=2){
      fscanf(stream,"%c",&sc);
      fscanf(stream,"%c",&sc2);
      (*data_buffer)[i-h->head_size]=((double) sc)+256*((double) sc2);
    }
  }
  return;
}

void write_data(struct Header *h, double* data_buffer){
  int i;
  for(i=0; i<h->head_size; ++i){
    printf("%c",h->string[i]);
  }
  if(h->bps==8)
  for(i=0; i<h->data_size; ++i){
    printf("%c",(unsigned char) data_buffer[i]);
  }
  if(h->bps==16)
    for(i=0; i<h->data_size; i+=1){
      printf("%c",(char) (((int) data_buffer[i])%256));
      printf("%c",(char) ((int) data_buffer[i]/256));
    }
  return;
}
