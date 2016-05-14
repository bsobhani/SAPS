#include "stdio.h"
#include "stdlib.h"
#include "file_io.h"

void print_header(struct Header *h){
  printf("ChunkID: %s\n",h->ChunkID);
  printf("ChunkSize: %d\n",h->ChunkSize);
  printf("Format: %s\n",h->Format);

  printf("Subchunk1ID: %s\n",h->Subchunk1ID);
  printf("Subchunk1Size: %d\n",h->Subchunk1Size);
  printf("AudioFormat: %d\n",h->AudioFormat);
  printf("NumChannels: %d\n",h->NumChannels);
  printf("SampleRate: %d\n",h->SampleRate);
  printf("ByteRate: %d\n",h->ByteRate);
  printf("BitsPerSample: %d\n",h->BitsPerSample);

  printf("Subchunk2ID: %s\n",h->Subchunk2ID);
  printf("Subchunk2Size: %d\n",h->Subchunk2Size);
  
}

int main(int argc, char** argv){
  FILE* fp;
  struct Header h;
  double* data_buffer;
  unsigned char header_string[100];
  
  fp=fopen(argv[1],"r");
  /* Read data from stdin (see file_io.c) */
  if(read_data_f(fp,&h,&data_buffer)!=0){
    fprintf(stderr,"Read error: Not a valid WAV file\n");
    return 1;
  }
  printf("Header data for %s:\n",argv[1]);
  print_header(&h);

  header_to_string(&h, header_string);

  return 0;
}
