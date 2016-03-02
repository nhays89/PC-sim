#include <gtk/gtk.h>
#include <stdio.h>
#include "controlunit.h"

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

int	main (int argc, char **argv) {
	gtk_init (&argc, &argv);
	
	ControlUnit *unit = controlUnitConst(0, 0x5000);
	
	GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	
	GtkWidget *box = gtk_box_new(FALSE, 0);
	gtk_container_add (GTK_CONTAINER(window), box);
	GtkWidget *menuBar = gtk_menu_bar_new();
	gtk_box_pack_start (GTK_BOX(box), menuBar, FALSE, FALSE, 0);
	
	GtkWidget *fileItem = gtk_menu_item_new_with_label("File");
	gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), fileItem);
	
	GtkWidget *fileMenu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileItem), fileMenu);
	
	GtkWidget *openItem = gtk_menu_item_new_with_label("Open");
	GtkWidget *closeItem = gtk_menu_item_new_with_label("Close");
	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), openItem);
	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), closeItem);
	
	g_signal_connect (G_OBJECT(closeItem), "activate", G_CALLBACK(gtk_main_quit), (gpointer) NULL);
	g_signal_connect (window, "delete_event", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect (G_OBJECT(openItem), "activate", G_CALLBACK(chooseFile), (gpointer) unit);
	
	gtk_widget_show_all (window);
	gtk_main ();
	
	return 0;
}