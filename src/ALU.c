#include "ALU.h"  /* Include the header (not strictly necessary here) */

int add(int x, int y)    /* Function definition */
{
    return x + y;
}

int nand(int x, int y) {
	return ~(x & y);
}

int isEqual(int x, int y) {
	return x == y;
}

int increment(int x) {
	return ++x;
}

ALU aluConst() {
	ALU alu;
	alu.add = add;
	alu.nand = nand;
	alu.isEqual = isEqual;
	alu.increment = increment;
	return alu;
}