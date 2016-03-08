/**
 * @file assemblerGUI.h
 * @author nnebp
 *
 * @brief GTK gui for the assembler allows a user to edit, save, open
 * and assemble a text file visually. Consists of a text editing
 * component as well as a menu for file functions.
 */

#ifndef ASSEMBLER_GUI_H_
#define ASSEMBLER_GUI_H_

#include <gtk/gtk.h>
#include <string.h>
#include <glib.h>
#include <glib/gi18n.h>

#include "assembler.h"

/**
 * @brief Clears all text from a GtkTextBuffer.
 *
 * @param widget pointer to calling widget.
 * @param user_data pointer to a GtkTextBuffer.
 */
void clearBuffer(GtkWidget* widget, gpointer *user_data)
{
    GtkTextIter start, end;

    GtkTextBuffer *textbuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(user_data));

    gtk_text_buffer_get_start_iter(textbuffer, &start);
    gtk_text_buffer_get_end_iter(textbuffer, &end);

    gtk_text_buffer_delete(textbuffer, &start, &end);
}

/**
 * @brief Places text from a text file into a GtkTextFuffer
 *
 * @param GtkTextBuffer pointer to textbuffer.
 * @param filename string representing text file name.
 */
void writeFileToBuffer(GtkTextBuffer *textbuffer, char *filename)
{
    FILE *file = fopen(filename, "r");
    char line[256];
    GtkTextIter iter;
    
    gtk_text_buffer_get_end_iter (textbuffer, &iter);

    while (fgets(line, sizeof(line), file))
    {
        gtk_text_buffer_insert(textbuffer, &iter, line, strlen(line));
        gtk_text_buffer_get_end_iter (textbuffer, &iter);
    }

    fclose(file);
}

/**
 * @brief Checks if a line is a valid piece of assembly code.
 * I.e. not a comment or blank line. Lines are checked by looking
 * at their first characters;
 *
 * @param line string representing a line.
 * @return 0 if invalid 1 if valid.
 */
int isValidLine(char *line)
{
    int result = 1;

    if ( line[0] == ' ' 
        || line[0] == ';'
        || line[0] == '\n'
        || line[0] == '\0' )
    {
        result = 0;
    }

    return result;
}

/**
 * @brief Saves the text in the text buffer into a textfile.
 *
 * @param widget pointer to calling widget.
 * @param user_data pointer to GtkTextView.
 */
void saveActivate(GtkWidget* widget, gpointer *user_data) 
{
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    GtkFileChooser *chooser;
    gint choice;
    GtkTextBuffer *textbuffer;
    char *filename;
    FILE *file;

    GtkTextIter start, end;

    /* assign textbuffer and save file */
    textbuffer =  gtk_text_view_get_buffer (GTK_TEXT_VIEW(user_data));

    dialog = gtk_file_chooser_dialog_new ("Save File",
                                          NULL,
                                          action,
                                          _("_Cancel"),
                                          GTK_RESPONSE_CANCEL,
                                          _("_Save"),
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);
    chooser = GTK_FILE_CHOOSER (dialog);
    gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

    gtk_file_chooser_set_current_name (chooser,
                                     _("Untitled document"));


    choice = gtk_dialog_run (GTK_DIALOG (dialog));
    if (choice == GTK_RESPONSE_ACCEPT)
    {
        filename = gtk_file_chooser_get_filename (chooser);
        file = fopen(filename, "w");

        if (file == NULL)
        {
            printf("error saving file\n");
            return;
        }

        //get the text in the buffer and save to file
        gtk_text_buffer_get_start_iter(textbuffer, &start);
        gtk_text_buffer_get_end_iter(textbuffer, &end);
        
        fputs(gtk_text_buffer_get_text(textbuffer, 
                                       &start,
                                       &end,
                                       FALSE), file);
                                
        fclose(file);
    }

    gtk_widget_destroy (dialog);
}

/**
 * @brief Opens a text file and writes the output to a text buffer.
 *
 * @param widget pointer to calling widget.
 * @param user_data pointer to GtkTextView.
 * @see writeFileToBuffer
 */
void openActivate(GtkWidget* widget, gpointer *user_data) 
{
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint choice;
    GtkTextBuffer *textbuffer;
    char *filename;

    /* assign textbuffer and open file */
    textbuffer =  gtk_text_view_get_buffer (GTK_TEXT_VIEW(user_data));

    dialog = gtk_file_chooser_dialog_new ("Open File",
                                          NULL,
                                          action,
                                          _("_Cancel"),
                                          GTK_RESPONSE_CANCEL,
                                          _("_Open"),
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);

    choice = gtk_dialog_run (GTK_DIALOG (dialog));

    /* if the user opens a file we write the contents to the buffer*/
    if (choice == GTK_RESPONSE_ACCEPT)
    {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);

        /* clear buffer and write */
        clearBuffer(NULL, user_data);
        writeFileToBuffer(textbuffer, filename);
    }

    gtk_widget_destroy (dialog);

}

