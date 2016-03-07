/**
 * @file assembler.h
 * @author nnebp
 *
 * @brief Assembles strings of assembly code to integers representing
 * binary machine code.
 */


#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

/**
 * @brief Determines the type of instruction in an instruction. 
 * based on the binary of the opcode
 *
 * @param instr a binary instruction.
 * @return Character representing the instruction type (R,I,J,O).
 */
char getInstructionType(int instr);

/**
 * @brief Sees if a line is a assembler directive by checking if the 
 * first character in the line is a '.'
 *
 * @param inst string representing an instruction
 * @return 0 if the string is not a directive 1 if it is.
 */
int isDirective(char *inst);


/**
 * @brief takes a string representing an instruction and returns an integer
 * with the correct binary representation of the machine code of the instuction string.
 *
 * @param inst string instruction.
 * @return machine code representation of instruction.
 */
int createMachineCode(char *inst);


/** 
 * @brief Adds an immidate value to an I instruction
 *
 * @param instr a binary instruction
 * @param imm string with base 10 immidate value
 */
void setImmediate(int *instr, char *imm);


/** @brief fills in the binary operand of a register number in an instruction
 *
 * @param instr pointer to the binary instruction we are creating
 * @param regToSet the register operand to set (x,y or z) (ex: add X,Y,Z or addi X,Y, 42) 
 * @param regName the ascii name of the register to be used as an operand (ex: $a0)
 */
void setRegister(char regToSet, int *instr, char *regName);


/**
 * @brief  returns an int containg the opcode for an instruction
 * (opcode in the most sig 4 bits)
 *
 * @param instr the text of an instruction (line from .asm file)
 * @return binary representing an instruction with the opcode bits set.
 */
int getOpcode(char *instr);


/** 
 * @brief Gets the base 10 number of a register based on it's string representation.
 *
 * @param reg the ascii name of a register
 * @return an int containg the dec number (0-15) or a register
 */
int getRegisterNum(char *reg);

#endif
