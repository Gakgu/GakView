#include <gtk/gtk.h>
#include <malloc.h>

#define UNUSED(x) (void)(x)

// NEW FUNCTION
static void gakview_init(GtkApplication *app, gpointer user_data);

void gakview_gmenu_create(GtkApplication *app);
void gakview_gmenu_open(GSimpleAction *action, GVariant *parameter, gpointer data);
void gakview_gmenu_mode_size_nomal(GSimpleAction *action, GVariant *parameter, gpointer data);
void gakview_gmenu_mode_size_auto(GSimpleAction *action, GVariant *parameter, gpointer data);
void gakview_gmenu_about(GSimpleAction *action, GVariant *parameter, gpointer data);
void gakview_gmenu_quit(GSimpleAction *action, GVariant *parameter, gpointer data);

void gakview_image_zoom_auto(GtkWindow *widget, GdkEvent *event, gpointer data);
void gakview_image_zoom_in();
void gakview_image_zoom_out();
void gakview_image_next();
void gakview_image_previous();

void gakview_input_key(GtkWidget *widget, GdkEventKey *event, gpointer data);

// FIXME OLD FUNCTION
static void change_current_file(char *file_path);

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

typedef struct GakviewFile
{
	GdkPixbuf *pixbuf;
	unsigned int width;
	unsigned int height;
    gpointer path;
    struct GakviewFile *next;
    struct GakviewFile *previous;
} GakviewFile;

typedef struct GakView
{
    GtkWidget *window;
    GtkWidget *scroll_box;
    GtkWidget *view_box;
    GtkWidget *image;
    GtkWidget *about;
    GakviewFile *file;
    unsigned int zoom_count;
} GakView;

GakView _gakview;
GakView *gakview = &_gakview;
GakviewFile _file;

// Declare variable
GSList *file_list;

int main (int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("gakgu.gakview", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (gakview_init), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (G_OBJECT(app));
    return status;
}

static void gakview_init (GtkApplication *app, gpointer user_data)
{
    UNUSED(user_data);

    /*-- Gtk Builder ---------------------------------------------------*/
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "./ui.glade", NULL);

    gakview -> scroll_box = GTK_WIDGET(gtk_builder_get_object(builder, "scroll_box"));
    gakview -> view_box = GTK_WIDGET(gtk_builder_get_object(builder, "view_box"));
    gakview -> image = GTK_WIDGET(gtk_builder_get_object(builder, "image"));
    gakview -> about = GTK_WIDGET(gtk_builder_get_object(builder, "about"));

    /*-- Init Setting --------------------------------------------------*/
gakview -> file = &_file;
    // window
    gakview -> window = gtk_application_window_new (app);
    change_current_file("./images/failed.jpg");
    gtk_container_add(GTK_CONTAINER(gakview -> window), gakview -> scroll_box);
    gtk_window_set_title (GTK_WINDOW (gakview -> window), "GakView");
    gtk_window_set_default_size(
        GTK_WINDOW(gakview -> window),
        gdk_pixbuf_get_width(gakview -> file -> pixbuf) + 5,
        gdk_pixbuf_get_height(gakview -> file -> pixbuf) + 5
    );
    GAKVIEW_SIZE_MODE = AUTO;

    // zoom
    gakview -> zoom_count = 100;

    // pixbuf
    GAKVIEW_QUALITY_MODE = HYPER;

    /*-- Gmenu ---------------------------------------------------------*/
    gakview_gmenu_create(app);

    /*-- Signal --------------------------------------------------------*/
    g_signal_connect(gakview -> window, "configure-event", G_CALLBACK(gakview_image_zoom_auto), NULL);
    g_signal_connect(gakview -> window, "key-press-event", G_CALLBACK(gakview_input_key), NULL);

    /*-- Finish --------------------------------------------------------*/
    gtk_widget_show_all (gakview -> window);
    g_object_unref(G_OBJECT(builder));
}

// Auto resizing image
void gakview_image_zoom_auto(GtkWindow *widget, GdkEvent *event, gpointer data)
{
    UNUSED(widget);
    UNUSED(event);
    UNUSED(data);

    unsigned int view_box_width = gtk_widget_get_allocated_width(gakview -> view_box);
    unsigned int view_box_height = gtk_widget_get_allocated_height(gakview -> view_box);
    if(gakview -> file -> width < view_box_width && gakview -> file -> height < view_box_height)
    {
        gtk_image_set_from_pixbuf(GTK_IMAGE(gakview -> image), gakview -> file -> pixbuf);
        return;
    }

    GdkPixbuf *tempbuf;
    if(gakview -> file -> height * view_box_width / gakview -> file -> width > view_box_height)
    {
        tempbuf = gdk_pixbuf_scale_simple(
            gakview -> file -> pixbuf,
            gakview -> file -> width * view_box_height / gakview -> file -> height,
            view_box_height,
            GAKVIEW_QUALITY_MODE
        );
    }
    else
    {
        tempbuf = gdk_pixbuf_scale_simple(
            gakview -> file -> pixbuf,
            view_box_width,
            gakview -> file -> height * view_box_width / gakview -> file -> width,
            GAKVIEW_QUALITY_MODE
        );
    }
    gtk_image_set_from_pixbuf(GTK_IMAGE(gakview -> image), tempbuf);

    // Next code is used to redraw for window.
    g_object_unref(G_OBJECT(tempbuf));
    gtk_widget_queue_draw(gakview -> window);
}

