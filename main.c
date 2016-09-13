#include <gtk/gtk.h>
#include <malloc.h>

#define UNUSED(x) (void)(x)

// NEW FUNCTION
static void gakview_init (GtkApplication *app, gpointer user_data);

void gakview_gmenu_create (GtkApplication *app);
void gakview_gmenu_open (GSimpleAction *action, GVariant *parameter, gpointer data);
void gakview_gmenu_mode_size_nomal(GSimpleAction *action, GVariant *parameter, gpointer data);
void gakview_gmenu_mode_size_auto(GSimpleAction *action, GVariant *parameter, gpointer data);
void gakview_gmenu_about (GSimpleAction *action, GVariant *parameter, gpointer data);
void gakview_gmenu_quit (GSimpleAction *action, GVariant *parameter, gpointer data);

void gakview_image_zoom_auto(GtkWindow *widget, GdkEvent *event, gpointer data);
void gakview_image_zoom_in ();
void gakview_image_zoom_out ();
void gakview_image_next ();
void gakview_image_previous ();

void gakview_input_key (GtkWidget *widget, GdkEventKey *event, gpointer data);

// FIXME OLD FUNCTION
static void change_current_file(char *file_path);

enum
{
    NOMAL_MODE,
    AUTO_MODE
} GAKVIEW_SIZE_MODE;

enum
{
    NEAREST = GDK_INTERP_NEAREST,
    TILES = GDK_INTERP_TILES,
    BILINEAR = GDK_INTERP_BILINEAR,
    HYPER = GDK_INTERP_HYPER
} GAKVIEW_QUALITY_MODE;

typedef struct GakFiles
{
    struct GakFiles *next;
    gpointer data;
    struct GakFiles *previous;
} GakFiles;

// Declare variable
GtkWidget *window;
GtkWidget *scroll_box;
GtkWidget *view_box;
GtkWidget *image;
GtkWidget *about;
GdkPixbuf *pixbuf;
GakFiles _gak_files;
GakFiles *gak_files = &_gak_files;
gchar *file_name;
GSList *file_list;
unsigned int pixbuf_width;
unsigned int pixbuf_height;
unsigned int zoom_count = 100;

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

    scroll_box = GTK_WIDGET(gtk_builder_get_object(builder, "scroll_box"));
    view_box = GTK_WIDGET(gtk_builder_get_object(builder, "view_box"));
    image = GTK_WIDGET(gtk_builder_get_object(builder, "image"));
    about = GTK_WIDGET(gtk_builder_get_object(builder, "about"));

    /*-- Init Setting --------------------------------------------------*/
    // pixbuf
    change_current_file("./images/failed.jpg");
    GAKVIEW_QUALITY_MODE = HYPER;

    // window
    window = gtk_application_window_new (app);
    gtk_container_add(GTK_CONTAINER(window), scroll_box);
    gtk_window_set_title (GTK_WINDOW (window), "GakView");
    gtk_window_set_default_size(
            GTK_WINDOW(window), 
            gdk_pixbuf_get_width(pixbuf) + 5,
            gdk_pixbuf_get_height(pixbuf) + 5);
    GAKVIEW_SIZE_MODE = AUTO_MODE;

    /*-- Gmenu ---------------------------------------------------------*/
    gakview_gmenu_create(app);

    /*-- Signal --------------------------------------------------------*/
    g_signal_connect(window, "configure-event", G_CALLBACK(gakview_image_zoom_auto), NULL);
    g_signal_connect(window, "key-press-event", G_CALLBACK(gakview_input_key), NULL);

    /*-- Finish --------------------------------------------------------*/
    gtk_widget_show_all (window);
    g_object_unref(G_OBJECT(builder));
}

// Auto resizing image
void gakview_image_zoom_auto(GtkWindow *widget, GdkEvent *event, gpointer data)
{
    UNUSED(widget);
    UNUSED(event);
    UNUSED(data);

    unsigned int view_box_width = gtk_widget_get_allocated_width(view_box);
    unsigned int view_box_height = gtk_widget_get_allocated_height(view_box);
    if(pixbuf_width < view_box_width && pixbuf_height < view_box_height)
    {
        gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
        return;
    }

    GdkPixbuf *tempbuf;
    if(pixbuf_height * view_box_width / pixbuf_width > view_box_height)
    {
        tempbuf = gdk_pixbuf_scale_simple(pixbuf,
                pixbuf_width * view_box_height / pixbuf_height,
                view_box_height,
                GAKVIEW_QUALITY_MODE);
    }
    else
    {
        tempbuf = gdk_pixbuf_scale_simple(pixbuf,
                view_box_width,
                pixbuf_height * view_box_width / pixbuf_width,
                GAKVIEW_QUALITY_MODE);
    }
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), tempbuf);

    // Next code is used to redraw for window.
    g_object_unref(G_OBJECT(tempbuf));
    gtk_widget_queue_draw(window);
}

