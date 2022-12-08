#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include "../preproc/preproc.h"
#include "../preproc/tools.h"
#include "../preproc/rotate.h"
#define UNUSED(x) (void)(x)

//Gtk variable declaration
GtkBuilder      *builder;
GtkWidget       *window;

GtkWidget 		*SFixed;
GtkWidget		*NNTFixed;
GtkWidget		*ImageFixed;
GtkWidget		*SoFixed;

GtkWidget		*button_load_file;
GtkWidget		*button_neural_network;
GtkWidget		*button_solve;
GtkWidget		*button_save_image;
GtkWidget		*button_rotate_left;
GtkWidget		*button_rotate_right;
GtkWidget		*button_rotate_auto;
GtkWidget		*button_settings;
GtkWidget		*button_normal_menu;
GtkWidget		*button_homepage;

GtkWidget		*switch_Dmode;

GtkWidget		*progress_bar;

GtkWidget		*image1;
GtkWidget		*image2;

GtkStack		*big_stack;
GtkStack		*Stack;

GtkWidget		*main_grid;
GtkWidget		*normal_grid;
GtkWidget		*side_menu;


gchar			*filename;


//Pixbuf declaration for the rescale of the image displayed
Uint32                  src_format;
Uint32                  dst_format;
int                     rowstride;
gboolean                has_alpha;
SDL_Surface*            surface = NULL;
guchar                  *pixels;

GdkPixbuf		*pix;
GdkPixbuf		*pixIn;
GdkPixbuf		*pixOut;

static void display_surface()
{
    //If an image is already display, we delete it
    if (image1)
        gtk_container_remove (GTK_CONTAINER (ImageFixed), image1);

    src_format = surface->format->format;

    has_alpha = SDL_ISPIXELFORMAT_ALPHA(src_format);
    if (has_alpha) {
        dst_format = SDL_PIXELFORMAT_RGBA32;
    }
    else {
        dst_format = SDL_PIXELFORMAT_RGB24;
    }
    // create pixbuf                                                            
    pixIn = gdk_pixbuf_new (GDK_COLORSPACE_RGB, has_alpha, 8,
            surface->w, surface->h);
    rowstride = gdk_pixbuf_get_rowstride (pixIn);
    pixels = gdk_pixbuf_get_pixels (pixIn);

    // copy pixels                                                              
    SDL_LockSurface(surface);
    SDL_ConvertPixels (surface->w, surface->h, src_format,
            surface->pixels, surface->pitch,
            dst_format,
            pixels,
            rowstride);
    SDL_UnlockSurface(surface);

    pix = gdk_pixbuf_scale_simple (pixIn, 415, 405, GDK_INTERP_NEAREST);
    // create GtkImage from pixbuf                                              
    image1 = gtk_image_new_from_pixbuf (pix);


    gtk_container_add(GTK_CONTAINER (ImageFixed), image1);
    gtk_fixed_move (GTK_FIXED (ImageFixed), image1, 39, 10);
    //We show the image to the user
    gtk_widget_show(image1);
}

//File Loader function
static void load_image_from_chooser(GtkFileChooser *chooser)
{
    filename = gtk_file_chooser_get_filename (chooser);
    // - Create a surface from the image.
    surface = load_image(filename);

    display_surface();
}

//Save function
static void save_image(gpointer window)
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
        pixOut = gdk_pixbuf_new_from_file (filename, NULL);
        gdk_pixbuf_save(pixOut, filename, "png", NULL, NULL);
        g_free (filename);
    }
    gtk_widget_destroy(dialog);
}

void changeStackVisibleS (GtkButton* a, gpointer user_data)
{
    UNUSED(a);
    gtk_stack_set_visible_child(user_data, SFixed);
}

void changeStackVisibleM (GtkButton *a, gpointer user_data)
{
    UNUSED(a);
    gtk_stack_set_visible_child(user_data, NNTFixed);
}

void changeStackVisibleN (GtkButton *a, gpointer user_data)
{
    UNUSED(a);
    gtk_stack_set_visible_child(user_data, normal_grid);
}

void changeStackVisibleSo (GtkButton *a, gpointer user_data)
{
    UNUSED(a);
    gtk_stack_set_visible_child(user_data, SoFixed);
}

void changeBigStackVisible (GtkButton *a, gpointer user_data)
{
    UNUSED(a);
    gtk_stack_set_visible_child(user_data, main_grid); 
}

void ui_solve(GtkButton *a, gpointer user_data)
{
    UNUSED(a);
    UNUSED(user_data);
    if (surface)
        surface = preproc(surface);
    display_surface();
    ///gtk_stack_set_visible_child(user_data, _grid); 
}

void ui_rotate_right(GtkButton *a, gpointer user_data)
{
    UNUSED(a);
    UNUSED(user_data);
    if (surface)
    {
        surface = rotateAny(surface, -90, 0, 0);
        display_surface();
    }
}

