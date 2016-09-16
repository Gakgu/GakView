#include <gtk/gtk.h>
#include <malloc.h>

#define UNUSED(x) (void)(x)

typedef struct GakviewImage
{
	GdkPixbuf *pixbuf;
	unsigned int width;
	unsigned int height;
    gpointer file;
    struct GakviewImage *next;
    struct GakviewImage *previous;
} GakviewImage;

typedef struct GakView
{
    GtkWidget *window;
	GtkBuilder *builder;
    GtkWidget *scroll_box;
    GtkWidget *view_box;
    GtkWidget *surface;
    GtkWidget *about;
    GakviewImage *image;
    unsigned int zoom_count;
} GakView;

// NEW FUNCTION
void gakview_main_init(GtkApplication *app, gpointer user_data);

void gakview_gmenu_create(GtkApplication *app);
void gakview_gmenu_open(GSimpleAction *action, GVariant *parameter, gpointer data);
void gakview_gmenu_mode_size_nomal(GSimpleAction *action, GVariant *parameter, gpointer data);
void gakview_gmenu_mode_size_auto(GSimpleAction *action, GVariant *parameter, gpointer data);
void gakview_gmenu_about(GSimpleAction *action, GVariant *parameter, gpointer data);
void gakview_gmenu_quit(GSimpleAction *action, GVariant *parameter, gpointer data);

GakviewImage* gakview_image_create(char *file);
void gakview_image_set(char *file, GakviewImage *image);
void gakview_image_zoom_auto(GtkWindow *widget, GdkEvent *event, gpointer data);
void gakview_image_zoom_in();
void gakview_image_zoom_out();
void gakview_image_next();
void gakview_image_previous();

void gakview_input_key(GtkWidget *widget, GdkEventKey *event, gpointer data);

enum
{
    NOMAL,
    AUTO
} GAKVIEW_SIZE_MODE;

enum
{
    NEAREST = GDK_INTERP_NEAREST,
    TILES = GDK_INTERP_TILES,
    BILINEAR = GDK_INTERP_BILINEAR,
    HYPER = GDK_INTERP_HYPER
} GAKVIEW_QUALITY_MODE;

GakView gakview;

// Declare variable
// TODO
GSList *file_list;

