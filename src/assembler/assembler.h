/**
 * @file assembler.h
 * @author nebp
 * @date 26 Feb 2016
 * @brief File containing example of doxygen usage for quick reference.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.TODO document all this shit
 */


#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

/**
 * @brief Use brief, otherwise the index won't have a brief explanation.
 *
 * Detailed explanation.
 */
char getInstructionType(int instr);

int isDirective(char *inst);

int createMachineCode(char *inst);

void setImmediate(int *instr, char *imm);

void setRegister(char regToSet, int *instr, char *regName);

int getOpcode(char *instr);

int getRegisterNum(char *reg);

#endif
