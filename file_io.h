#ifndef FILE_IO_H
#define FILE_IO_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "complex.h"

struct Header{
  int head_size;
  int data_size;
  char* string;
  unsigned int sample_rate;
  unsigned int bps;

  char ChunkID[5];
  int ChunkSize;
  char Format[5];

  char Subchunk1ID[5];
  int Subchunk1Size;
  int AudioFormat;
  int NumChannels;
  int SampleRate;
  int ByteRate;
  int BlockAlign;
  int BitsPerSample;

  char Subchunk2ID[5];
  int Subchunk2Size;
};

int read_data(struct Header *h, double** data_buffer);
int read_data_f(FILE* stream, struct Header *h, double** data_buffer);
void write_data(struct Header *h, double* data_buffer);
void header_to_string(struct Header *h, unsigned char* string);

#endif
