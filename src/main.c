#include <stdio.h>
#include <stdlib.h>
#include "ALU.h"  /* Include the header here, to obtain the function declaration */
#include "controlunit.h"

int main(void)
{
    int y = add(3, 10);  /* Use the function here */
    printf("%d\n", y);
	ControlUnit unit = controlUnitConst(0, 0x5000);
	unit.memory.write(&(unit.memory), 0, 0x31003000);
	unit.memory.write(&(unit.memory), 1, 0x32003001);
	unit.memory.write(&(unit.memory), 2, 0x03100002);
	unit.memory.write(&(unit.memory), 0x3000, 0x00000008);
	unit.memory.write(&(unit.memory), 0x3001, 0x00000009);
	printf("Starting Instructions\n");
	unit.nextInst(&unit);
	unit.nextInst(&unit);
	unit.nextInst(&unit);
    return 0;
}