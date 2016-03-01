// author: Nicholas hays
#include <gtk/gtk.h>
#include <stdio.h>
//#include <gdk/gdkkeysyms.h>

		
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
	pathToRow = gtk_tree_path_new_from_string("1:2000");
	
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
        guint32 address;
		gchar *instruction;

        if (gtk_tree_selection_get_selected (selection, &instruction_model, &iter))
        {
				gtk_tree_model_get (instruction_model, &iter, COL_ADDRESS, &address, -1);

                g_print ("The value at address %" G_GUINT32_FORMAT, address);
				gtk_tree_model_get (instruction_model, &iter, COL_VALUE, &instruction, -1);
				
				g_print (" is: %s", instruction);
				g_print("\n");
				 

                //g_free (address);
				g_free (instruction);
        }
}
static void
reg_tree_selection_changed_cb (GtkTreeSelection *selection, gpointer data)
{
        GtkTreeIter iter;
        gchar *reg_num;
		guint reg_val;

        if (gtk_tree_selection_get_selected (selection, &reg_model, &iter))
        {
                gtk_tree_model_get (reg_model, &iter, REG_NUM, &reg_num, -1);

					g_print ("The value of %s ", reg_num);

				 gtk_tree_model_get (reg_model, &iter, REG_VAL, &reg_val, -1);
				
				g_print ("is: %" G_GUINT32_FORMAT, reg_val);
				g_print("\n");
				
                //g_free (address);
				g_free (reg_num);
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
create_and_fill_instruction_model (void)
{
  GtkListStore  *store;
  GtkTreeIter    iter;
  
  store = gtk_list_store_new (NUM_INSTRUCTION_COLS, G_TYPE_UINT, G_TYPE_STRING);
  int i;
for(i = 0; i < 1000; i++) {
  /* Append a row and fill in some data */
  
  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter,
                      COL_ADDRESS, (guint32) i,
                      COL_VALUE, "00000000000000000000000000000000",
                      -1);
}
  return GTK_TREE_MODEL (store);
}

//register model
static GtkTreeModel *
create_and_fill_register_model (void)
{
  GtkListStore  *store;
  GtkTreeIter    iter;
  
  store = gtk_list_store_new (NUM_REGISTER_COLS, G_TYPE_STRING, G_TYPE_UINT);
  int i;
	gtk_list_store_append (store, &iter);
	gtk_list_store_set(store, &iter,
						REG_NUM, "IR",
						REG_VAL, (guint32) 0,
						-1);
	gtk_list_store_append (store, &iter);
	gtk_list_store_set(store, &iter,
						REG_NUM, "PC",
						REG_VAL, (guint32) 0,
						-1);
	
for(i = 0; i < 16; i++) {
  /* Append a row and fill in some data */
  gchar *my_reg_num = g_strdup_printf("R%i", i);
  gtk_list_store_append (store, &iter);

//optional to hold reg value with numbers
 /*  gtk_list_store_set (store, &iter,
                      REG_NUM, (guint32) i,
                      REG_VAL, "0",
                      -1); */
	gtk_list_store_set (store, &iter,
                      REG_NUM, my_reg_num,
                      REG_VAL, (guint32) 0, //global variable to store reg value?
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
create_instruction_view_and_model (void)
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

  model = create_and_fill_instruction_model ();

  gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

  /* The tree view has acquired its own reference to the
   *  model, so we can drop ours. That way the model will
   *  be freed automatically when the tree view is destroyed */

  g_object_unref (model);

  return view;
}

static GtkWidget *
create_register_view_and_model (void)
{
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
//call back function to properly destroy the resources associated with the 
//main widget. 
void 
on_window_main_destroy(GtkWidget *object, gpointer user_data) {
	g_print("in window destory");
	//g_object_unref(G_OBJECT(builder));
	gtk_main_quit();
}

void 
load_binary_file(GtkWidget *object, gpointer user_data) {
	
	GtkWidget *dialog;
GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
gint res;

dialog = gtk_file_chooser_dialog_new ("Open File",
                                      NULL,
                                      action,
                                      "Cancel",
                                      GTK_RESPONSE_CANCEL,
                                      "Open",
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);

res = gtk_dialog_run (GTK_DIALOG (dialog));
if (res == GTK_RESPONSE_ACCEPT)
  {
	  
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    filename = gtk_file_chooser_get_filename (chooser);
   FILE *bin_file = fopen (filename, "r");
   int c;
 while(1)
   {
      c = fgetc(bin_file);
      if( feof(bin_file) )
      { 
         break ;
      }
      printf("%c", c);
   }
   fclose(bin_file);
   
   
   
   /* 
	char *buffer;
	long filelen;

	fileptr = fopen("myfile.txt", "rb");  // Open the file in binary mode
	fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	filelen = ftell(fileptr);             // Get the current byte offset in the file
	rewind(fileptr);                      // Jump back to the beginning of the file

	buffer = (char *)malloc((filelen+1)*sizeof(char)); // Enough memory for file + \0
	fread(buffer, filelen, 1, fileptr); // Read in the entire file
	fclose(bin_file); // Close the file */
    g_free (filename); 
  }

gtk_widget_destroy (dialog);
	
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
	//GtkTreeIter   iter;
	//GtkTreeModel *model;
	guint keycode = event->keyval;
	
	if(keycode == gdk_keyval_from_name("F5")) {
		g_print("keycode equals %d", keycode);
		//model = gtk_tree_view_get_model(GTK_TREE_VIEW(widget));	
	}
return 1;
}
		

		
		
int	
main (int argc, char **argv)
{
	/* css */
	//GtkCssProvider *provider;
	//GdkDisplay *display;
	//GdkScreen *screen;
  
	GtkTreeSelection *instruction_select; 	
	GtkTreeSelection *reg_select;
	GtkWidget *window;
	GtkWidget *instr_tree_view;
	GtkWidget *reg_tree_view;
	GtkWidget *instr_scrolled_window;
	GtkWidget *reg_scrolled_window;
	GtkWidget *box;
	//GtkWidget *pane;
	
	GtkWidget *file_menu_item;
	GtkWidget *exit_menu_item;
	GtkWidget *load_program_menu_item;
	GtkWidget *file_menu;
	GtkWidget *menu_bar;
	GtkWidget *seperator;
	
	gtk_init (&argc, &argv);
	
	//pane = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	init_default_styling(GTK_WINDOW(window));

	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	instr_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	reg_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (instr_scrolled_window),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (reg_scrolled_window),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);				
	instr_tree_view = create_instruction_view_and_model ();
	reg_tree_view = create_register_view_and_model ();
	/* menu bar */
	
	seperator = gtk_separator_menu_item_new();
	menu_bar = gtk_menu_bar_new();
	file_menu_item = gtk_menu_item_new_with_label ("File");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_menu_item);
	file_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (file_menu_item), file_menu);
	load_program_menu_item = gtk_menu_item_new_with_label("Load");
	exit_menu_item = gtk_menu_item_new_with_label("Exit");
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), load_program_menu_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), seperator);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), exit_menu_item);
	
	
	/*add components to layout container */
	
	//gtk_container_add(GTK_CONTAINER(pane));
	gtk_container_add( GTK_CONTAINER(reg_scrolled_window), reg_tree_view);
	gtk_container_add (GTK_CONTAINER(instr_scrolled_window), instr_tree_view);
	gtk_box_pack_start(GTK_BOX(box), menu_bar,FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), reg_scrolled_window, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box), instr_scrolled_window, TRUE, TRUE, 0);
	//gtk_grid_attach (GTK_GRID (grid), menu_bar, 0,0,5,5);
	
	//gtk_grid_attach(GTK_GRID(grid), scrolled_window,1, 0, 5,5);
	gtk_container_add(GTK_CONTAINER(window), box);
	

