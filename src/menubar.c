#include <gtk/gtk.h>
#include <stdio.h>
#include "controlunit.h"


/*
=========================MENU BAR=============================
*/
void loadToMemory(FILE *fp, ControlUnit *unit) {
	int *origin;
	fread(origin, sizeof(int), 1, fp);
	printf("Origin: %d\n", *origin);
	int *buff;
	//Load content of file to memory
	int i = 0;
	for (i = 0; fread(buff, sizeof(int), 1, fp) == 1; i++) {
		int instruction = *buff;
		int address = *origin + i;
		unit->memory->write(unit->memory, address, instruction);
		printf("Instruction %d loaded into memory location %d\n", instruction, address);
	}
	
}

int openFile(char* fileName, ControlUnit *unit) {
	FILE *fp;
	fp = fopen(fileName, "r");
	if (fp != NULL) {
		loadToMemory(fp, unit);
	}
	fclose(fp);
	return fp != NULL;
}

void chooseFile(GtkWidget *widget, ControlUnit *unit) {
	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;
	dialog = gtk_file_chooser_dialog_new ("Open File", NULL, action, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);
	
	res = gtk_dialog_run(GTK_DIALOG(dialog));
	if (res == GTK_RESPONSE_ACCEPT) {
		char *filename;
		GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
		filename = gtk_file_chooser_get_filename (chooser);
		openFile (filename, unit);
		g_free (filename);
	}
	
	gtk_widget_destroy(dialog);
}

GtkWidget *createMenuBar(ControlUnit *unit) {
	GtkWidget *menuBar = gtk_menu_bar_new();
	GtkWidget *fileItem = gtk_menu_item_new_with_label("File");
	gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), fileItem);
	
	GtkWidget *fileMenu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileItem), fileMenu);
	
	GtkWidget *openItem = gtk_menu_item_new_with_label("Open");
	GtkWidget *closeItem = gtk_menu_item_new_with_label("Close");
	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), openItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), closeItem);
	
	g_signal_connect (G_OBJECT(closeItem), "activate", G_CALLBACK(gtk_main_quit), (gpointer) NULL);
	g_signal_connect (G_OBJECT(openItem), "activate", G_CALLBACK(chooseFile), (gpointer) unit);
	
	return menuBar;
}



/*
====================MEMORY DISPLAY===========================
*/
enum
{
  COL_FIRST_NAME = 0,
  COL_LAST_NAME,
  NUM_COLS
} ;

static GtkTreeModel *
create_and_fill_model (void)
{
  GtkTreeStore  *treestore;
  GtkTreeIter    toplevel, child;

  treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_STRING);

  /* Append a top level row and leave it empty */
  gtk_tree_store_append(treestore, &toplevel, NULL);

  /* Append a second top level row, and fill it with some data */
  gtk_tree_store_append(treestore, &toplevel, NULL);
  gtk_tree_store_set(treestore, &toplevel,
                     COL_FIRST_NAME, "Joe",
                     COL_LAST_NAME, "Average",
                     -1);

  /* Append a child to the second top level row, and fill in some data */
  gtk_tree_store_append(treestore, &child, &toplevel);
  gtk_tree_store_set(treestore, &child,
                     COL_FIRST_NAME, "Jane",
                     COL_LAST_NAME, "Average",
                     -1);

  return GTK_TREE_MODEL(treestore);
}

GtkWidget *createMemoryDisplay(ControlUnit *unit) {
	GtkTreeViewColumn   *col;
	GtkCellRenderer     *renderer;
	GtkWidget           *view;
	GtkTreeModel        *model;

	view = gtk_tree_view_new();

	/* --- Column #1 --- */
	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, "First Name");
	/* pack tree view column into tree view */
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
	renderer = gtk_cell_renderer_text_new();
	/* pack cell renderer into tree view column */
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	/* set 'text' property of the cell renderer */
	g_object_set(renderer, "text", "Boooo!", NULL);


	/* --- Column #2 --- */
	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, "Last Name");
	/* pack tree view column into tree view */
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
	GtkCellRenderer *renderer2 = gtk_cell_renderer_text_new();
	/* pack cell renderer into tree view column */
	gtk_tree_view_column_pack_start(col, renderer2, TRUE);
  /* set 'cell-background' property of the cell renderer */
	g_object_set(renderer2,
				"cell-background", "Orange",
				"cell-background-set", TRUE,
				NULL);
	model = create_and_fill_model();
	gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
	g_object_unref(model); /* destroy model automatically with view */
	gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)),
								GTK_SELECTION_NONE);

	return view;
}












int	main (int argc, char **argv) {
	gtk_init (&argc, &argv);
	ControlUnit *unit = controlUnitConst(0, 0x5000);
	GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	
	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add (GTK_CONTAINER(window), box);
	
	GtkWidget *menuBar = createMenuBar(unit);
	gtk_box_pack_start (GTK_BOX(box), menuBar, FALSE, FALSE, 0);
	
	GtkWidget *view;
	view = createMemoryDisplay(unit);
	gtk_box_pack_start (GTK_BOX(box), view, TRUE, TRUE, 0);
	
	g_signal_connect (window, "delete_event", G_CALLBACK(gtk_main_quit), NULL);
	
	gtk_widget_show_all (window);
	gtk_main ();
	
	return 0;
}