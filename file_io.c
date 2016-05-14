#include "file_io.h"

#define buffer_size 1024*200

void read_data_old(struct Header *h, double** data_buffer){
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
    }
    if(i==35){
      /* Get bits per sample */
      h->bps=((int) head[i-1])+256*((int) head[i]);
    }
  }
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

void read_data_f_old(FILE* stream, struct Header *h, double** data_buffer){
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

int read_data_f(FILE* stream, struct Header *h, double** data_buffer){
  unsigned char head[200];
  int i;
  int data_index=-100;
  unsigned char uc;
  char sc;
  int weight;

  /* Get header contents up to and including 'data' */
  i=0;
  while(fscanf(stream, "%c",head+i) != EOF){
    switch(i){
    case 3:
      memcpy(h->ChunkID,head+0,4);
      h->ChunkID[4]='\0';
      if(strcmp(h->ChunkID,"RIFF")!=0){
	fprintf(stderr,"Read error: Not a valid WAV file\n");
	return 1;
      }
      break;
    case 7:
      h->ChunkSize=*(head+4)+*(head+5)*256+*(head+6)*256*256+*(head+7)*256*256;
      break;
    case 11:
      memcpy(h->Format,head+8,4);
      h->Format[4]='\0';
      break;
    case 15:
      memcpy(h->Subchunk1ID,head+12,4);
      h->Subchunk1ID[4]='\0';
      break;
    case 19:
      h->Subchunk1Size=head[16]+head[17]*256+head[18]*256*256+head[19]*256*256*256;
      break;
    case 21:
      h->AudioFormat=head[20]+head[21]*256;
      break;
    case 23:
      h->NumChannels=head[22]+head[23]*256;
      break;
    case 27:
      h->SampleRate=head[24]+256*head[25]+256*256*head[26]+256*256*256*head[27];
      break;
    case 31:
      h->ByteRate=head[28]+256*head[29]+256*256*head[30]+256*256*256*head[31];
      break;
    case 33:
      h->BlockAlign=head[32]+256*head[33];
      break;
    case 35:
      h->BitsPerSample=head[34]+256*head[35];
      break;
    default:
      break;
    }
    if(i>4 && memcmp(head+i-3,"data",4)==0){
      memcpy(h->Subchunk2ID,head+i-3,4);
      h->Subchunk2ID[4]='\0';
      data_index=i-3;
    }
    if(i==data_index+7){
      h->Subchunk2Size=head[i-3]+256*head[i-2]+256*256*head[i-1]+256*256*256*head[i];
      break;
    }
    ++i;
  }

  if(i<40){
    fprintf(stderr,"Read error: Not a valid WAV file\n");
    return 2;
  }

  *data_buffer=(double*) malloc(((h->Subchunk2Size*8)/h->BitsPerSample)*sizeof(double));
  
  i=0;
  weight=1;
  if(h->BitsPerSample==8){
    while((fscanf(stream, "%c",&uc) != EOF)){
      if(i%(h->BitsPerSample/8)==0){
	weight=1;
	(*data_buffer)[i/(h->BitsPerSample/8)]=0;
      }
      (*data_buffer)[i/(h->BitsPerSample/8)]+=(double) (weight*uc);
      weight*=256;
      ++i;
    }
  }
  else{
    while((fscanf(stream, "%c",&sc) != EOF)){
      if(i%(h->BitsPerSample/8)==0){
	weight=1;
	(*data_buffer)[i/(h->BitsPerSample/8)]=0;
      }
      (*data_buffer)[i/(h->BitsPerSample/8)]+=(double) (weight*sc);
      weight*=256;
      ++i;
    }
  }
  return 0;
}

int read_data(struct Header *h, double** data_buffer){
  int err=read_data_f(stdin,h,data_buffer);
  return err;
}

void header_to_string(struct Header *h, unsigned char* string){
  memcpy(string,&(h->ChunkID),4);
  memcpy(string+4,&(h->ChunkSize),4);
  memcpy(string+8,&(h->Format),4);

  memcpy(string+12,&(h->Subchunk1ID),4);
  memcpy(string+16,&(h->Subchunk1Size),4);
  memcpy(string+20,&(h->AudioFormat),2);
  memcpy(string+22,&(h->NumChannels),2);
  memcpy(string+24,&(h->SampleRate),4);
  memcpy(string+28,&(h->ByteRate),4);
  memcpy(string+32,&(h->BlockAlign),2);
  memcpy(string+34,&(h->BitsPerSample),2);
  
  memcpy(string+36,&(h->Subchunk2ID),4);
  memcpy(string+40,&(h->Subchunk2Size),4);
  
  
  /*printf("%d %d %d %d\n",string[0],string[1],string[2],string[3]);*/
  return;
}
/*
void write_data(struct Header *h, double* data_buffer){
  int i;
  for(i=0; i<h->head_size; ++i){
    printf("%c",h->string[i]);
  }
  if(h->bps==8)
  for(i=0; i<h->data_size; ++i){
    printf("%c\n",(unsigned char) data_buffer[i]);
  }
  if(h->bps==16)
    for(i=0; i<h->data_size; i+=1){
      printf("%c",(char) (((int) data_buffer[i])%256));
      printf("%c",(char) ((int) data_buffer[i]/256));
    }
  return;
}
*/

void write_data(struct Header *h, double* data_buffer){
  int i;
  unsigned char header_string[200];
  int header_size;
  int data_size=((h->Subchunk2Size)*8)/(h->BitsPerSample);
  /*
  for(i=0; i<h->head_size; ++i){
    printf("%c",h->string[i]);
  }
  */
  header_size=12+(8+h->Subchunk1Size)+8;
  header_to_string(h,header_string);
  for(i=0; i<header_size; ++i){
    printf("%c",header_string[i]);
  }
  if(h->BitsPerSample==8)
  for(i=0; i<data_size; ++i){
    printf("%c",(unsigned char) data_buffer[i]);
  }
  if(h->BitsPerSample==16)
    for(i=0; i<data_size; i+=1){
      printf("%c",(char) (((int) data_buffer[i])%256));
      printf("%c",(char) ((int) data_buffer[i]/256));
    }
  return;
}
