// author: Nicholas hays
#include <gtk/gtk.h>
//#include <gdk/gdkkeysyms.h>
enum
{
  COL_ADDRESS = 0,
  COL_VALUE,
  NUM_COLS
} ;
 void // initialize values 
  write_to_list_store (GtkListStore *liststore, gchar *startRow, gchar *binString)
  {
    GtkTreeIter  iter;
    gboolean     valid;
	GtkTreePath *pathToRow;
    g_return_if_fail ( liststore != NULL );

	// need a way to concatenate the 'startRow' argument being passed in
	// with the depth of the list store (which is 1).
	// so if the start row aka .orig = 3000, then we need to concatenate that
	// with '1:' to make '1:3000' which is where we will start rewriting values
	// in the list store to match what
	
    /* get path to the row in the tree model hard coded right now */
	pathToRow = gtk_tree_path_new_from_string(startRow);
	
	// only operate on this row if the row has been added to the model.
    valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(liststore), &iter, pathToRow);
	//gtk_list_store_set(liststore, )
	
	g_print("is gtk path a valid arg: '%i'", valid);
	
    if (valid)
    {
       /* ... do something with that row using the iter ...          */
       /* (Here column 0 of the list store is of type G_TYPE_STRING) */
       gtk_list_store_set(liststore, &iter, 0, &binString, -1);

       /* Make iter point to the next row in the list store */
      // valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(liststore), &iter);
	   //get operation to determine if data is .end
	  //gtk_tree_model_get(model, &iter, COL_NAME, &name, -1);
    }
  }
  
  //when the user clicks a row this method will grab the selected row in the treeview that the user is pointing to,
  //and then call this function.
static void
tree_selection_changed_cb (GtkTreeSelection *selection, gpointer data)
{
        GtkTreeIter iter;
        GtkTreeModel *model;
        guint32 address;
		gchar *instruction;

        if (gtk_tree_selection_get_selected (selection, &model, &iter))
        {
                gtk_tree_model_get (model, &iter, COL_ADDRESS, &address, -1);

                g_print ("%" G_GUINT32_FORMAT, address);
				
				 gtk_tree_model_get (model, &iter, COL_VALUE, &instruction, -1);
				
				g_print ("The value of the instruction is %s\n", instruction);

                //g_free (address);
				g_free (instruction);
        }
}
  
void
onTreeViewRowActivated (GtkTreeView *view, GtkTreePath *path,
                        GtkTreeViewColumn *col, gpointer userdata)
{
  GtkTreeIter   iter;
  GtkTreeModel *model;
	//g_print("in tree view row activated");
  model = gtk_tree_view_get_model(view);

  if (gtk_tree_model_get_iter(model, &iter, path))
  {
    guint32 value;

    gtk_tree_model_get(model, &iter, COL_ADDRESS, &value, -1);
	
   g_print ("%" G_GUINT32_FORMAT, value);
	//g_print("the value of");
   // g_free(value);
  }
	
}


//this method initializes the TreeModel(the tree model is a generic interface used by any arbitrary store, in this
//case we are using a store that will be a list, referreded in gtk as a "liststore") by adding values i.e "000..." to each row in
//our list. 
static GtkTreeModel *
create_and_fill_model (void)
{
  GtkListStore  *store;
  GtkTreeIter    iter;
  
  store = gtk_list_store_new (NUM_COLS, G_TYPE_UINT, G_TYPE_STRING);
  int i;
for(i = 0; i < 10; i++) {
  /* Append a row and fill in some data */
  
  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter,
                      COL_ADDRESS, (guint32) i,
                      COL_VALUE, "00000000000000000000000000000000",
                      -1);
}
  return GTK_TREE_MODEL (store);
}

//this method will do two things
//1. It will create the TreeView (a generic widget for displaying content from a store) with a couple columns 
//(i.e a column for the hex address corresponding to the actual address of the instruction stored in memory, 
// and also a column for displaying the acutal instruction at that particular address).
//2. It will connect the previously created store to the view, so that the view now has a reference to the model (our instructions).

static GtkWidget *
create_view_and_model (void)
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

  model = create_and_fill_model ();

  gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

  /* The tree view has acquired its own reference to the
   *  model, so we can drop ours. That way the model will
   *  be freed automatically when the tree view is destroyed */

  g_object_unref (model);

  return view;
}

static Gtk_Widget *
create_menu_bar_model(void) {
	
}






static Gtk Widget*
create_menu_bar(void) {
	GtkWidget *menu_model;
	GtkWidget *menu_bar;
	menu_model = create_menu_bar_model();
	menu_bar = gtk_menu_bar_new_from_model(menu_model);
	return menu_bar;
}





//call back function to properly destroy the resources associated with the 
//main widget. 
void 
on_window_main_destroy(GtkWidget *object, gpointer user_data) {
	g_print("in window destory");
	//g_object_unref(G_OBJECT(builder));
	gtk_main_quit();
}

// initializes some basic styling to the gtk window. 
void 
init_default_styling(GtkWindow *window) {
	
	gtk_window_set_title(window, "Vader");
	gtk_window_set_default_size (window,
                             350,
                             700);
}

// if the "F5" key is pressed, then this call back function will advance the program to the 
// next instruction.  
gboolean 
advanceLine(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {

	guint keycode = event->keyval;
	g_print("keycode equals %d", keycode);
	if(keycode == gdk_keyval_from_name("F5")) {
		g_print("inside key code if");
		
	}
return 1;
}
		
int	
main (int argc, char **argv)
{
  GtkTreeSelection *select; 	
  GtkWidget *window;
  GtkWidget *view;
  GtkWidget *scrolled_window;
  GtkWidget *grid;
  GtkWidget *menu_bar;
 
  
  gtk_init (&argc, &argv);
  
  
  menu_bar = create_menu_bar();
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  grid = gtk_grid_new();
  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  init_default_styling(GTK_WINDOW(window));
  view = create_view_and_model ();
  /* create a new scrolled window. */
    
	/* set scroll policy on scrolled window, first is horizontal, second is vertical */
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
									
									
  gtk_grid_attach (GTK_GRID(scrolled_window), view);
  gtk_grid_attach ()
  
  
  
  
  
  
  gtk_container_add(GTK_CONTAINER(window), grid);
	
	
	
	
	
	
	
	
  g_signal_connect (window, "delete_event", G_CALLBACK(on_window_main_destroy), NULL); /* dirty */
  //g_signal_connect (view, "row-activated", G_CALLBACK(write_to_list_store), NULL); 
  g_signal_connect (view, "row-activated",  G_CALLBACK(onTreeViewRowActivated), NULL);
  g_signal_connect(G_OBJECT(view), "key-press-event", G_CALLBACK(advanceLine), NULL);
  select = gtk_tree_view_get_selection (GTK_TREE_VIEW (view));
  gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (select), "changed",
                  G_CALLBACK (tree_selection_changed_cb),
                  NULL);
  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}