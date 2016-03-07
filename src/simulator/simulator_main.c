// author: Nicholas hays
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../model/controlunit.h"


		
/* global variables */

GtkTreeModel *instruction_model;
GtkTreeModel *reg_model;

 enum
{
  COL_ADDRESS = 0,
  COL_VALUE,
  NUM_INSTRUCTION_COLS
} ; 
enum 
{
	REG_NUM,
	REG_VAL,
	NUM_REGISTER_COLS
	
} ;

struct UpdateData {
	GtkTreeView *instView;
	GtkListStore *instList;
	GtkListStore *regList;
	ControlUnit *unit;
};


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

void refresh_lists (struct UpdateData *data) {
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
		gtk_list_store_set(instructionList, &iter, 1, get32BitIntString(updatedValue), -1);
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
		gtk_list_store_set(regList, &iter, 1, updatedValue, -1);
	}
	
	//Update PC
	g_return_if_fail(regList != NULL);
	updatedValue = unit->programCounter;
	pathToRow = gtk_tree_path_new_from_string("1"); // '1' is row where PC resides
	valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(regList), &iter, pathToRow);
	if (valid) {
		gtk_list_store_set(regList, &iter, 1, updatedValue, -1);
	}
	
	//Update IR
	g_return_if_fail(regList != NULL);
	updatedValue = unit->instructionRegister;
	pathToRow = gtk_tree_path_new_from_string("0"); // '0' is row where IR resides
	valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(regList), &iter, pathToRow);
	if (valid) {
		gtk_list_store_set(regList, &iter, 1, get32BitIntString(updatedValue), -1);
	}
	
	
}


/*=============================SUPPLEMENTARY CALLBACK FUNCTIONS=====================================*/

  
  /** This callback function will grab the selected row in the instruction treeview that the user is pointing to.
	  Grabs a reference to a selection object connected to the "changed" signal (single click) */
  
static void instruction_tree_selection_changed_cb (GtkTreeSelection *selection, gpointer data){
	
        GtkTreeIter iter;    
        guint32 address;
		gchar *instruction;

        if (gtk_tree_selection_get_selected (selection, &instruction_model, &iter)) 
		{
			gtk_tree_model_get (instruction_model, &iter, COL_ADDRESS, &address, -1);
				
            g_print ("Single Click: The value at address %" G_GUINT32_FORMAT, address);
				
			gtk_tree_model_get (instruction_model, &iter, COL_VALUE, &instruction, -1);
				
			g_print (" is: %s", instruction);
				
			g_print("\n");
				
			g_free (instruction);
        }
}
 /** This callback function will grab the selected row in the register treeview that the user is pointing to.
	  Grabs a reference to a selection object connected to the "changed" signal (single click) */
	  
static void reg_tree_selection_changed_cb (GtkTreeSelection *selection, gpointer data) {
    GtkTreeIter iter;
    gchar *reg_num;
	guint reg_val;

    if (gtk_tree_selection_get_selected (selection, &reg_model, &iter))
    {
        gtk_tree_model_get (reg_model, &iter, REG_NUM, &reg_num, -1);

		g_print ("Single Click: The value of %s ", reg_num);

		gtk_tree_model_get (reg_model, &iter, REG_VAL, &reg_val, -1);
				
		g_print ("is: %" G_GUINT32_FORMAT, reg_val);
		
		g_print("\n");
	
		g_free (reg_num);
    }
}


/** This method will grab the selected row in the treeview that the user is pointing when
	the user double clicks a treeview row with the "activate-on-single-click" property set to FALSE */

void onInstrTreeViewRowActivated (GtkTreeView *view, GtkTreePath *path, GtkTreeViewColumn *col, gpointer user_data) {
	
  GtkTreeIter   iter;
  GtkTreeModel *model;
  gchar *instruction;
  guint32 address;
  
  model = gtk_tree_view_get_model(view);

  if (gtk_tree_model_get_iter(model, &iter, path))
  {
	  
   gtk_tree_model_get (instruction_model, &iter, COL_ADDRESS, &address, -1);
   
   g_print ("Double click: The value at address %" G_GUINT32_FORMAT, address);
				
			gtk_tree_model_get (instruction_model, &iter, COL_VALUE, &instruction, -1);
				
			g_print (" is: %s", instruction);
				
			g_print("\n");
				
			g_free (instruction);

  }

}

/** This method will grab the selected row in the treeview that the user is pointing when
	the user double clicks a treeview row with the "activate-on-single-click" property set to FALSE */

void onRegTreeViewRowActivated (GtkTreeView *view, GtkTreePath *path, GtkTreeViewColumn *col, gpointer user_data) {
	
	GtkTreeIter iter;
	GtkTreeModel *model;
    gchar *reg_num;
	guint reg_val;
	model = gtk_tree_view_get_model(view);
	
     if (gtk_tree_model_get_iter(model, &iter, path))
  {
        gtk_tree_model_get (model, &iter, REG_NUM, &reg_num, -1);

		g_print ("Double click: The value of %s ", reg_num);

		gtk_tree_model_get (model, &iter, REG_VAL, &reg_val, -1);
				
		g_print ("is: %" G_GUINT32_FORMAT, reg_val);
		
		g_print("\n");
	
		g_free (reg_num);
    }
}