/**
 * @brief Reads the assembley code in a textbuffer then writes
 * the machinecode translation to a binary file.
 *
 * @param textBuffer pointer to GtkTextBuffer object.
 * @param filename string represeting the name of the output file.
 */
void assembleBuffer(GtkTextBuffer *textbuffer, char *filename)
{
    GtkTextIter start, end;
    char* tempBuffer;
    int lineCount = 0;
    int binline = 0;
    int i = 0;

    FILE *file = fopen(filename, "wb");
    if(file == NULL)
    {
        printf("error opening file\n");
        return;
    }

    /* iterate through the text buffer and write the assembled lines */
    gtk_text_buffer_get_start_iter(textbuffer, &start);
    gtk_text_buffer_get_end_iter(textbuffer, &end);
    
    lineCount = gtk_text_buffer_get_line_count(textbuffer);

    for (i = 0; i < lineCount; i++)
    {
        //set itrs to begin of line
        gtk_text_iter_set_line (&start, i);
        gtk_text_iter_set_line (&end, i);
        //move end itr to the end of the line
        gtk_text_iter_forward_to_line_end(&end);
        tempBuffer = gtk_text_iter_get_text(&start, &end);

        if (isValidLine(tempBuffer))
        {
            //for debugging
            //printf("%d !!! %d\n", createMachineCode(tempBuffer), i);//TODO remove
            binline = createMachineCode(tempBuffer);
            fwrite(&binline, sizeof(int), 1, file);
        }
    }

    fclose(file);
}

/**
 * @brief Displays a file dialog for the user to select the output file
 * for assembley of the entered code.
 *
 * @param widget pointer to calling widget. 
 * @param user_data pointer to GtkTextView object. 
 * @see assembleBuffer
 */
void assembleActivate(GtkWidget* widget, gpointer *user_data) 
{

    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    GtkFileChooser *chooser;
    gint choice;
    GtkTextBuffer *textbuffer;
    char *filename;

    /* assign textbuffer */
    textbuffer =  gtk_text_view_get_buffer (GTK_TEXT_VIEW(user_data));

    dialog = gtk_file_chooser_dialog_new ("Assemble File",
                                          NULL,
                                          action,
                                          _("_Cancel"),
                                          GTK_RESPONSE_CANCEL,
                                          _("_Assemble"),
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);
    chooser = GTK_FILE_CHOOSER (dialog);
    gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

    gtk_file_chooser_set_current_name (chooser,
                                     _("Untitled"));


    choice = gtk_dialog_run (GTK_DIALOG (dialog));
    if (choice == GTK_RESPONSE_ACCEPT)
    {
        filename = gtk_file_chooser_get_filename (chooser);

        /* assemble the file */
        assembleBuffer(textbuffer, filename);
    }

    gtk_widget_destroy (dialog);
}

/**
 * @brief Creates and displays the GTK gui.
 */
void startGTK()
{

    GtkBuilder *builder;
    GtkWidget  *window;
    GError     *error = NULL;
    GObject    *textview;

    //init GTK
    gtk_init(NULL, NULL);

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();

    if( ! gtk_builder_add_from_file( builder, "GUIlayout.glade", &error ) )
    {
        g_warning( "%s", error->message );
        g_free( error );
        return;
    }

    /* Get pointers from UI */
    window = GTK_WIDGET( gtk_builder_get_object( builder, "window1" ) );
    textview = gtk_builder_get_object(builder, "textview1");

    /* min window size */
    gtk_window_set_default_size (GTK_WINDOW (window), 400, 300);

    /* Connect Signals */
    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    g_signal_connect ( gtk_builder_get_object( builder, "menuitem_quit" ),
                        "activate", 
                        G_CALLBACK (gtk_main_quit), 
                        NULL);
    g_signal_connect ( gtk_builder_get_object( builder, "menuitem_open" ),
                        "activate", 
                        G_CALLBACK (openActivate), 
                        textview);
    g_signal_connect ( gtk_builder_get_object( builder, "menuitem_new" ),
                        "activate", 
                        G_CALLBACK (clearBuffer), 
                        textview);
    g_signal_connect ( gtk_builder_get_object( builder, "menuitem_save" ),
                        "activate", 
                        G_CALLBACK (saveActivate), 
                        textview);
    g_signal_connect ( gtk_builder_get_object( builder, "menuitem_assemble" ),
                        "activate", 
                        G_CALLBACK (assembleActivate), 
                        textview);
    /* Destroy builder*/
    g_object_unref( G_OBJECT( builder ) );

    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show( window );

    /* Start main loop */
    gtk_main();

}

#endif
