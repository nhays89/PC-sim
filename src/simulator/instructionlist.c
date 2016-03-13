/**
 * @file instructionlist.c
 * @author Nicholas Hays & Henry Lawrence
 *
 * @brief Builds an Instruction List GtkTreeView.  
 */
#include <gtk/gtk.h>
#include "../model/controlunit.h"
#include "helper.h"


 /** @brief Callback function when a selected row is clicked (single-click) in the instruction TreeView.
	  @param selection the selceted row in the GtkTreeView.
	  @param data the data to pass into the callback function.*/
  
static void instruction_tree_selection_changed_cb (GtkTreeSelection *selection, gpointer data){
	
        GtkTreeIter iter;    
        gchar *address;
		gchar *instruction;
		GtkTreeModel *instruction_model = (GtkTreeModel *) data;

        if (gtk_tree_selection_get_selected (selection, &instruction_model, &iter)) 
		{
			gtk_tree_model_get (instruction_model, &iter, COL_ADDRESS, &address, -1);
				
            g_print ("Single Click: The value at address %s", address);
				
			gtk_tree_model_get (instruction_model, &iter, COL_VALUE, &instruction, -1);
				
			g_print (" is: %s", instruction);
				
			g_print("\n");
				
			g_free (instruction);
        }
}

/** @brief Callback function when a selected row is clicked (double-click) in the isntruction TreeView.
	The user double clicks a treeview row with the "activate-on-single-click" property set to FALSE. 
	 @param view the GtkTreeView.
	 @param path the path to the row in the GtkTreeView.
	 @param col the column in the GtkTreeView.
	 @param user_data the data to pass into the callback function.*/

void onInstrTreeViewRowActivated (GtkTreeView *view, GtkTreePath *path, GtkTreeViewColumn *col, gpointer user_data) {
	
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *instruction;
  gchar *address;
  
  model = gtk_tree_view_get_model(view);

  if (gtk_tree_model_get_iter(model, &iter, path))
  {
	  
   gtk_tree_model_get (model, &iter, COL_ADDRESS, &address, -1);
   
   g_print ("Double click: The value at address %s", address);
				
			gtk_tree_model_get (model, &iter, COL_VALUE, &instruction, -1);
				
			g_print (" is: %s", instruction);
				
			g_print("\n");
				
			g_free (instruction);

  }

}



/** @brief Initializes the GtkListStore with addresses from the CPU memory to each row in the list.
	@param mem the memory from the CPU to load.
	@return a GtkTreeModel that represents the list store.*/
	
static GtkTreeModel *create_and_fill_instruction_model (Memory *mem)
{
  GtkListStore *store;
  GtkTreeIter iter;
  
  store = gtk_list_store_new (NUM_INSTRUCTION_COLS, G_TYPE_STRING, G_TYPE_STRING);
  
  int i;
  
	for(i = 0; i < mem->size; i++) 
	{ 
		/* Append a row and fill in some data */
		
		char *value = get32BitIntString(mem->read(mem, i));
		gtk_list_store_append (store, &iter);
		gtk_list_store_set (store, &iter,
							COL_ADDRESS, getHexVal(i),
							COL_VALUE, value,
							-1);
	}
  return GTK_TREE_MODEL (store);
}


/** Creates the TreeView (a generic widget for displaying content from a tree store) with a 2 columns: 
	column 1 - the hex address corresponding to the actual address of the instruction stored in memory, 
    column 2 - displays the acutal instruction at that particular address.
	It connects the previously tree view to a list store, so that the view now has a reference to the model.
	@param mem the memory in the CPU that holds the instructions to view.
	@return the TreeView widget associated with the Tree Model. */

static GtkWidget *create_instruction_view_and_model (Memory *mem)
{
  GtkCellRenderer *renderer;
  GtkTreeModel *model;
  GtkWidget *view;

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

  g_object_unref (model);

  return view;
}


/** @brief Constructor to initialize the instruction's TreeModel and TreeView widget. 
	@param unit the Control Unit to pull data from.
	@return GtkTreeView that renders the data from the its TreeStore.*/

GtkWidget *instructionListConst(ControlUnit *unit) {
	GtkTreeSelection *instruction_select; 	
	GtkWidget *instr_tree_view;
	instr_tree_view = create_instruction_view_and_model (unit->memory);
	g_signal_connect (instr_tree_view, "row-activated",  G_CALLBACK(onInstrTreeViewRowActivated), COL_ADDRESS);
	instruction_select = gtk_tree_view_get_selection (GTK_TREE_VIEW (instr_tree_view));
	gtk_tree_selection_set_mode (instruction_select, GTK_SELECTION_SINGLE);
	g_signal_connect (G_OBJECT (instruction_select), "changed",
                      G_CALLBACK (instruction_tree_selection_changed_cb),
                      gtk_tree_view_get_model(GTK_TREE_VIEW(instr_tree_view)));
	return instr_tree_view;
}