/*   provider = gtk_css_provider_new ();
  display = gdk_display_get_default ();
  screen = gdk_display_get_default_screen (display);
  gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

 gtk_css_provider_load_from_data (GTK_CSS_PROVIDER(provider),"GtkTreeView {\n background-color: black;\n}\n GtkWindow {\n background-color: black;\n}\n",-1, NULL);  */

 
  //g_object_unref (provider);
									
  g_signal_connect(G_OBJECT(exit_menu_item), "activate", G_CALLBACK(on_window_main_destroy), NULL);
  g_signal_connect(G_OBJECT(load_program_menu_item), "activate",G_CALLBACK(load_binary_file), NULL);
  g_signal_connect (window, "delete_event", G_CALLBACK(on_window_main_destroy), NULL); /* dirty */
  //g_signal_connect (instr_tree_view, "row-activated", G_CALLBACK(write_to_list_store), NULL); 
  g_signal_connect (instr_tree_view, "row-activated",  G_CALLBACK(onTreeViewRowActivated), NULL);
   g_signal_connect (reg_tree_view, "row-activated",  G_CALLBACK(onTreeViewRowActivated), NULL);
  g_signal_connect(G_OBJECT(instr_tree_view), "key-press-event", G_CALLBACK(advanceLine), NULL);
  
  reg_select = gtk_tree_view_get_selection (GTK_TREE_VIEW (reg_tree_view));
  instruction_select = gtk_tree_view_get_selection (GTK_TREE_VIEW (instr_tree_view));
  
  gtk_tree_selection_set_mode (reg_select, GTK_SELECTION_SINGLE);
   gtk_tree_selection_set_mode (instruction_select, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (reg_select), "changed",
                  G_CALLBACK (reg_tree_selection_changed_cb),
                  NULL);
  g_signal_connect (G_OBJECT (instruction_select), "changed",
                  G_CALLBACK (tree_selection_changed_cb),
                  NULL);
  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}