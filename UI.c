#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
    GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *image; // Widget for the image

    // Initialize GTK+
    gtk_init(&argc, &argv);

    // Create a new GtkBuilder object and load the UI from the .glade file
    builder = gtk_builder_new_from_file("UI OCR.glade");

    // Get the main window from the .glade file by its ID
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main")); // Replace with actual ID

    // Get the image widget from the .glade file by its ID
    image = GTK_WIDGET(gtk_builder_get_object(builder, "SudokuGirdPicture")); // Use your actual GtkImage ID

    // Load an image file into the GtkImage widget
    gtk_image_set_from_file(GTK_IMAGE(image), "GridDetection/Images/image.jpg"); // Replace with the actual image path

    // Show all widgets within the window
    gtk_widget_show_all(window);

    // Connect signal handlers here if you have any

    // Destroy builder since it's no longer needed
    g_object_unref(G_OBJECT(builder));

    // Start the GTK+ main loop
    gtk_main();

    return 0;
}