/** call back function to properly destroy the resources associated with the 
    widget. */
   
void on_window_main_destroy(GtkWidget *object, gpointer user_data) {
	
	gtk_main_quit();
	
}

/** reinitlizes the machine for a new program to be loaded. */
void reinitialize_machine(GtkWidget *widget, gpointer user_data) {
	//clear memory in control unit
	//clear registers in register file
	//reset PC and IR
	//call create_instruction_view_and_model to clear tree view
	//call refresh_lists
}






/*=============================INSTRUCTION TREE=====================================*/


/** Initializes the TreeModel(the tree model is a generic interface used by any arbitrary store, in this
	case we are using a store that will be a list, referred to in gtk as a "liststore") by adding 32 bit 
	instruction values i.e "0000000..." to each row in our list.*/
static GtkTreeModel *create_and_fill_instruction_model (Memory *mem)
{
  GtkListStore  *store;
  GtkTreeIter   iter;
  
  store = gtk_list_store_new (NUM_INSTRUCTION_COLS, G_TYPE_UINT, G_TYPE_STRING);
  
  int i;
  
	for(i = 0; i < mem->size; i++) 
	{ 
    /* Append a row and fill in some data */
	char *value = get32BitIntString(mem->read(mem, i));
    
	gtk_list_store_append (store, &iter);
    
	gtk_list_store_set (store, &iter,
                        COL_ADDRESS, (guint32) i,
                        COL_VALUE, value,
                        -1);
	}
  return GTK_TREE_MODEL (store);
}


/** Creates the TreeView (a generic widget for displaying content from a tree store) with a couple columns 
	(i.e a column for the hex address corresponding to the actual address of the instruction stored in memory, 
	and also a column for displaying the acutal instruction at that particular address).
	It connects the previously tree view to a list store, so that the view now has a reference to the model. */

static GtkWidget *create_instruction_view_and_model (Memory *mem)
{
  GtkCellRenderer     *renderer;
  GtkTreeModel        *model;
  GtkWidget           *view;

  view = gtk_tree_view_new ();

  /* --- Column #1 --- */

  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,      
                                               "Hex Address",  
                                               renderer,
                                               "text", COL_ADDRESS,
                                               NULL);

  /* --- Column #2 --- */

  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,      
                                               "Binary Value",  
                                               renderer,
                                               "text", COL_VALUE,
                                               NULL);

  model = create_and_fill_instruction_model (mem);

  gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

  /* The tree view has acquired its own reference to the
   *  model, so we can drop ours. That way the model will
   *  be freed automatically when the tree view is destroyed */

  g_object_unref (model);

  return view;
}







/*===========================REGISTER TREE============================*/


//register model
static GtkTreeModel *
create_and_fill_register_model (void)
{
  GtkListStore  *store;
  GtkTreeIter    iter;
  
	store = gtk_list_store_new (NUM_REGISTER_COLS, G_TYPE_STRING, G_TYPE_INT);
	int i;
	gtk_list_store_append (store, &iter);
	gtk_list_store_set(store, &iter,
						REG_NUM, "IR",
						REG_VAL, (gint32) 0,
						-1);
	gtk_list_store_append (store, &iter);
	gtk_list_store_set(store, &iter,
						REG_NUM, "PC",
						REG_VAL, (gint32) 0,
						-1);
	
	for(i = 0; i < 16; i++) {
		
	/* Append a row and fill in some data */
	gchar *my_reg_num = g_strdup_printf("R%i", i);
	
	gtk_list_store_append (store, &iter);
	
	gtk_list_store_set (store, &iter,
                      REG_NUM, my_reg_num,
                      REG_VAL, (gint32) 0, 
                      -1);
	}
  return GTK_TREE_MODEL (store);
}


static GtkWidget *create_register_view_and_model (void) {
  GtkCellRenderer     *renderer;
  GtkTreeModel        *model;
  GtkWidget           *view;

  view = gtk_tree_view_new ();

  /* --- Column #1 --- */

  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,      
                                               "Register",  
                                               renderer,
                                               "text", REG_NUM,
                                               NULL);

  /* --- Column #2 --- */

  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,      
                                               "Value",  
                                               renderer,
                                               "text", REG_VAL,
                                               NULL);

  model = create_and_fill_register_model ();

  gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

  /* The tree view has acquired its own reference to the
   *  model, so we can drop ours. That way the model will
   *  be freed automatically when the tree view is destroyed */

  g_object_unref (model);

  return view;
}






/*===========================Open File============================*/

