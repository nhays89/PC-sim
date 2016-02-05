
#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H

#include "memory.h"
#include "ALU.h"
#include "regfile.h"

typedef struct ControlUnit{
	int instructionRegister;
	int programCounter;
	Memory memory;
	ALU alu;
	RegFile regFile;
	void (*nextInst)(struct ControlUnit*);
} ControlUnit;

ControlUnit controlUnitConst(int, int);


#endif /* CONTROLUNIT_H */