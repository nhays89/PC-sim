
#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H

typedef struct ControlUnit{
	int instructionRegister;
	int programCounter;
	int *memory;
	void (*nextInst)(struct ControlUnit*);
} ControlUnit;

ControlUnit controlUnitConst(int, int *);


#endif /* CONTROLUNIT_H */