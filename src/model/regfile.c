#include <stdlib.h>
#include <string.h>
#include "regfile.h"

int read_Register(int regNum, RegFile *regFile) {
	return regFile->registers[regNum];
}

void write_Register(int regNum, int value, RegFile *regFile) {
	regFile->registers[regNum] = value;
	regFile->lastModified = regNum;
}

void regFileDeconst(RegFile *file) {
	free(file);
}

RegFile *regFileConst() {
	RegFile *regs = calloc(1, sizeof(RegFile));
	memset(regs->registers, 0, sizeof(int)*16);
	regs->read = read_Register;
	regs->write = write_Register;
	regs->lastModified = 0;
	return regs;
}