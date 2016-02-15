#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* Function: getRegisterNum
 *-------------------
 *  returns an int containg the dec number (0-15) or a register
 *
 *  *reg: the ascii name of a register
 */
int getRegisterNum(char *reg)
{
    int result = 0;
    
    if (strcmp(reg, "$zero") == 0)
        result = 0;
    else if (strcmp(reg, "$at") == 0)
        result = 1;
    else if (strcmp(reg, "$v0") == 0)
        result = 2;
    else if (strcmp(reg, "$a0") == 0)
        result = 3;
    else if (strcmp(reg, "$a1") == 0)
        result = 4;
    else if (strcmp(reg, "$a2") == 0)
        result = 5;
    else if (strcmp(reg, "$t0") == 0)
        result = 6;
    else if (strcmp(reg, "$t1") == 0)
        result = 7;
    else if (strcmp(reg, "$t2") == 0)
        result = 8;
    else if (strcmp(reg, "$s0") == 0)
        result = 9;
    else if (strcmp(reg, "$s1") == 0)
        result = 10;
    else if (strcmp(reg, "$s2") == 0)
        result = 11;
    else if (strcmp(reg, "$k0") == 0)
        result = 12;
    else if (strcmp(reg, "$sp") == 0)
        result = 13;
    else if (strcmp(reg, "$fp") == 0)
        result = 14;
    else if (strcmp(reg, "$ra") == 0)
        result = 15;
    else 
        result = -1;

    return result;
}

void setImmediate(int *instr, char *imm)
{
    char *ptr;
    long immInt;

    immInt = strtol(imm, &ptr, 10);
    printf("imm value is: %d\n", (int) immInt);

    if (something < 0xFFFFF)
        *instr = *instr | (int) immInt;
}

/* Function: setRegister
 *-------------------
 *  fills in the binary operand of a register number in an instruction
 *
 *  *instr: pointer to the binary instruction we are creating
 *  regToSet: the register operand to set (x,y or z) (ex: add X,Y,Z or addi X,Y, 42) 
 *  *regName: the ascii name of the register to be used as an operand (ex: $a0)
 */
void setRegister(char regToSet, int *instr, char *regName)
{
    int shift = 0;
    int regNum = 0;

    switch (regToSet)
    {
        case 'X':
        case 'x':
            shift = 24;
            break;
        case 'y':
        case 'Y':
            shift = 20;
            break;
        case 'z':
        case 'Z':
        //no shift needed to set Reg Z
            break;
        default :
            printf("Not a valid register to set\n");
    }

    //get the register number and shifts into place
    if (getRegisterNum(regName) == -1) //TODO handle this better
        printf("error reading register name!\n");
    regNum = getRegisterNum(regName) << shift;
    *instr = *instr |  regNum;
}

/* Function: getOpcode
 *-------------------
 *  returns an int containg the opcode for an instruction (opcode in the most sig 4 bits)
 *
 *  *instr: the text of an instruction (line from .asm file)
 */
int getOpcode(char *instr)
{
    int result = 0;

    //R-type
    if (strcmp(instr, "add") == 0) //Opcode: 0000
        result = 0;
    else if (strcmp(instr, "nand") == 0)//Opcode: 0001
        result = 0x10000000; 
    //I-Type
    else if (strcmp(instr, "addi") == 0)//Opcode: 0010
        result = 0x20000000; 
    else if (strcmp(instr, "lw") == 0)//Opcode: 0011
        result = 0x30000000; 
    else if (strcmp(instr, "sw") == 0)//Opcode: 0100
        result = 0x40000000; 
    else if (strcmp(instr, "beq") == 0)//Opcode: 0101
        result = 0x50000000; 
    //J-Type
    else if (strcmp(instr, "jalr") == 0)//Opcode: 0110
        result = 0x60000000; 
    //O-Type
    else if (strcmp(instr, "halt") == 0)//Opcode: 0111
        result = 0x70000000; 
    else
        result = -1;

    return result;
}

/* Function: getInstructionType
 *-------------------
 *  returns a char representing an instruction type (R, I, J or O)
 *  useful for creating instruction binary
 *
 *  instr: a binary instruction 
 */
char getInstructionType(int instr)
{
    char result;
    int opcode = instr >> 28;

    switch(opcode)
    {
        case 0 ... 1:
            result = 'R';
            break;
        case 2 ... 5:
            result = 'I';
            break;
        case 6:
            result = 'J';
            break;
        case 7:
            result = 'O';
            break;
        default:
            printf("this opcode has no type. something went wrong");
    }

    return result;
}

int createMachineCode(char *inst)
{
    int result = 0;

    char *token;
    char *regX;
    char *regY;
    char *regZ;
    
    token = strtok(inst, " ");
    result = getOpcode(token);
    
    //pasrse and create the rest of the instruction based on its type
    switch(getInstructionType(result))
    {
        case 'R':
            regX = strtok(NULL, ",");
            setRegister('x', &result, regX);

            regY = strtok(NULL, " ,");
            setRegister('y', &result, regY);

            regZ = strtok(NULL, " \0");
            setRegister('z', &result, regZ);
            break;
        case 'I':
            //TODO write function to translate imm val to binary
            break;
        case 'J':
            regX = strtok(NULL, ",");
            setRegister('x', &result, regX);

            regY = strtok(NULL, " \0");
            setRegister('y', &result, regY);
            break;
        case 'O':
            // O-type all we need is an opcode
            break;
        default:
            printf("error");
    }

    return result;
}




