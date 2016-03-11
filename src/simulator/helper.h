#ifndef HELPER_H_   /* Include guard */
#define HELPER_H_

enum
{
	REG_NUM,
	REG_VAL,
	NUM_REGISTER_COLS
	
} ;

enum
{
  COL_ADDRESS = 0,
  COL_VALUE,
  NUM_INSTRUCTION_COLS
} ; 

typedef struct {
	GtkTreeView *instView;
	GtkListStore *instList;
	GtkListStore *regList;
	ControlUnit *unit;
} UpdateData;

char *intToString(int val);

char *getHexVal(int val);

char *get32BitIntString(int value);

void refresh_lists (UpdateData *data);

#endif // HELPER_H_