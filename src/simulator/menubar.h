/**
 * @file menubar.h
 * @author Nicholas Hays & Henry Lawrence
 *
 * @brief Prototype for menu bar construction.  
 */
#ifndef MEUNBAR_H_   /* Include guard */
#define MEUNBAR_H_
#include "../model/controlunit.h"

/** Constructs the GtkWidget Menu Bar.  */

GtkWidget *menuBarConst(GtkWidget *instructionTreeView, GtkWidget *registerTreeView, ControlUnit *unit);

#endif //MENUBAR_H_