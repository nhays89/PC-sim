#include <stdio.h>
#include "controlunit.h"

typedef struct {
	int opcode;
	int regX;
	int regY;
	int regZ;
	int immediate;
} InstructionSet;

int getOpcode(int instruction) {
	int opcodeMask = 0xf0000000;
	int resultMask = 0x0000000f;
	int opcode = instruction & opcodeMask; 		//Get bits 31 to 28
	opcode = opcode >> 28;						//Shift four bits to beginning of byte
	opcode = opcode & resultMask;				//Remove all bits except last four this is needed if the opcode is negative
	return opcode;
}

int getRegX(int instruction) {
	//Get bits 27 to 24
	int regXMask = 0x0f000000;
	int regX = (instruction & regXMask) >> 24;
	return regX;
}

int getRegY(int instruction) {
	//Get bits 23 to 20
	int regYMask = 0x00f00000;
	int regY = (instruction & regYMask) >> 20;
	return regY;
}

int getRegZ(int instruction) {
	//Get bits 3 to 0
	int regZMask = 0x0000000f;
	int regZ = instruction & regZMask;
	return regZ;
}

int getImmediate(int instruction) {
	//Get bits 19 to 0
	int immediateMask = 0x000fffff;
	int immediate = instruction & immediateMask;
	if (immediate & 0x00080000) {				//If immediate value is supposed to be negative
		immediate = immediate | 0xfff00000;		//Fill preceding bits with value of 1
	}
	return immediate;
}

void fetch(ControlUnit *unit) {
	int instruction = unit->memory[unit->programCounter];
	unit->instructionRegister = instruction;
	unit->programCounter++;
}

InstructionSet decode(ControlUnit *unit) {
	int opcode = getOpcode(unit->instructionRegister);
	printf("Opcode: %d\n", opcode);
	
	int regX = getRegX(unit->instructionRegister);
	printf("RegX: %d\n", regX);
	
	int regY = getRegY(unit->instructionRegister);
	printf("RegY: %d\n", regY);
	
	int regZ = getRegZ(unit->instructionRegister);
	printf("RegZ: %d\n", regZ);
	
	int immediate = getImmediate(unit->instructionRegister);
	printf("Immediate: %d\n", immediate);
	
	InstructionSet set = {opcode, regX, regY, regZ, immediate};
	return set;
}

void execute(InstructionSet instruction) {
	switch(instruction.opcode) {
		case 0 :
			printf("Add\n");
			break;
		case 1 :
			printf("nand\n");
			break;
		case 2 :
			printf("addi\n");
			break;
		case 3 :
			printf("lw\n");
			break;
		case 4 :
			printf("sw\n");
			break;
		case 5 :
			printf("beq\n");
			break;
		case 6 :
			printf("jalr\n");
			break;
		case 7 :
			printf("halt\n");
			break;
		default :
			printf("NOP\n");
	};
}

void nextInst(ControlUnit *unit) {
	fetch(unit);
	InstructionSet instructions = decode(unit);
	execute(instructions);
}

ControlUnit controlUnitConst(int pc, int *mem) {
	ControlUnit unit;
	unit.programCounter = pc;
	unit.memory = mem;
	unit.nextInst = nextInst;
	return unit;
}



