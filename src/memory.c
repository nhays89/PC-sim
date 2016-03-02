#include <stdio.h>
#include "memory.h"
#include <stdlib.h>

Memory *createMemory(int size)
{
    Memory *result = malloc(sizeof(Memory));
    result->words =  malloc(sizeof(int) * size);
    result->write = write;
    result->destroyMemory = destroyMemory;
    result->read = read;
	result->size = size;
    return result;
} 

void destroyMemory(Memory *self)
{
    free(self->words);
    free(self);
}

void write(Memory *self, int addr, int value)
{
    self->words[addr] = value;
}

int read(Memory *self, int addr)
{
    return self->words[addr];
}

