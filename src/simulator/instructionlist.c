#include <gtk/gtk.h>
#include "../model/controlunit.h"
#include "helper.h"


  /** This callback function will grab the selected row in the instruction treeview that the user is pointing to.
	  Grabs a reference to a selection object connected to the "changed" signal (single click) */
  
static void instruction_tree_selection_changed_cb (GtkTreeSelection *selection, gpointer data){
	
        GtkTreeIter iter;    
        guint32 address;
		gchar *instruction;
		GtkTreeModel *instruction_model = (GtkTreeModel *) data;

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
	  
   gtk_tree_model_get (model, &iter, COL_ADDRESS, &address, -1);
   
   g_print ("Double click: The value at address %" G_GUINT32_FORMAT, address);
				
			gtk_tree_model_get (model, &iter, COL_VALUE, &instruction, -1);
				
			g_print (" is: %s", instruction);
				
			g_print("\n");
				
			g_free (instruction);

  }

}



/** Initializes the TreeModel(the tree model is a generic interface used by any arbitrary store, in this
	case we are using a store that will be a list, referred to in gtk as a "liststore") by adding 32 bit 
	instruction values i.e "0000000..." to each row in our list.*/
static GtkTreeModel *create_and_fill_instruction_model (Memory *mem)
{
  GtkListStore  *store;
  GtkTreeIter   iter;
  
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