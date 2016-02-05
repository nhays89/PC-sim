#ifndef REG_FILE_H_
#define REG_FILE_H_

typedef struct RegFile{
	int registers[16];
	int (*read)(int, struct RegFile*);
	void (*write)(int, int, struct RegFile*);
} RegFile;

RegFile regFileConst();
int read_Register(int, RegFile*);
void write_Register(int, int, RegFile*);

#endif