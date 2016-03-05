#include <stdio.h>
#include "memory.h"
#include <stdlib.h>

Memory *createMemory(int size)
{
    Memory *result = calloc(1, sizeof(Memory));
    result->words =  calloc(1, sizeof(int) * size);
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
	self->lastModified = addr;
}

int read(Memory *self, int addr)
{
    return self->words[addr];
}