// SubMenu "Open"
void gakview_gmenu_open(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    UNUSED(action);
    UNUSED(parameter);
    UNUSED(data);

    GtkWidget *dialog = gtk_file_chooser_dialog_new ("Open File",
            GTK_WINDOW(window),
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
        // GakFiles
        GakFiles *current_file = gak_files;
        GSList *temp_gslist = file_list;
        while(temp_gslist)
        {
            current_file -> data = temp_gslist -> data;

            if(temp_gslist -> next)
            {
                GakFiles *next_file = (GakFiles*)malloc(sizeof(GakFiles));
                current_file -> next = (GakFiles*)next_file;
                next_file -> previous = current_file;
            }
            else
            {
                current_file -> next = gak_files;
                gak_files -> previous = current_file;
            }
            current_file = current_file -> next;
            temp_gslist = temp_gslist -> next;
        }

        // Resizing image
        if(GAKVIEW_SIZE_MODE == NOMAL_MODE)
            gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
        else if (GAKVIEW_SIZE_MODE == AUTO_MODE)
            gakview_image_zoom_auto(NULL, NULL, NULL);
    }
    gtk_widget_destroy(dialog);
}

void gakview_gmenu_about (GSimpleAction *action, GVariant *parameter, gpointer data)
{
    UNUSED(action);
    UNUSED(parameter);
    UNUSED(data);

    gtk_dialog_run(GTK_DIALOG(about));
    gtk_widget_hide(about);
}

// SubMenu "Quit"
void gakview_gmenu_quit (GSimpleAction *action, GVariant *parameter, gpointer data)
{
    UNUSED(action);
    UNUSED(parameter);
    UNUSED(data);

    gtk_widget_destroy(window);
}

void gakview_gmenu_mode_size_nomal (GSimpleAction *action, GVariant *parameter, gpointer data)
{
    UNUSED(action);
    UNUSED(parameter);
    UNUSED(data);

    GAKVIEW_SIZE_MODE = NOMAL_MODE;
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
    zoom_count = 100;
}

void gakview_gmenu_mode_size_auto (GSimpleAction *action, GVariant *parameter, gpointer data)
{
    UNUSED(action);
    UNUSED(parameter);
    UNUSED(data);

    if(GAKVIEW_SIZE_MODE != AUTO_MODE)
        GAKVIEW_SIZE_MODE = AUTO_MODE;
    gakview_image_zoom_auto(NULL, NULL, NULL);
}

void gakview_input_key(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    UNUSED(widget);
    UNUSED(data);

    switch(event -> keyval)
    {
        case GDK_KEY_Escape:
            gtk_widget_destroy(window);
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
    if(GAKVIEW_SIZE_MODE != NOMAL_MODE || zoom_count >= 500)
        return; 
    zoom_count += 5;

    GdkPixbuf *tempbuf = gdk_pixbuf_scale_simple(pixbuf,
            gdk_pixbuf_get_width(pixbuf) * zoom_count / 100,
            gdk_pixbuf_get_height(pixbuf) * zoom_count / 100,
            GAKVIEW_QUALITY_MODE);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), tempbuf);

    g_object_unref(G_OBJECT(tempbuf));
}

void gakview_image_zoom_out()
{
    if(GAKVIEW_SIZE_MODE != NOMAL_MODE || zoom_count <= 10)
        return;

    zoom_count -= 5;

    GdkPixbuf *tempbuf = gdk_pixbuf_scale_simple(pixbuf,
            gdk_pixbuf_get_width(pixbuf) * zoom_count / 100,
            gdk_pixbuf_get_height(pixbuf) * zoom_count / 100,
            GAKVIEW_QUALITY_MODE);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), tempbuf);

    g_object_unref(G_OBJECT(tempbuf));
}

void gakview_image_next()
{
    gak_files = gak_files -> next;
    change_current_file(gak_files->data);
}

void gakview_image_previous()
{
    gak_files = gak_files -> previous;
    change_current_file(gak_files->data);
}

static void change_current_file(char *file_path)
{
    gtk_window_set_title(GTK_WINDOW(window), file_name);
    file_name = file_path;
    pixbuf = gdk_pixbuf_new_from_file(file_path, NULL);
    pixbuf_width = gdk_pixbuf_get_width(pixbuf);
    pixbuf_height = gdk_pixbuf_get_height(pixbuf);
    if(GAKVIEW_SIZE_MODE == NOMAL_MODE);
        // zoom(0);
    else if(GAKVIEW_SIZE_MODE == AUTO_MODE)
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
