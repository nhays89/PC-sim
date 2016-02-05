#include <stdlib.h>
#include <string.h>
#include "regfile.h"

int read_Register(int index, RegFile *regFile) {
	return regFile->registers[index];
}

void write_Register(int index, int value, RegFile *regFile) {
	regFile->registers[index] = value;
}

RegFile regFileConst() {
	RegFile regs;
	memset(regs.registers, 0, sizeof(int)*16);
	regs.read = read_Register;
	regs.write = write_Register;
	return regs;
}