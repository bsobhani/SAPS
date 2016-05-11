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
};

void read_data(struct Header *h, double** data_buffer);
void write_data(struct Header *h, double* data_buffer);

#endif
