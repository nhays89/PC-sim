#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

char getInstructionType(int instr);

int createMachineCode(char *inst);

void setImmediate(int *instr, char *imm);

void setRegister(char regToSet, int *instr, char *regName);

int getOpcode(char *instr);

int getRegisterNum(char *reg);

#endif