// SubMenu "Open"
void gakview_gmenu_open(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    UNUSED(action);
    UNUSED(parameter);
    UNUSED(data);

    GtkWidget *dialog = gtk_file_chooser_dialog_new ("Open File",
    GTK_WINDOW(gakview -> window),
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

        change_current_file(gtk_file_chooser_get_filename(chooser));

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
        // GakviewFile
        GakviewFile *current_file = gakview -> file;
        GSList *temp_gslist = file_list;
        while(temp_gslist)
        {
            current_file -> path = temp_gslist -> data;

            if(temp_gslist -> next)
            {
                GakviewFile *next_file = (GakviewFile*)malloc(sizeof(GakviewFile));
                current_file -> next = (GakviewFile*)next_file;
                next_file -> previous = current_file;
            }
            else
            {
                current_file -> next = gakview -> file;
                gakview -> file -> previous = current_file;
            }
            current_file = current_file -> next;
            temp_gslist = temp_gslist -> next;
        }

        // Resizing image
        if(GAKVIEW_SIZE_MODE == NOMAL)
        gtk_image_set_from_pixbuf(GTK_IMAGE(gakview -> image), gakview -> file -> pixbuf);
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

    gtk_dialog_run(GTK_DIALOG(gakview -> about));
    gtk_widget_hide(gakview -> about);
}

// SubMenu "Quit"
void gakview_gmenu_quit (GSimpleAction *action, GVariant *parameter, gpointer data)
{
    UNUSED(action);
    UNUSED(parameter);
    UNUSED(data);

    gtk_widget_destroy(gakview -> window);
}

void gakview_gmenu_mode_size_nomal (GSimpleAction *action, GVariant *parameter, gpointer data)
{
    UNUSED(action);
    UNUSED(parameter);
    UNUSED(data);

    GAKVIEW_SIZE_MODE = NOMAL;
    gtk_image_set_from_pixbuf(GTK_IMAGE(gakview -> image), gakview -> file -> pixbuf);
    gakview -> zoom_count = 100;
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
        gtk_widget_destroy(gakview -> window);
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
    if(GAKVIEW_SIZE_MODE != NOMAL || gakview -> zoom_count >= 500)
    return;
    gakview -> zoom_count += 5;

    GdkPixbuf *tempbuf = gdk_pixbuf_scale_simple(
        gakview -> file -> pixbuf,
        gdk_pixbuf_get_width(gakview -> file -> pixbuf) * gakview -> zoom_count / 100,
        gdk_pixbuf_get_height(gakview -> file -> pixbuf) * gakview -> zoom_count / 100,
        GAKVIEW_QUALITY_MODE);
        gtk_image_set_from_pixbuf(GTK_IMAGE(gakview -> image), tempbuf
    );

    g_object_unref(G_OBJECT(tempbuf));
}

void gakview_image_zoom_out()
{
    if(GAKVIEW_SIZE_MODE != NOMAL || gakview -> zoom_count <= 10)
    return;

    gakview -> zoom_count -= 5;

    GdkPixbuf *tempbuf = gdk_pixbuf_scale_simple(
        gakview -> file -> pixbuf,
        gdk_pixbuf_get_width(gakview -> file -> pixbuf) * gakview -> zoom_count / 100,
        gdk_pixbuf_get_height(gakview -> file -> pixbuf) * gakview -> zoom_count / 100,
        GAKVIEW_QUALITY_MODE
    );
    gtk_image_set_from_pixbuf(GTK_IMAGE(gakview -> image), tempbuf);

    g_object_unref(G_OBJECT(tempbuf));
}

void gakview_image_next()
{
    gakview -> file = gakview -> file -> next;
    change_current_file(gakview -> file -> path);
}

void gakview_image_previous()
{
    gakview -> file = gakview -> file -> previous;
    change_current_file(gakview -> file-> path);
}

static void change_current_file(char *file_path)
{
    gtk_window_set_title(GTK_WINDOW(gakview -> window), file_path);
    gakview -> file -> path = (gpointer)file_path;
    gakview -> file -> pixbuf = gdk_pixbuf_new_from_file(file_path, NULL);
    gakview -> file -> width = gdk_pixbuf_get_width(gakview -> file -> pixbuf);
    gakview -> file -> height = gdk_pixbuf_get_height(gakview -> file -> pixbuf);
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