int main (int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("gakgu.gakview", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (gakview_main_init), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (G_OBJECT(app));
    return status;
}

void gakview_main_init (GtkApplication *app, gpointer user_data)
{
    UNUSED(user_data);

    /*-- Gtk Builder ---------------------------------------------------*/
    gakview.window = gtk_application_window_new (app);
    gakview.builder = gtk_builder_new_from_file("./ui.glade");
    gakview.scroll_box = GTK_WIDGET(gtk_builder_get_object(gakview.builder, "scroll_box"));
    gakview.view_box = GTK_WIDGET(gtk_builder_get_object(gakview.builder, "view_box"));
    gakview.surface = GTK_WIDGET(gtk_builder_get_object(gakview.builder, "image"));
    gakview.about = GTK_WIDGET(gtk_builder_get_object(gakview.builder, "about"));
	gakview.image = gakview_image_create("./images/failed.jpg");
    gakview.zoom_count = 100;
    /*-- Init Setting --------------------------------------------------*/
    // window
    gtk_container_add(GTK_CONTAINER(gakview.window), gakview.scroll_box);
    gtk_window_set_title (GTK_WINDOW (gakview.window), "GakView");
    gtk_window_set_default_size(
        GTK_WINDOW(gakview.window),
        gakview.image -> width + 5,
        gakview.image -> height + 5
    );
    GAKVIEW_SIZE_MODE = AUTO;
    GAKVIEW_QUALITY_MODE = HYPER;

    /*-- Gmenu ---------------------------------------------------------*/
    gakview_gmenu_create(app);

    /*-- Signal --------------------------------------------------------*/
    g_signal_connect(gakview.window, "configure-event", G_CALLBACK(gakview_image_zoom_auto), NULL);
    g_signal_connect(gakview.window, "key-press-event", G_CALLBACK(gakview_input_key), NULL);

    /*-- Finish --------------------------------------------------------*/
    gtk_widget_show_all (gakview.window);
    g_object_unref(G_OBJECT(gakview.builder));
}

// Auto resizing image
void gakview_image_zoom_auto(GtkWindow *widget, GdkEvent *event, gpointer data)
{
    UNUSED(widget);
    UNUSED(event);
    UNUSED(data);

    unsigned int view_box_width = gtk_widget_get_allocated_width(gakview.view_box);
    unsigned int view_box_height = gtk_widget_get_allocated_height(gakview.view_box);
    if(gakview.image -> width < view_box_width && gakview.image -> height < view_box_height)
    {
        gtk_image_set_from_pixbuf(GTK_IMAGE(gakview.surface), gakview.image -> pixbuf);
        return;
    }

    GdkPixbuf *tempbuf;
    if(gakview.image -> height * view_box_width / gakview.image -> width > view_box_height)
    {
        tempbuf = gdk_pixbuf_scale_simple(
            gakview.image -> pixbuf,
            gakview.image -> width * view_box_height / gakview.image -> height,
            view_box_height,
            GAKVIEW_QUALITY_MODE
        );
    }
    else
    {
        tempbuf = gdk_pixbuf_scale_simple(
            gakview.image -> pixbuf,
            view_box_width,
            gakview.image -> height * view_box_width / gakview.image -> width,
            GAKVIEW_QUALITY_MODE
        );
    }
    gtk_image_set_from_pixbuf(GTK_IMAGE(gakview.surface), tempbuf);

    // Next code is used to redraw for window.
    g_object_unref(G_OBJECT(tempbuf));
    gtk_widget_queue_draw(gakview.window);
}

// SubMenu "Open"
void gakview_gmenu_open(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    UNUSED(action);
    UNUSED(parameter);
    UNUSED(data);

    GtkWidget *dialog = gtk_file_chooser_dialog_new ("Open File",
    GTK_WINDOW(gakview.window),
    GTK_FILE_CHOOSER_ACTION_OPEN,
    ("_Open"),
    GTK_RESPONSE_ACCEPT,
    NULL);
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_pixbuf_formats(filter);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        if(!gdk_pixbuf_new_from_file(gtk_file_chooser_get_filename(chooser), NULL))
        {
            gtk_widget_destroy(dialog);
            return;
        }

        gakview_image_set(gtk_file_chooser_get_filename(chooser), gakview.image);

        // GSList
        if(file_list)
        {
            GSList *temp_list = file_list;
            while(temp_list)
            {
                g_free(temp_list -> data);
                temp_list = temp_list -> next;
            }
            g_slist_free(file_list);
        }

        gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), TRUE);
        gtk_file_chooser_select_all(GTK_FILE_CHOOSER(dialog));
        file_list = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(dialog));

        // TODO
        // GakviewImage
        GakviewImage *current_file = gakview.image;
        GSList *temp_gslist = file_list;
        while(temp_gslist)
        {
            current_file -> file = temp_gslist -> data;

            if(temp_gslist -> next)
            {
                GakviewImage *next_file = (GakviewImage*)malloc(sizeof(GakviewImage));
                current_file -> next = (GakviewImage*)next_file;
                next_file -> previous = current_file;
            }
            else
            {
                current_file -> next = gakview.image;
                gakview.image -> previous = current_file;
            }
            current_file = current_file -> next;
            temp_gslist = temp_gslist -> next;
        }

        // Resizing image
        if(GAKVIEW_SIZE_MODE == NOMAL)
        gtk_image_set_from_pixbuf(GTK_IMAGE(gakview.surface), gakview.image -> pixbuf);
        else if (GAKVIEW_SIZE_MODE == AUTO)
        gakview_image_zoom_auto(NULL, NULL, NULL);
    }
    gtk_widget_destroy(dialog);
}

void gakview_gmenu_about (GSimpleAction *action, GVariant *parameter, gpointer data)
{
    UNUSED(action);
    UNUSED(parameter);
    UNUSED(data);

    gtk_dialog_run(GTK_DIALOG(gakview.about));
    gtk_widget_hide(gakview.about);
}

// SubMenu "Quit"
void gakview_gmenu_quit (GSimpleAction *action, GVariant *parameter, gpointer data)
{
    UNUSED(action);
    UNUSED(parameter);
    UNUSED(data);

    gtk_widget_destroy(gakview.window);
}

void gakview_gmenu_mode_size_nomal (GSimpleAction *action, GVariant *parameter, gpointer data)
{
    UNUSED(action);
    UNUSED(parameter);
    UNUSED(data);

    GAKVIEW_SIZE_MODE = NOMAL;
    gtk_image_set_from_pixbuf(GTK_IMAGE(gakview.surface), gakview.image -> pixbuf);
    gakview.zoom_count = 100;
}

void gakview_gmenu_mode_size_auto (GSimpleAction *action, GVariant *parameter, gpointer data)
{
    UNUSED(action);
    UNUSED(parameter);
    UNUSED(data);

    if(GAKVIEW_SIZE_MODE != AUTO)
    GAKVIEW_SIZE_MODE = AUTO;
    gakview_image_zoom_auto(NULL, NULL, NULL);
}

