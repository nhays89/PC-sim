/**
 * @file registerlist.c
 * @author Nicholas Hays & Henry Lawrence
 *
 * @brief Builds an Register List GtkTreeView.  
 */
#include <gtk/gtk.h>
#include "../model/controlunit.h"
#include "helper.h"

 /** @brief Callback function when a selected row is clicked (single-click) in the register TreeView.
	  @param selection the selected row in the GtkTreeView.
	  @param data the data to pass into the callback function.*/
	  
static void reg_tree_selection_changed_cb (GtkTreeSelection *selection, gpointer data) {
    GtkTreeIter iter;
    gchar *reg_num;
	gchar *reg_val;
	GtkTreeModel *reg_model = (GtkTreeModel *) data;

    if (gtk_tree_selection_get_selected (selection, &reg_model, &iter))
    {
        gtk_tree_model_get (reg_model, &iter, REG_NUM, &reg_num, -1);

		g_print ("Single Click: The value of %s ", reg_num);

		gtk_tree_model_get (reg_model, &iter, REG_VAL, &reg_val, -1);
				
		g_print ("is: %s", reg_val);
		
		g_print("\n");
	
		g_free (reg_num);
		
		g_free (reg_val);
    }
}


/** @brief Callback function when a selected row is clicked (double-click) in the register TreeView.
	The user double clicks a treeview row with the "activate-on-single-click" property set to FALSE. 
	 @param view the GtkTreeView.
	 @param path the path to the row in the GtkTreeView.
	 @param col the column in the GtkTreeView.
	 @param user_data the data to pass into the callback function.*/

void onRegTreeViewRowActivated (GtkTreeView *view, GtkTreePath *path, GtkTreeViewColumn *col, gpointer user_data) {
	
	GtkTreeIter iter;
	GtkTreeModel *model;
    gchar *reg_num;
	gchar *reg_val;
	model = gtk_tree_view_get_model(view);
	
     if (gtk_tree_model_get_iter(model, &iter, path))
  {
        gtk_tree_model_get (model, &iter, REG_NUM, &reg_num, -1);

		g_print ("Double click: The value of %s ", reg_num);

		gtk_tree_model_get (model, &iter, REG_VAL, &reg_val, -1);
				
		g_print ("is: %s", reg_val);
		
		g_print("\n");
	
		g_free (reg_num);
    }
}


/** @brief Initializes the GtkListStore with values from the CPU register file, IR, and PC to each row in the list.
	@return a GtkTreeModel that represents the list store.*/
static GtkTreeModel *create_and_fill_register_model (void) {
	GtkListStore *store;
	GtkTreeIter iter;
  
	store = gtk_list_store_new (NUM_REGISTER_COLS, G_TYPE_STRING, G_TYPE_STRING);
	int i;
	gtk_list_store_append (store, &iter);
	gtk_list_store_set(store, &iter,
						REG_NUM, "IR",
						REG_VAL, get32BitIntString(0),
						-1);
	gtk_list_store_append (store, &iter);
	gtk_list_store_set(store, &iter,
						REG_NUM, "PC",
						REG_VAL, getHexVal(0),
						-1);
	
	for(i = 0; i < 16; i++) {
		/* Append a row and fill in some data */
		gchar *my_reg_num = g_strdup_printf("R%i", i);
		gtk_list_store_append (store, &iter);

		gtk_list_store_set (store, &iter,
						REG_NUM, my_reg_num,
						REG_VAL, intToString(0), //global variable to store reg value?
						-1);
	}
  return GTK_TREE_MODEL (store);
}


/** Creates the TreeView (a generic widget for displaying content from a tree store) with a 2 columns: 
	column 1 - the register number, PC, or IR.  
    column 2 - displays the acutal value of that particular register. 
	It connects the previously tree view to a list store, so that the view now has a reference to the model.
	@return the TreeView widget associated with the Tree Model. */
	
static GtkWidget *create_register_view_and_model (void) {
  GtkCellRenderer *renderer;
  GtkTreeModel *model;
  GtkWidget *view;

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

  g_object_unref (model);

  return view;
}

/** @brief Constructor to initialize the register's TreeModel and TreeView widget. 
	@return GtkTreeView that renders the data from the its TreeStore.*/
	
GtkWidget *registerListConst() {
	GtkWidget *reg_tree_view;
	GtkTreeSelection *reg_select;
	reg_tree_view = create_register_view_and_model ();
	g_signal_connect (reg_tree_view, "row-activated",  G_CALLBACK(onRegTreeViewRowActivated), NULL);
	reg_select = gtk_tree_view_get_selection (GTK_TREE_VIEW (reg_tree_view));
	gtk_tree_selection_set_mode (reg_select, GTK_SELECTION_SINGLE);
	g_signal_connect (G_OBJECT (reg_select), "changed",
                    G_CALLBACK (reg_tree_selection_changed_cb),
                    gtk_tree_view_get_model(GTK_TREE_VIEW(reg_tree_view)));
	return reg_tree_view;
}