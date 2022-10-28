#include <gtk/gtk.h>

//Gtk variable declaration
GtkBuilder      *builder;
GtkWidget       *window1;
GtkWidget		*windowNNT;
GtkWidget		*windowSolved;
GtkWidget		*fixed1;
GtkWidget		*button_load_image;
GtkWidget		*button_neural_training;
GtkWidget		*button_solve;
GtkWidget		*button_save_image;
GtkWidget		*button_rotate_left;
GtkWidget		*button_rotate_right;
GtkWidget		*button_save_result;
GtkWidget		*button_quit_solved_menu;
GtkWidget		*button_quit_neural_training;
GtkWidget		*image1;

//Pixbuf declaration for the rescale of the image displayed
GdkPixbuf		*pix;
GdkPixbuf		*pixIn;
GdkPixbuf		*pixOut;



//File Loader function
static void load_image(GtkWidget* button, gpointer window)
{
    GtkWidget *dialog;

    dialog = gtk_file_chooser_dialog_new("Open File", window, GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_OK, NULL);
    gint resp = gtk_dialog_run(GTK_DIALOG(dialog));
    if(resp == GTK_RESPONSE_OK)
    {
			//If an image is allready display, we delete it
			if (image1)
				gtk_container_remove (GTK_CONTAINER (fixed1), image1);

			//Rescale the image for the display
			pixIn = gdk_pixbuf_new_from_file (gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)), NULL);
			pix = gdk_pixbuf_scale_simple (pixIn, 415, 405, GDK_INTERP_NEAREST);
			image1 = gtk_image_new_from_pixbuf (pix);
			gtk_container_add(GTK_CONTAINER (fixed1), image1);
			
			//We show the image to the user
			gtk_widget_show(image1);

	}
    gtk_widget_destroy(dialog);
}

//Save function
static void save_image(GtkWidget* button, gpointer window)
{
	GtkWidget *dialog;
	GtkFileChooser *chooser;

	dialog = gtk_file_chooser_dialog_new("Save File", window, GTK_FILE_CHOOSER_ACTION_SAVE, "Cancel", GTK_RESPONSE_CANCEL, "Save", GTK_RESPONSE_OK, NULL);

	chooser = GTK_FILE_CHOOSER (dialog);
	gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

	gtk_file_chooser_set_current_name (chooser, "test");

	gint resp = gtk_dialog_run(GTK_DIALOG(dialog));
	if(resp == GTK_RESPONSE_OK)
	{
		char *filename;

		filename = gtk_file_chooser_get_filename (chooser);
		//here the function we have to call to save the image
		pixOut = gdk_pixbuf_new_from_file ("resources/epita_logo.png", NULL);
		gdk_pixbuf_save(pixOut, filename, "png", NULL, NULL);
		g_print(filename);
		g_free (filename);
	}
	gtk_widget_destroy(dialog);
}
	
//All the window open/close function
static void show_neural_training(GtkBuilder* builder)
{
	gtk_widget_show (windowNNT);
}	

static void hide_neural_training(GtkBuilder* builder)
{
	gtk_widget_hide (windowNNT);
}

static void show_solve_menu(GtkBuilder* builder)
{
	gtk_widget_show (windowSolved);
}

static void hide_solve_menu(GtkBuilder* builder)
{
	gtk_widget_hide (windowSolved);
}

int main (int argc, char *argv[])
{
    gtk_init (&argc, &argv);
    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "resources/OCR test.glade", NULL);

	//Window association
	window1 = GTK_WIDGET (gtk_builder_get_object (builder, "Main menu"));
	windowNNT = GTK_WIDGET (gtk_builder_get_object (builder, "NNT Menu"));
	windowSolved = GTK_WIDGET (gtk_builder_get_object (builder, "Solved Menu"));

    gtk_builder_connect_signals (builder, NULL);

	g_signal_connect(window1, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(windowSolved, "destroy", G_CALLBACK(hide_solve_menu), NULL);

	//Widget association
	button_load_image = GTK_WIDGET (gtk_builder_get_object (builder, "load_image"));
	fixed1 = GTK_WIDGET (gtk_builder_get_object (builder, "fixed1"));
	button_neural_training = GTK_WIDGET (gtk_builder_get_object (builder, "neural_training"));
	button_solve = GTK_WIDGET (gtk_builder_get_object (builder, "solve"));
	button_rotate_left = GTK_WIDGET (gtk_builder_get_object (builder, "rotate_left"));
	button_rotate_right = GTK_WIDGET (gtk_builder_get_object (builder, "rotate_right"));
	button_save_result = GTK_WIDGET (gtk_builder_get_object (builder, "save_result"));
	button_quit_solved_menu = GTK_WIDGET (gtk_builder_get_object (builder, "quit SM"));
	button_quit_neural_training = GTK_WIDGET (gtk_builder_get_object (builder, "quit NNT"));
	button_save_image = GTK_WIDGET (gtk_builder_get_object (builder, "save_result"));

    g_object_unref (G_OBJECT (builder));


	//Button Action
	g_signal_connect(button_load_image, "clicked", G_CALLBACK(load_image), window1);
    g_signal_connect(button_neural_training, "clicked", G_CALLBACK(show_neural_training), window1);
	g_signal_connect(button_quit_neural_training, "clicked", G_CALLBACK(hide_neural_training), windowNNT);
	g_signal_connect(button_solve, "clicked", G_CALLBACK(show_solve_menu), window1);
	g_signal_connect(button_quit_solved_menu, "clicked", G_CALLBACK(hide_solve_menu), windowSolved);
	g_signal_connect(button_save_image, "clicked", G_CALLBACK (save_image), windowSolved);

	//Change the Color of the Background
	//GdkColor color;
	//color.red = 0xcccc;
	//color.blue = 0xdd00;
	//color.green = 0xcccc;
	//gtk_widget_modify_bg(GTK_WIDGET(window1), GTK_STATE_NORMAL, &color);
	
	gtk_widget_show (window1);

	image1 = NULL;
    gtk_main ();

    return (0);
}
