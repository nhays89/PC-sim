#ifndef ALU_H_   /* Include guard */
#define ALU_H_

typedef struct {
	int (*add)(int, int);
	int (*nand)(int, int);
	int (*isEqual)(int, int);
	int (*increment)(int);
} ALU;

int add(int x, int y);  /* An example function declaration */

int nand(int x, int y);

int isEqual(int x, int y);

int increment(int x);

ALU *aluConst();

void aluDeconst(ALU *alu);

#endif // ALU_H_