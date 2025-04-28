#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char byte;
typedef unsigned short int word;
typedef word address;

#define MEMSIZE 64*1024
extern byte mem[MEMSIZE];
extern word reg[8];

void b_write(address adr, byte val);
byte b_read(address adr);
void w_write(address adr, word val);
word w_read(address adr);
void mem_dump(address adr, int size);
void test_mem();
void load_data(FILE * fin);
void load_file(const char * filename);

#pragma once
