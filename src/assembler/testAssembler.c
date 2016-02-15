#include <stdio.h>
#include <string.h>
#include "assembler.h"

//http://stackoverflow.com/questions/1024389/print-an-int-in-binary-representation-using-c
char *int2bin(int a, char *buffer, int buf_size) {
    buffer += (buf_size - 1);

    for (int i = 31; i >= 0; i--) {
        *buffer-- = (a & 1) + '0';

        a >>= 1;
        }
        //annoying. added this
        buffer[33] = '\0';
        return buffer;
}

int main()
{
    char binString[33];
    char testInst[] = "add $s0, $at, $a0";
    int binTestInst = createMachineCode(testInst);
    int2bin(binTestInst, binString, 32);

    printf("%s\n", binString); 
    /*
    char testInst[] = "nand $at,$fp,";
    int instr = 0;// getOpcode(testInst);
    char binString[33];
    char *token;
    char *regX;
    char *regY;

    token = strtok(testInst, " ");
    regX = strtok(NULL, ",");
    regY = strtok(NULL, ",");
    printf("%s is register number:%d\n", regX, getRegisterNum(regX));
    //testing setting registers
     
    instr = getOpcode(token);
    setRegister('x', &instr, regX);
    setRegister('Y', &instr, regY);
    
    int2bin(instr, binString, 32);

    printf ("%s\n", binString);
    printf("instruction is type %c \n", getInstructionType(instr));
    */
    return 0;
}
