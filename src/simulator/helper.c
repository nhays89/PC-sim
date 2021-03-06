/**
 * @file helper.c
 * @author Nicholas Hays & Henry Lawrence
 *
 * @brief Helper functions to render data on TreeView widgets.  
 */
#include <gtk/gtk.h>
#include <stdlib.h>
#include <math.h>
#include "../model/controlunit.h"
#include "helper.h"

/** @brief Converts an integer to a string. 
	@param value an integer to convert to string
	@return pointer to a 32 bit binary string.*/
	
char *intToString(int value) {
	char *str = calloc(11, sizeof(char)); //Max chars needed for int string + null
	sprintf(str, "%d", value);
	return str;
}

/** @brief Converts a decimal integer to a hex string.  
	@param value an integer to convert to hex
	@return pointer to a string hex address.*/
	
char *getHexVal(int value) {
	char *res = calloc(9, sizeof(char)); //2 hex vals / byte + null
	if (value <= 0xFFFF)
	{
		sprintf(&res[0], "%04x", value);
	}
	return res;
}

/** @brief Gets a string representation of a 32-bit integer.  
	@param value an integer to convert to a binary string. 
	@return pointer to a 32 bit binary string.*/
	
char *get32BitIntString (int value) {
	char *bits = calloc(32 + 1, 1);
	int i;
	for (i = 0; i < 32; i++) {
		if (value % 2 == 0) 
			bits[31 - i] = '0';
		else 
			bits[31 - i] = '1';
		value /= 2;
	}
	return bits;
}

/** @brief Updates the instruction tree view and register tree view with data from the CPU memory and register file.
	@param data a reference to the data structure */
	
void refresh_lists (UpdateData *data) {
	GtkTreeIter  iter;
	gboolean     valid;
	GtkTreePath *pathToRow;
	ControlUnit *unit = data->unit;
	GtkListStore *regList = GTK_LIST_STORE(data->regList);
	GtkListStore *instructionList = GTK_LIST_STORE(data->instList);
	GtkTreeView *view = data->instView;
	
	//Update Instruction List
	g_return_if_fail(instructionList != NULL);
	int maxMemAddressSize = log10(unit->memory->size);
	char path[3 + maxMemAddressSize];
	int updatedAddress = unit->memory->lastModified;
	int updatedValue = unit->memory->read(unit->memory, updatedAddress);
	printf("Memory address %d updated to %d\n", updatedAddress, updatedValue);
	sprintf(path, "%d", updatedAddress);
	pathToRow = gtk_tree_path_new_from_string(path);
    valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(instructionList), &iter, pathToRow);
	if (valid) {
		gtk_list_store_set(instructionList, &iter, COL_VALUE, get32BitIntString(updatedValue), -1);
	}
	
	//set highlighted path
	
	GtkTreeSelection *selection = gtk_tree_view_get_selection(view);	
    gtk_tree_selection_set_mode (selection, GTK_SELECTION_SINGLE);
	sprintf(path, "%d", unit->programCounter);
	pathToRow = gtk_tree_path_new_from_string(path);
	gtk_tree_selection_select_path(selection, pathToRow);
	
	//scrolls to location cell
	gtk_tree_view_scroll_to_cell(view, pathToRow, NULL, FALSE, 1.0, 1.0);
	
	//Update Register List
	g_return_if_fail(regList != NULL);
	updatedAddress = unit->regFile->lastModified;
	updatedValue = unit->regFile->read(updatedAddress,unit->regFile);
	printf("Register %d updated to %d\n", updatedAddress, updatedValue);
	sprintf(path, "%d", updatedAddress + 2);
	pathToRow = gtk_tree_path_new_from_string(path);
    valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(regList), &iter, pathToRow);
	if (valid) {
		gtk_list_store_set(regList, &iter, REG_VAL, intToString(updatedValue), -1);
	}
	
	//Update IR & PC
	updatedValue = unit->instructionRegister;
	pathToRow = gtk_tree_path_new_from_string("0");
	valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(regList), &iter, pathToRow);
	if (valid) {
		gtk_list_store_set(regList, &iter, REG_VAL, get32BitIntString(updatedValue), -1);
	}
	
	updatedValue = unit->programCounter;
	pathToRow = gtk_tree_path_new_from_string("1");
	valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(regList), &iter, pathToRow);
	if (valid) {
		gtk_list_store_set(regList, &iter, REG_VAL, getHexVal(updatedValue), -1);
	}
	
}

