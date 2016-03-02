#ifndef REG_FILE_H_
#define REG_FILE_H_

typedef struct RegFile{
	int registers[16];
	int (*read)(int, struct RegFile*);
	void (*write)(int, int, struct RegFile*);
} RegFile;

RegFile *regFileConst();
void regFileDeconst(RegFile *file);
int read_Register(int regNum, RegFile* file);
void write_Register(int regNum, int value, RegFile* file);

#endif