#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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
    //for debugging
    //printf("imm value is: %d\n", (int) immInt);

    if (immInt < 0xFFFFF)
        *instr = *instr | (int) immInt;
}

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

int isDirective(char *inst)
{
    if (inst[0] == '.')
        return 1;
    else 
        return 0;
}

int createMachineCode(char *inst)
{
    int result = 0;

    char copy[128]; 
    char *token;
    char *dir;
    char *regX;
    char *regY;
    char *regZ;
    char *imm;
    
    strcpy(copy, inst);
	// token = strtok(copy, " $");
    
    //deal with directives
    if (isDirective(copy) == 1)
    {
        dir = strtok(copy, " \0;");
        //handle an .END directive as a halt
        if (strcmp(dir, ".END") == 0)
        {
           return createMachineCode("halt"); 
        } 
        //write the int after .orig
        else if (strcmp(dir, ".ORIG") == 0)
        {
            dir = strtok(NULL, " \0;");
            return atoi(dir);
        }
    }

    token = strtok(copy, " ") ;
    result = getOpcode(token);

    //parse and create the rest of the instruction based on its type
    switch(getInstructionType(result))
    {
        case 'R':
            regX = strtok(NULL, ",");
            setRegister('x', &result, regX);

            regY = strtok(NULL, " ,");
            setRegister('y', &result, regY);

            regZ = strtok(NULL, " \0;");
            setRegister('z', &result, regZ);
            break;
        case 'I':
            //sw and lw look at little different
            //TODO fix how these two work
            if (strcmp(token, "sw") == 0
                || strcmp(token, "lw") == 0)
            {
                regX = strtok(NULL, ",");
                setRegister('x', &result, regX);

                imm = strtok(NULL, "(");
                setImmediate(&result, imm);

                regY = strtok(NULL, ")");
                setRegister('y', &result, regY);

                break;
            }
            regX = strtok(NULL, ",");
            setRegister('x', &result, regX);

            regY = strtok(NULL, " ,");
            setRegister('y', &result, regY);

            imm = strtok(NULL, " \0");

            setImmediate(&result, imm);
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




