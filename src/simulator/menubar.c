/**
 * @file menubar.c
 * @author Nicholas Hays & Henry Lawrence
 *
 * @brief Provides definitions for menu bar construction.  
 */
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "../model/controlunit.h"
#include "helper.h"


/** @brief Loads the bytes from the file to UpdateData structure. 
	@param fp the File to read.
	@param data the UpdateData strucutre. */
void load_memory(FILE *fp, UpdateData *data) {

	ControlUnit *unit = data->unit;
	printf("Unit pc: %d, ir: %d\n", unit->programCounter, unit->instructionRegister);
	int *origin = malloc(sizeof(int));
	fread(origin, sizeof(int), 1, fp);
	unit->programCounter = *origin;
	
	char pathToStart[20];
	int originalLoc = *origin;
	
	//convert PC to string
	sprintf(pathToStart, "%d", originalLoc);
	
	//scrolls to location cell
	gtk_tree_view_scroll_to_cell(data->instView, gtk_tree_path_new_from_string(pathToStart), NULL, FALSE, 1.0, 1.0);
	
	//Load content of file to memory
	int *buff = malloc(sizeof(int));
	int i = 0;
	for (i = 0; fread(buff, sizeof(int), 1, fp) == 1; i++) {
		int instruction = *buff;
		int address = *origin + i;
		printf("Loading %d into memory address %d\n", instruction, address);
		unit->memory->write(unit->memory, address, instruction);
		refresh_lists(data);
	}
	free(origin);
	free(buff);
}

/** @brief Opens the file to read in bytes. 
	@param filename the name of the file to read.
	@param data the UserData reference to update. 
	@return an integer representing if the file is null. */
	
int open_file(char *filename, UpdateData *data) {
	FILE *fp;
	fp = fopen(filename, "rb");
	if (fp != NULL) {
		load_memory(fp, data);
	}
	fclose(fp);
	return fp != NULL;
}
/** @brief Opens the GtkFileChooser Widget to load the binaray file.
	@param object optional argument.
	@param user_data user data to supply to the call back function. */
void load_binary_file(GtkWidget *object, gpointer user_data) {
	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;
	dialog = gtk_file_chooser_dialog_new ("Open File", NULL, action, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);
	
	res = gtk_dialog_run(GTK_DIALOG(dialog));
	if (res == GTK_RESPONSE_ACCEPT) {
		char *filename;
		GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
		filename = gtk_file_chooser_get_filename (chooser);
		UpdateData *data = (UpdateData *) user_data;
		open_file (filename, data);
		g_free (filename);
	}
	
	gtk_widget_destroy(dialog);
}

/** @brief Reinitlizes the machine for a new program to be loaded. 
	@deprecated is no longer supported. 
	@param widget the GtkTreeView to reinit.
	@param user_data data passed in to the function. */
void reinitialize_machine(GtkWidget *widget, gpointer user_data) {
	//clear memory in control unit
	//clear registers in register file
	//reset PC and IR
	//call create_instruction_view_and_model to clear tree view
	//call refresh_lists
}

/** @brief Menubar constructor. Responsible for calling functions to load assembly file. 
	@param instr_tree_view instruction TreeView widget.
	@param reg_tree_view register TreeView widget.
	@param unit ControlUnit which contains the data for the widgets.
	@return a menu bar GtkWidget. */

GtkWidget *menuBarConst(GtkWidget *instr_tree_view, GtkWidget *reg_tree_view, ControlUnit *unit) {
	GtkWidget *menu_bar;
	GtkWidget *file_menu_item;
	GtkWidget *reinitialize_machine_menu_item;
	GtkWidget *exit_menu_item;
	GtkWidget *load_program_menu_item;
	GtkWidget *file_menu;
	GtkWidget *line_seperator_1;
	GtkWidget *line_seperator_2;
	
	line_seperator_1 = gtk_separator_menu_item_new();
	line_seperator_2 = gtk_separator_menu_item_new();
	menu_bar = gtk_menu_bar_new();
	file_menu_item = gtk_menu_item_new_with_label ("File");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_menu_item);
	file_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (file_menu_item), file_menu);
	load_program_menu_item = gtk_menu_item_new_with_label("Load");
	reinitialize_machine_menu_item = gtk_menu_item_new_with_label("Reintialize Machine");
	exit_menu_item = gtk_menu_item_new_with_label("Exit");
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), load_program_menu_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), line_seperator_1);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), reinitialize_machine_menu_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), line_seperator_2);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), exit_menu_item);
	g_signal_connect(G_OBJECT(exit_menu_item), "activate", G_CALLBACK(gtk_main_quit), NULL);
	
	GtkTreeModel *instModel = gtk_tree_view_get_model(GTK_TREE_VIEW(instr_tree_view));
	GtkTreeModel *regModel = gtk_tree_view_get_model(GTK_TREE_VIEW(reg_tree_view));
	
	UpdateData *data = malloc(sizeof(UpdateData));
	data->instView = GTK_TREE_VIEW(instr_tree_view);
	data->instList = GTK_LIST_STORE(instModel);
	data->regList = GTK_LIST_STORE(regModel);
	data->unit = unit;
	
    g_signal_connect(G_OBJECT(load_program_menu_item), "activate", G_CALLBACK(load_binary_file), data);
	g_signal_connect(G_OBJECT(reinitialize_machine_menu_item), "activate", G_CALLBACK(reinitialize_machine), data);
	
	return menu_bar;
}