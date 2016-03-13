/**
 * @file instructionlist.h
 * @author Nicholas Hays & Henry Lawrence
 *
 * @brief Prototype for instruction list constructor.  
 */
#ifndef INSTRUCTIONLIST_H_   /* Include guard */
#define INSTRUCTIONLIST_H_
#include "../model/controlunit.h"

/** Constructs the Instruction TreeView widget. */

GtkWidget *instructionListConst(ControlUnit *unit);

#endif // INSTRUCTIONLIST_H_