void ui_rotate_left(GtkButton *a, gpointer user_data)
{
    UNUSED(a);
    UNUSED(user_data);
    if (surface)
    {
        surface = rotateAny(surface, 90, 0, 0);
        display_surface();
    }
}

void change_mode(GtkSwitch *a, gpointer user_data)
{
	UNUSED(user_data);
	if (gtk_switch_get_active(a) == 1)
	{
		GtkCssProvider *css2 = gtk_css_provider_new();
		gtk_css_provider_load_from_path(css2, "resources/DMode.css", NULL);
		gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(css2),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);
		gtk_container_remove (GTK_CONTAINER (side_menu), image2);
		image2 = gtk_image_new_from_file ("resources/sigmavisionW_logo.png");
   		gtk_container_add (GTK_CONTAINER (side_menu), image2);
		gtk_widget_show(image2);
	}
	else
	{
		GtkCssProvider * css1 = gtk_css_provider_new();
		gtk_css_provider_load_from_path(css1, "resources/LMode.css", NULL);
		gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(css1),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);
		gtk_container_remove (GTK_CONTAINER (side_menu), image2);
		image2 = gtk_image_new_from_file ("resources/sigmavision_logo.png");
   		gtk_container_add (GTK_CONTAINER (side_menu), image2);
		gtk_widget_show(image2);
	}

}

int ui (int argc, char *argv[])
{
    gtk_init (&argc, &argv);
    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "resources/OCR.glade", NULL);

    //Window association
    window = GTK_WIDGET (gtk_builder_get_object (builder, "MainWindow"));

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //Widget association
    image1 = GTK_WIDGET (gtk_builder_get_object (builder, "image1"));
	image2 = GTK_WIDGET (gtk_builder_get_object (builder, "image2"));

    big_stack = GTK_STACK (gtk_builder_get_object (builder, "MainStack"));
    Stack = GTK_STACK (gtk_builder_get_object (builder, "ImageStack"));
    main_grid = GTK_WIDGET (gtk_builder_get_object (builder, "MainGrid"));
    normal_grid = GTK_WIDGET (gtk_builder_get_object (builder, "NormalGrid"));
    NNTFixed = GTK_WIDGET (gtk_builder_get_object (builder, "NNTFixed"));
    SFixed = GTK_WIDGET (gtk_builder_get_object	(builder, "SettingFixed"));
    ImageFixed = GTK_WIDGET (gtk_builder_get_object (builder, "ImageFixed"));
    SoFixed = GTK_WIDGET (gtk_builder_get_object (builder, "SolvedFixed"));
	side_menu = GTK_WIDGET (gtk_builder_get_object (builder, "Side_Menu"));

    button_homepage = GTK_WIDGET (gtk_builder_get_object (builder, "ButtonHomepage"));

    button_load_file = GTK_WIDGET (gtk_builder_get_object (builder, "load_file"));
    button_solve = GTK_WIDGET (gtk_builder_get_object (builder, "solve"));
    button_save_image = GTK_WIDGET (gtk_builder_get_object (builder, "save_result"));
    button_normal_menu = GTK_WIDGET (gtk_builder_get_object (builder, "NormalMenu"));
    button_neural_network = GTK_WIDGET (gtk_builder_get_object (builder, "neural_network"));
    button_settings = GTK_WIDGET (gtk_builder_get_object (builder, "settings"));

    button_rotate_right = GTK_WIDGET (gtk_builder_get_object (builder, "rotate_right"));
    button_rotate_left = GTK_WIDGET (gtk_builder_get_object (builder, "rotate_left"));
    button_rotate_auto = GTK_WIDGET (gtk_builder_get_object (builder, "auto_rotate"));

    progress_bar = GTK_WIDGET (gtk_builder_get_object (builder, "progress"));

	switch_Dmode = GTK_WIDGET (gtk_builder_get_object (builder, "switchDmode"));

    g_object_unref (G_OBJECT (builder));

    //Button Action
    g_signal_connect(button_homepage, "clicked", G_CALLBACK(changeBigStackVisible), big_stack);
    g_signal_connect(button_settings, "clicked", G_CALLBACK(changeStackVisibleS), Stack);
    g_signal_connect(button_neural_network, "clicked", G_CALLBACK(changeStackVisibleM), Stack);
    g_signal_connect(button_normal_menu, "clicked", G_CALLBACK(changeStackVisibleN), Stack);
    g_signal_connect(button_solve, "clicked", G_CALLBACK(ui_solve), Stack);

    g_signal_connect(button_rotate_right, "clicked", G_CALLBACK(ui_rotate_right), Stack);
    g_signal_connect(button_rotate_left, "clicked", G_CALLBACK(ui_rotate_left), Stack);
	g_signal_connect(switch_Dmode, "state-set", G_CALLBACK(change_mode), NULL);

    g_signal_connect(button_load_file, "selection_changed", 
            G_CALLBACK(load_image_from_chooser), NULL);

    g_signal_connect(button_save_image, "clicked", G_CALLBACK (save_image), NULL);


    gtk_widget_show (window);

    image1 = NULL;
    gtk_main ();

    return (0);
}
