#include <stdlib.h>
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

void aluDeconst(ALU *alu) {
	free(alu);
}

ALU *aluConst() {
	ALU *alu = calloc(1, sizeof(ALU));
	alu->add = add;
	alu->nand = nand;
	alu->isEqual = isEqual;
	alu->increment = increment;
	return alu;
}