void load_memory(FILE *fp, struct UpdateData *data) {
	ControlUnit *unit = data->unit;
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
		unit->memory->write(unit->memory, address, instruction);
		refresh_lists(data);
	}
	free(origin);
	free(buff);
}

int open_file(char *filename, struct UpdateData *data) {
	FILE *fp;
	fp = fopen(filename, "rb");
	if (fp != NULL) {
		load_memory(fp, data);
	}
	fclose(fp);
	return fp != NULL;
}

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
		struct UpdateData *data = (struct UpdateData *) user_data;
		open_file (filename, data);
		g_free (filename);
	}
	
	gtk_widget_destroy(dialog);
}







/*===========================WINDOW INITIALIZATION============================*/


// initializes some basic styling to the gtk window. 
void init_default_styling(GtkWindow *window) {
	gtk_window_set_title(window, "Vader");
	gtk_window_set_default_size (window, 350, 700);
}


/*===========================KEY COMMANDS============================*/


// if the "F5" key is pressed, then this call back function will advance the program to the 
// next instruction.  
gboolean advanceLine(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
	
	guint keycode = event->keyval;
	
	struct UpdateData *data = (struct UpdateData *) user_data;
	
		if(keycode == gdk_keyval_from_name("F5")) {
			
			printf("Running Next Instruction\n");
			
			data->unit->nextInst(data->unit);
			
			refresh_lists(data);
		}
	return 1;
}
		
	
		
		
int	main (int argc, char **argv){
	
	ControlUnit *unit = controlUnitConst(0, 0x5000);
  
	GtkTreeSelection *instruction_select; 	
	GtkTreeSelection *reg_select;
	GtkWidget *window;
	GtkWidget *instr_tree_view;
	GtkWidget *reg_tree_view;
	GtkWidget *instr_scrolled_window;
	GtkWidget *reg_scrolled_window;
	GtkWidget *box;
	
	GtkWidget *file_menu_item;
	GtkWidget *reinitialize_machine_menu_item;
	GtkWidget *exit_menu_item;
	GtkWidget *load_program_menu_item;
	GtkWidget *file_menu;
	GtkWidget *menu_bar;
	GtkWidget *line_seperator_1;
	GtkWidget *line_seperator_2;
	
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
	instr_tree_view = create_instruction_view_and_model (unit->memory);
	reg_tree_view = create_register_view_and_model ();
	
	/* menu bar */
	
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
	
	
	/*add components to layout container */
	
	gtk_container_add( GTK_CONTAINER(reg_scrolled_window), reg_tree_view);
	gtk_container_add (GTK_CONTAINER(instr_scrolled_window), instr_tree_view);
	gtk_box_pack_start(GTK_BOX(box), menu_bar,FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), reg_scrolled_window, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box), instr_scrolled_window, TRUE, TRUE, 0);

	gtk_container_add(GTK_CONTAINER(window), box);
									
    g_signal_connect(G_OBJECT(exit_menu_item), "activate", G_CALLBACK(on_window_main_destroy), NULL);
	
	GtkTreeModel *instModel = gtk_tree_view_get_model(GTK_TREE_VIEW(instr_tree_view));
	GtkTreeModel *regModel = gtk_tree_view_get_model(GTK_TREE_VIEW(reg_tree_view));
	struct UpdateData data = {GTK_TREE_VIEW(instr_tree_view), GTK_LIST_STORE(instModel), GTK_LIST_STORE(regModel), unit};
    g_signal_connect(G_OBJECT(load_program_menu_item), "activate", G_CALLBACK(load_binary_file), &data);
	g_signal_connect(G_OBJECT(reinitialize_machine_menu_item), "activate", G_CALLBACK(reinitialize_machine), &data);
    g_signal_connect (window, "delete_event", G_CALLBACK(on_window_main_destroy), NULL); /* dirty */
    g_signal_connect (instr_tree_view, "row-activated",  G_CALLBACK(onInstrTreeViewRowActivated), COL_ADDRESS);
	
    g_signal_connect (reg_tree_view, "row-activated",  G_CALLBACK(onRegTreeViewRowActivated), NULL);
    g_signal_connect (window, "key-press-event", G_CALLBACK(advanceLine), &data);
  
    reg_select = gtk_tree_view_get_selection (GTK_TREE_VIEW (reg_tree_view));
    instruction_select = gtk_tree_view_get_selection (GTK_TREE_VIEW (instr_tree_view));
  
    gtk_tree_selection_set_mode (reg_select, GTK_SELECTION_SINGLE);
    gtk_tree_selection_set_mode (instruction_select, GTK_SELECTION_SINGLE);
    g_signal_connect (G_OBJECT (reg_select), "changed",
                    G_CALLBACK (reg_tree_selection_changed_cb),
                    NULL);
    g_signal_connect (G_OBJECT (instruction_select), "changed",
                      G_CALLBACK (instruction_tree_selection_changed_cb),
                      NULL);
    gtk_widget_show_all (window);

    gtk_main ();
  
    controlUnitDeconst(unit);
  
    return 0;
}