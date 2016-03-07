#ifndef MEMORY_H_
#define MEMORY_H_

typedef struct Memory 
{
    int *words;
    void (*destroyMemory)(struct Memory *self);
    void (*write)(struct Memory *self,int addr, int value);
    int (*read)(struct Memory *self, int addr);
	int size;
	int lastModified;
} Memory;

Memory *createMemory(int size);

void destroyMemory(Memory *self);

void write(Memory *self, int addr, int value);

int read(Memory *self, int addr);

#endif