void gakview_input_key(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    UNUSED(widget);
    UNUSED(data);

    switch(event -> keyval)
    {
        case GDK_KEY_Escape:
        gtk_widget_destroy(gakview.window);
        break;
        case GDK_KEY_F1:
        gakview_gmenu_about(NULL, NULL, NULL);
        break;
        case GDK_KEY_F2:
        gakview_gmenu_mode_size_nomal(NULL, NULL, NULL);
        break;
        case GDK_KEY_F3:
        gakview_gmenu_mode_size_auto(NULL, NULL, NULL);
        break;
        case GDK_KEY_Left:
        gakview_image_previous();
        break;
        case GDK_KEY_Right:
        gakview_image_next();
        break;
        case GDK_KEY_Down:
        gakview_image_zoom_out();
        break;
        case GDK_KEY_Up:
        gakview_image_zoom_in();
        break;
    }
}

void gakview_image_zoom_in()
{
    if(GAKVIEW_SIZE_MODE != NOMAL || gakview.zoom_count >= 500)
    return;
    gakview.zoom_count += 5;

    GdkPixbuf *tempbuf = gdk_pixbuf_scale_simple(
        gakview.image -> pixbuf,
        gdk_pixbuf_get_width(gakview.image -> pixbuf) * gakview.zoom_count / 100,
        gdk_pixbuf_get_height(gakview.image -> pixbuf) * gakview.zoom_count / 100,
        GAKVIEW_QUALITY_MODE);
        gtk_image_set_from_pixbuf(GTK_IMAGE(gakview.surface), tempbuf
    );

    g_object_unref(G_OBJECT(tempbuf));
}

void gakview_image_zoom_out()
{
    if(GAKVIEW_SIZE_MODE != NOMAL || gakview.zoom_count <= 10)
    return;

    gakview.zoom_count -= 5;

    GdkPixbuf *tempbuf = gdk_pixbuf_scale_simple(
        gakview.image -> pixbuf,
        gdk_pixbuf_get_width(gakview.image -> pixbuf) * gakview.zoom_count / 100,
        gdk_pixbuf_get_height(gakview.image -> pixbuf) * gakview.zoom_count / 100,
        GAKVIEW_QUALITY_MODE
    );
    gtk_image_set_from_pixbuf(GTK_IMAGE(gakview.surface), tempbuf);

    g_object_unref(G_OBJECT(tempbuf));
}

void gakview_image_next()
{
    gakview.image = gakview.image -> next;
    gakview_image_set(gakview.image -> file, gakview.image);
}

void gakview_image_previous()
{
    gakview.image = gakview.image -> previous;
    gakview_image_set((char*)gakview.image -> file, gakview.image);
}

GakviewImage* gakview_image_create(char *file)
{
	GakviewImage *temp_image = (GakviewImage*)malloc(sizeof(GakviewImage));
	gakview_image_set(file, temp_image);
	return temp_image;
}

void gakview_image_set(char *file, GakviewImage *image)
{
    gtk_window_set_title(GTK_WINDOW(gakview.window), file);
    image -> file = (gpointer)file;
    image -> pixbuf = gdk_pixbuf_new_from_file(file, NULL);
    image -> width = gdk_pixbuf_get_width(image -> pixbuf);
    image -> height = gdk_pixbuf_get_height(image -> pixbuf);
    if(GAKVIEW_SIZE_MODE == NOMAL);
	// zoom(0);
    else if(GAKVIEW_SIZE_MODE == AUTO)
	gakview_gmenu_mode_size_auto(NULL, NULL, NULL);
}

void gakview_gmenu_create(GtkApplication *app)
{
    GMenu *menu = g_menu_new();
    GMenu *mode = g_menu_new();
    g_menu_append(menu, "Open", "app.Open");
    g_menu_append_submenu(menu, "View Mode", G_MENU_MODEL(mode));
    g_menu_append(menu, "About", "app.About");
    g_menu_append(menu, "Quit", "app.Quit");

    g_menu_append(mode, "Nomal Mode", "app.Nomal");
    g_menu_append(mode, "Auto Mode", "app.Auto");

    gtk_application_set_app_menu(app, G_MENU_MODEL(menu));

    const GActionEntry actions[] = {
        {"Open", gakview_gmenu_open, NULL, NULL, NULL, {0, 0, 0}},
        {"Nomal", gakview_gmenu_mode_size_nomal, NULL, NULL, NULL, {0, 0, 0}},
        {"Auto", gakview_gmenu_mode_size_auto, NULL, NULL, NULL, {0, 0, 0}},
        {"About", gakview_gmenu_about, NULL, NULL, NULL, {0, 0, 0}},
        {"Quit", gakview_gmenu_quit, NULL, NULL, NULL, {0, 0, 0}}
    };
    g_action_map_add_action_entries(G_ACTION_MAP(app), actions, G_N_ELEMENTS(actions), app);
    g_object_unref(G_OBJECT(menu));
    g_object_unref(G_OBJECT(mode));
}
