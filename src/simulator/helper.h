/**
 * @file helper.h
 * @author Nicholas Hays & Henry Lawrence
 *
 * @brief Prototype functions to render data on TreeView widgets.  
 */
#ifndef HELPER_H_   /* Include guard */
#define HELPER_H_

/** Holds the cell renderer data values for the register tree view*/
enum
{
	REG_NUM,
	REG_VAL,
	NUM_REGISTER_COLS
} ;

/** Holds the cell renderer data values for the instruction tree view*/
enum
{
  COL_ADDRESS = 0,
  COL_VALUE,
  NUM_INSTRUCTION_COLS
} ; 


/** Structure for holding references to TreeView Widgets and CPU data. */
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