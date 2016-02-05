#include <stdio.h>
#include <stdlib.h>
#include "controlunit.h"

int main() {
	int memory[] = {0x4ab80005, 0x1ab7000f};
	int programStart = 0;
	ControlUnit control = controlUnitConst(programStart, memory);
	control.nextInst(&control);
	printf("\n");
	control.nextInst(&control);
	return 0;
}