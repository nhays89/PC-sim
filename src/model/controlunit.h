
#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H

#include "memory.h"
#include "ALU.h"
#include "regfile.h"

typedef struct ControlUnit{
	int instructionRegister;
	int programCounter;
	Memory *memory;
	ALU *alu;
	RegFile *regFile;
	int (*nextInst)(struct ControlUnit*);
} ControlUnit;

ControlUnit *controlUnitConst(int pc, int memSize);

void controlUnitDeconst(ControlUnit *unit);

#endif /* CONTROLUNIT_H */