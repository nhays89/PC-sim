/**
 * @file simulator.c
 * @author Nicholas Hays & Henry Lawrence
 *
 * @brief implements the simulator GUI. 
 */
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../model/controlunit.h"
#include "menubar.h"
#include "registerlist.h"
#include "instructionlist.h"
#include "helper.h"



/** @brief Call back function to properly destroy the resources associated with the GtkWindow.
	@param object pointer to a GtkWindow
	@param user_data data passed to the callback*/
   
void on_window_main_destroy(GtkWidget *object, gpointer user_data) {
	
	gtk_main_quit();
	
}


/** @brief Initializes some basic styling and size to the GtkWindow widget.
	@param window pointer to a GtkWindow to style*/

void init_default_styling(GtkWindow *window) {
	gtk_window_set_title(window, "Vader");
	gtk_window_set_default_size (window, 350, 700);
}

/** @brief Call back function to advance the program to the next instruction. Occurs when "F5" key is pressed.
	@param widget GtkTreeView to reference
	@param event the key event signal that was invoked.
	@param user_data data passed to this callback.
	@return TRUE to stop other handlers from being invoked for the event. FALSE to propagate the event further.*/ 

gboolean advanceLine(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
	
	guint keycode = event->keyval;
	
	UpdateData *data = (UpdateData *) user_data;
	
		if(keycode == gdk_keyval_from_name("F5")) {
			
			printf("Running Next Instruction\n");
			
			data->unit->nextInst(data->unit);
			
			refresh_lists(data);
		}
	return 1;
}
		
/** Initialize everything needed to operate the Simulator. 
	@param number of command line args
	@param array of args*/	

void startSimulator(int argc, char **argv){
	
	ControlUnit *unit = controlUnitConst(0, 0x5000);
  
	GtkWidget *window;
	GtkWidget *instr_scrolled_window;
	GtkWidget *reg_scrolled_window;
	GtkWidget *box;
	
	
	gtk_init (&argc, &argv);
	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	init_default_styling(GTK_WINDOW(window));

	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	instr_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	reg_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (instr_scrolled_window),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (reg_scrolled_window),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

	GtkWidget *reg_tree_view = registerListConst();
	GtkWidget *instr_tree_view = instructionListConst(unit);
	GtkWidget *menu_bar = menuBarConst(instr_tree_view, reg_tree_view, unit);
	
	/*add components to layout container */
	
	gtk_container_add( GTK_CONTAINER(reg_scrolled_window), reg_tree_view);
	gtk_container_add (GTK_CONTAINER(instr_scrolled_window), instr_tree_view);
	gtk_box_pack_start(GTK_BOX(box), menu_bar,FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), reg_scrolled_window, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box), instr_scrolled_window, TRUE, TRUE, 0);

	gtk_container_add(GTK_CONTAINER(window), box);
	
    g_signal_connect (window, "delete_event", G_CALLBACK(on_window_main_destroy), NULL); /* dirty */
	
	GtkTreeModel *instModel = gtk_tree_view_get_model(GTK_TREE_VIEW(instr_tree_view));
	GtkTreeModel *regModel = gtk_tree_view_get_model(GTK_TREE_VIEW(reg_tree_view));
	UpdateData data = {GTK_TREE_VIEW(instr_tree_view), GTK_LIST_STORE(instModel), GTK_LIST_STORE(regModel), unit};
    g_signal_connect (window, "key-press-event", G_CALLBACK(advanceLine), &data);
  
    gtk_widget_show_all (window);

    gtk_main ();
  
    controlUnitDeconst(unit);
}