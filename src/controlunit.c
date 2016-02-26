#include <stdio.h>
#include "controlunit.h"
#include "memory.h"
#include "ALU.h"
#include "regfile.h"

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
	int instruction = unit->memory.read(&(unit->memory), unit->programCounter);
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

int execute(InstructionSet instruction, ControlUnit *unit) {
	int op1;
	int op2;
	int immmediate;
	int address;
	int result;
	
	switch(instruction.opcode) {
		case 0 : // add
			op1 = unit->regFile.read(instruction.regY, &(unit->regFile));		//get regY
			op2 = unit->regFile.read(instruction.regZ, &(unit->regFile));		//get regZ
			result = unit->alu.add(op1, op2);									//a = regY + regZ
			unit->regFile.write(instruction.regX, result, &(unit->regFile));	//regX = a
			return 0;
			break;
		case 1 : //nand
			op1 = unit->regFile.read(instruction.regY, &(unit->regFile));		//get regY
			op2 = unit->regFile.read(instruction.regZ, &(unit->regFile));		//get regZ
			result = unit->alu.nand(op1, op2);									//a = !(regY & regZ)
			unit->regFile.write(instruction.regX, result, &(unit->regFile));	//regX = a
			return 0;
			break;
		case 2 : //addi
			op1 = unit->regFile.read(instruction.regY, &(unit->regFile));		//get regY
			op2 = instruction.immediate;										//get immediate
			result = unit->alu.add(op1, op2);									//a = regY + immediate
			unit->regFile.write(instruction.regX, result, &(unit->regFile));	//regX = a
			return 0;
			break;
		case 3 : //lw
			op1 = unit->regFile.read(instruction.regY, &(unit->regFile));		//get regY
			op2 = instruction.immediate;										//get immediate
			address = unit->alu.add(op1, op2);									//address = regY + immediate
			result = unit->memory.read(&(unit->memory), address);				//a = MEM[address]
			unit->regFile.write(instruction.regX, result, &(unit->regFile));	//regX = a
			return 0;
			break;
		case 4 : //sw
			op1 = unit->regFile.read(instruction.regX, &(unit->regFile));		//get regX
			op2 = unit->regFile.read(instruction.regY, &(unit->regFile));		//get regY
			immmediate = instruction.immediate;									//get immediate
			address = unit->alu.add(immmediate, op2);							//address = immediate + regY
			unit->memory.write(&(unit->memory), immmediate + op2, op1);			//Mem[address] = regX
			return 0;
			break;
		case 5 : //beq
			op1 = unit->regFile.read(instruction.regX, &(unit->regFile));		//get regX
			op2 = unit->regFile.read(instruction.regY, &(unit->regFile));		//get regY
			immmediate = instruction.immediate;									//get immediate
			if (unit->alu.isEqual(op1, op2)) {									//if regX = regY
				unit->programCounter += immmediate;								//PC = PC + immediate
			}
			return 0;
			break;
		case 6 : //jalr
			op1 = unit->regFile.read(instruction.regX, &(unit->regFile));						//Get val of regX
			unit->regFile.write(instruction.regY, unit->programCounter, &(unit->regFile));		//Save PC to regY
			unit->programCounter = op1;															//Set PC to regY
			return 0;
			break;
		case 7 : //halt
			return 1;
			break;
		default :
			printf("NOP\n");
			return 0;
	};
}

int nextInst(ControlUnit *unit) {
	fetch(unit);
	InstructionSet instructions = decode(unit);
	return execute(instructions, unit);
}

ControlUnit controlUnitConst(int pc, int memSize) {
	ControlUnit unit;
	unit.programCounter = pc;
	Memory* mem = createMemory(memSize);
	unit.memory = *mem;
	unit.alu = aluConst();
	unit.regFile = regFileConst();
	unit.nextInst = nextInst;
	return unit;
}



