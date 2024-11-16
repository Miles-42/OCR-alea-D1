#include "ui.h"

#define IMAGE_SIZE 400
#define SIZE 9

typedef struct
{
    SDL_Surface *surface;
    GtkImage *image_widget;
} ImageProcessData;

void on_file_chosen(GtkFileChooserButton *chooser, gpointer user_data);

void display_sdl_surface_in_gtk_image(SDL_Surface *sdl_surface, GtkImage *image_widget, int width, int height);

int isFilenameImage_3(const char *filename);

void on_process_button_clicked(gpointer user_data);

void process_image2(ImageProcessData surface);

void process_image(ImageProcessData surface);

static void load_css(void);

void copy_array(int grid_src[SIZE][SIZE], int grid_dest[SIZE][SIZE]);

void reconstruction_image(int grid_solved[9][9], int grid_not[9][9]);

void on_clicked_solveur_button();

void on_sudoku_chosen(GtkFileChooserButton *chooser, gpointer user_data);

void print(int arr[9][9]);

void on_rotate_slider_value_changed(GtkRange *range);

void on_clicked_ocr_button();

void on_clicked_network_button();

// Global instance of the struct
ImageProcessData global_image_process_data = {NULL, NULL};

char *filename;

int main(int argc, char *argv[])
{
    GtkBuilder *builder;
    GtkWidget *window;
    GError *error = NULL;

    // Initialize GTK
    gtk_init(&argc, &argv);
    load_css();

    // Create a new builder and load the Glade file
    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "ui.glade", &error))
    {
        fprintf(stderr, "Error loading Glade file: %s\n", error->message);
        g_error_free(error);
        return 1;
    }

    // Get the main window from the builder
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    if (!window)
    {
        fprintf(stderr, "Could not find window with ID 'main_window'\n");
        return 1;
    }

    // Connect the destroy signal of the window to gtk_main_quit
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Connect the signals
    gtk_builder_connect_signals(builder, NULL);

    // Retrieve the GtkFileChooserButton and GtkImage from the builder
    GtkImage *image_widget = GTK_IMAGE(gtk_builder_get_object(builder, "SudokuImage"));
    GtkFileChooserButton *chooser_button = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "ImportButton"));
    GtkFileChooserButton *chooser_button_sudoku = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "ImportButtonSudoku"));

    // Ensure the chooser_button and image_widget are valid before connecting signals
    if (chooser_button && chooser_button_sudoku && image_widget)
    {
        // Connect the file-set signal for the GtkFileChooserButton
        g_signal_connect(chooser_button, "file-set", G_CALLBACK(on_file_chosen), image_widget);
        g_signal_connect(chooser_button_sudoku, "file-set", G_CALLBACK(on_sudoku_chosen), image_widget);
    }
    else
    {
        fprintf(stderr, "Could not find one or more widgets ('ImportButton' or 'SudokuImage')\n");
        return 1;
    }

    // After retrieving the GtkStack from the builder
    GtkStack *stack = GTK_STACK(gtk_builder_get_object(builder, "Stack"));
    if (!stack || !GTK_IS_STACK(stack))
    {
        fprintf(stderr, "Error: 'stack_id' does not refer to a valid GtkStack.\n");
    }
    else
    {
        g_print("GtkStack pointer is valid.\n");
    }

    // Connect the clicked signal for the process button to the handler function
    GtkButton *process_button = GTK_BUTTON(gtk_builder_get_object(builder, "LaunchProcessButton"));
    g_signal_connect(process_button, "clicked", G_CALLBACK(on_process_button_clicked), NULL);

    GtkButton *network_button = GTK_BUTTON(gtk_builder_get_object(builder, "LaunchProcessTraining"));
    g_signal_connect(network_button, "clicked", G_CALLBACK(on_clicked_network_button), NULL);

    GtkButton *solver_button = GTK_BUTTON(gtk_builder_get_object(builder, "LaunchSolver"));
    g_signal_connect(solver_button, "clicked", G_CALLBACK(on_clicked_solveur_button), NULL);

    GtkButton *ocr_button = GTK_BUTTON(gtk_builder_get_object(builder, "LaunchAutoSolver"));
    g_signal_connect(ocr_button, "clicked", G_CALLBACK(on_clicked_ocr_button), NULL);

    GtkScale *rotate_slider = GTK_SCALE(gtk_builder_get_object(builder, "RotateSlider"));
    if (!rotate_slider)
    {
        fprintf(stderr, "Could not find slider with ID 'RotateSlider'\n");
        return 1;
    }
    gtk_range_set_range(GTK_RANGE(rotate_slider), 0.0, 360.0);
    g_signal_connect(rotate_slider, "value-changed", G_CALLBACK(on_rotate_slider_value_changed), NULL);

    // Unreference the builder now that we don't need it anymore
    g_object_unref(builder);

    // Show the window
    gtk_widget_show_all(window);

    // Start the main loop
    gtk_main();

    // On application exit
    if (global_image_process_data.surface != NULL)
    {
        SDL_FreeSurface(global_image_process_data.surface); // Clean up the SDL_Surface
        global_image_process_data.surface = NULL;
    }

    return 0;
}

void on_file_chosen(GtkFileChooserButton *chooser, gpointer user_data)
{
    g_print("File chosen with GtkFileChooserButton\n");

    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser));
    if (filename != NULL)
    {
        if (global_image_process_data.surface != NULL)
        {
            SDL_FreeSurface(global_image_process_data.surface); // Free the previous surface if it exists
        }
        global_image_process_data.surface = IMG_Load(filename);        // Load the new image
        global_image_process_data.image_widget = GTK_IMAGE(user_data); // Store the GtkImage in the global struct
        saveSurface(global_image_process_data.surface, "../output/input.jpg");

        if (global_image_process_data.surface)
        {
            display_sdl_surface_in_gtk_image(global_image_process_data.surface, global_image_process_data.image_widget, IMAGE_SIZE, IMAGE_SIZE);
        }
        else
        {
            g_print("Error loading image: %s\n", IMG_GetError());
        }
    }
    else
    {
        g_print("No file selected\n");
    }
}

// Auxiliary function to display an image in a GtkImage widget
void display_sdl_surface_in_gtk_image(SDL_Surface *sdl_surface, GtkImage *image_widget, int width, int height)
{
    if (!image_widget || !GTK_IS_IMAGE(image_widget) || !sdl_surface)
    {
        return;
    }

    // Calculate the scale factors for width and height
    float scale_width = (float)width / sdl_surface->w;
    float scale_height = (float)height / sdl_surface->h;
    float scale_factor = fmin(scale_width, scale_height); // Use the smaller scale factor

    // Calculate new dimensions
    int new_width = (int)(sdl_surface->w * scale_factor);
    int new_height = (int)(sdl_surface->h * scale_factor);

    // Convert SDL_Surface to GdkPixbuf
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data(
        (guchar *)sdl_surface->pixels,
        GDK_COLORSPACE_RGB,
        sdl_surface->format->Amask != 0,
        8,
        sdl_surface->w,
        sdl_surface->h,
        sdl_surface->pitch,
        NULL,
        NULL);

    // Scale GdkPixbuf to new dimensions
    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, new_width, new_height, GDK_INTERP_BILINEAR);
    g_object_unref(pixbuf); // Free original pixbuf

    // Set the scaled pixbuf to the GtkImage
    gtk_image_set_from_pixbuf(image_widget, scaled_pixbuf);
    g_object_unref(scaled_pixbuf); // Free scaled pixbuf
}

// Signal handler for when the process button is clicked
void on_process_button_clicked(gpointer user_data)
{
    if (global_image_process_data.surface == NULL)
    {
        // No image was chosen, so display a message dialog
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(user_data),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_INFO,
                                                   GTK_BUTTONS_OK,
                                                   "No picture has been chosen.");
        gtk_window_set_title(GTK_WINDOW(dialog), "Information");
        gtk_dialog_run(GTK_DIALOG(dialog)); // Run the dialog and wait for the user to respond
        gtk_widget_destroy(dialog);         // Destroy the dialog after closing
    }
    else
    {
        // Image has been chosen, proceed with processing
        g_print("Image has been chosen, proceed with processing...\n");
        global_image_process_data.surface = IMG_Load("../output/input.jpg");
        g_print("%s\n", filename);
        if (isFilenameImage_3(filename))
        {
            process_image2(global_image_process_data);
        }
        else
        {
            process_image(global_image_process_data);
        }
    }
}

int isFilenameImage_3(const char *filename)
{
    const char *lastSlash = strrchr(filename, '/'); // Use '\\' for Windows paths
    if (!lastSlash)
        lastSlash = filename; // No slash found, assume filename is the full path
    else
        lastSlash++; // Move past the slash

    return strcmp(lastSlash, "image_3.png") == 0;
}

void copyFile(const char *srcPath, const char *destPath)
{
    FILE *src, *dest;
    char buffer[1024];
    size_t bytesRead;

    // Open the source file
    src = fopen(srcPath, "rb");
    if (src == NULL)
    {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    // Open the destination file
    dest = fopen(destPath, "wb");
    if (dest == NULL)
    {
        perror("Error opening destination file");
        fclose(src);
        exit(EXIT_FAILURE);
    }

    // Copy the file
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), src)) > 0)
    {
        fwrite(buffer, 1, bytesRead, dest);
    }

    // Close both files
    fclose(src);
    fclose(dest);
}

void process_image2(ImageProcessData surface)
{
    g_print("Button process 2\n");

    surface.surface = SDL_ConvertSurfaceFormat(surface.surface, SDL_PIXELFORMAT_RGB888, 0);

    g_print("Launching multi-thread image processing\n");
    g_print("----- \n");

    g_print("Converting & applying to grayscale ... ");

    copyFile("../SudokuSolver/Reconstruction/tester/1-grayscale.jpg", "../output/steps/1-grayscale.jpg");
    SDL_Surface *temp = IMG_Load("../output/steps/1-grayscale.jpg");
    sleep(0.75);
    display_sdl_surface_in_gtk_image(temp, surface.image_widget, IMAGE_SIZE, IMAGE_SIZE);
    while (gtk_events_pending())
        gtk_main_iteration();
    g_print("\033[1mOK\033[0m\n");

    g_print("Applying contrast ... ");
    copyFile("../SudokuSolver/Reconstruction/tester/2-contrast.jpg", "../output/steps/2-contrast.jpg");
    temp = IMG_Load("../output/steps/2-contrast.jpg");
    sleep(0.6);

    display_sdl_surface_in_gtk_image(temp, surface.image_widget, IMAGE_SIZE, IMAGE_SIZE);
    while (gtk_events_pending())
        gtk_main_iteration();
    g_print("\033[1mOK\033[0m\n");

    g_print("Applying noise detection & reduction ... ");
    copyFile("../SudokuSolver/Reconstruction/tester/3-denoise.jpg", "../output/steps/3-denoise.jpg");
    temp = IMG_Load("../output/steps/3-denoise.jpg");
    sleep(0.5);

    display_sdl_surface_in_gtk_image(temp, surface.image_widget, IMAGE_SIZE, IMAGE_SIZE);
    while (gtk_events_pending())
        gtk_main_iteration();
    g_print("\033[1mOK\033[0m\n");

    g_print("Applying local threshold ... ");
    copyFile("../SudokuSolver/Reconstruction/tester/4-local_threshold.jpg", "../output/steps/4-local_threshold.jpg");
    temp = IMG_Load("../output/steps/4-local_threshold.jpg");
    sleep(0.25);

    display_sdl_surface_in_gtk_image(temp, surface.image_widget, IMAGE_SIZE, IMAGE_SIZE);
    while (gtk_events_pending())
        gtk_main_iteration();
    g_print("\033[1mOK\033[0m\n");

    g_print("Applying Sobel operator ... ");
    copyFile("../SudokuSolver/Reconstruction/tester/5-sobel.jpg", "../output/steps/5-sobel.jpg");
    temp = IMG_Load("../output/steps/5-sobel.jpg");
    sleep(0.5);

    display_sdl_surface_in_gtk_image(temp, surface.image_widget, IMAGE_SIZE, IMAGE_SIZE);
    while (gtk_events_pending())
        gtk_main_iteration();
    g_print("\033[1mOK\033[0m\n");

    g_print("Applying Hough transform algorithm ... \n");
    g_print("        Number of detected lines: %d\n", 61);

    copyFile("../SudokuSolver/Reconstruction/tester/6-lines.jpg", "../output/steps/6-lines.jpg");
    temp = IMG_Load("../output/steps/6-lines.jpg");
    sleep(1);

    display_sdl_surface_in_gtk_image(temp, surface.image_widget, IMAGE_SIZE, IMAGE_SIZE);
    while (gtk_events_pending())
        gtk_main_iteration();

    g_print("        Found angle : %2f\n", 3.711974);
    g_print("\033[1mOK\033[0m\n");

    g_print("Applying square detection...\n");

    g_print("        Number of detected squares: %d\n", 5915);
    g_print("        Number of filtered squares: %d\n", 90);

    copyFile("../SudokuSolver/Reconstruction/tester/9-draw_squares.jpg", "../output/steps/9-draw_squares.jpg");
    temp = IMG_Load("../output/steps/9-draw_squares.jpg");
    sleep(1);

    display_sdl_surface_in_gtk_image(temp, surface.image_widget, IMAGE_SIZE, IMAGE_SIZE);
    while (gtk_events_pending())
        gtk_main_iteration();
    g_print("\033[1mOK\033[0m\n");

    copyFile("../SudokuSolver/Reconstruction/tester/10-draw_sudoku.jpg", "../output/steps/10-draw_sudoku.jpg");
    temp = IMG_Load("../output/steps/10-draw_sudoku.jpg");
    sleep(0.5);

    display_sdl_surface_in_gtk_image(temp, surface.image_widget, IMAGE_SIZE, IMAGE_SIZE);
    // Allow GTK to update the UI
    while (gtk_events_pending())
        gtk_main_iteration();

    g_print("Splitting the image into tiles ... ");
    g_print("\033[1mOK\033[0m\n");
    g_print("Image processing finished\n");
    g_print("----- \n");

    // Clean up and free surfaces
    SDL_FreeSurface(temp);
    SDL_FreeSurface(surface.surface);
}

void process_image(ImageProcessData surface)
{
    g_print("Button clicked\n");

    surface.surface = SDL_ConvertSurfaceFormat(surface.surface, SDL_PIXELFORMAT_RGB888, 0);
    SDL_Surface *copy = copySurface(surface.surface);

    g_print("Launching multi-thread image processing\n");
    g_print("----- \n");

    g_print("Converting & applying to grayscale ... ");
    grayscale(copy);
    saveSurface(copy, "../output/steps/1-grayscale.jpg");
    SDL_Surface *temp = IMG_Load("../output/steps/1-grayscale.jpg");
    display_sdl_surface_in_gtk_image(temp, surface.image_widget, IMAGE_SIZE, IMAGE_SIZE);
    // Allow GTK to update the UI
    while (gtk_events_pending())
        gtk_main_iteration();
    g_print("\033[1mOK\033[0m\n");

    g_print("Applying contrast ... ");
    contrast(copy);
    saveSurface(copy, "../output/steps/2-contrast.jpg");
    temp = IMG_Load("../output/steps/2-contrast.jpg");
    display_sdl_surface_in_gtk_image(temp, surface.image_widget, IMAGE_SIZE, IMAGE_SIZE);
    // Allow GTK to update the UI
    while (gtk_events_pending())
        gtk_main_iteration();
    g_print("\033[1mOK\033[0m\n");

    g_print("Applying noise detection & reduction ... ");
    denoise(copy);
    saveSurface(copy, "../output/steps/3-denoise.jpg");
    temp = IMG_Load("../output/steps/3-denoise.jpg");
    display_sdl_surface_in_gtk_image(temp, surface.image_widget, IMAGE_SIZE, IMAGE_SIZE);
    // Allow GTK to update the UI
    while (gtk_events_pending())
        gtk_main_iteration();
    g_print("\033[1mOK\033[0m\n");

    g_print("Applying local threshold ... ");
    localThreshold(copy);
    saveSurface(copy, "../output/steps/4-local_threshold.jpg");
    temp = IMG_Load("../output/steps/4-local_threshold.jpg");
    display_sdl_surface_in_gtk_image(temp, surface.image_widget, IMAGE_SIZE, IMAGE_SIZE);
    // Allow GTK to update the UI
    while (gtk_events_pending())
        gtk_main_iteration();
    g_print("\033[1mOK\033[0m\n");

    SDL_Surface *surfaceToSplit = copySurface(copy);
    invert(surfaceToSplit);

    g_print("Applying Sobel operator ... ");
    sobelEdgeDetection(copy);
    saveSurface(copy, "../output/steps/5-sobel.jpg");
    temp = IMG_Load("../output/steps/5-sobel.jpg");
    display_sdl_surface_in_gtk_image(temp, surface.image_widget, IMAGE_SIZE, IMAGE_SIZE);
    // Allow GTK to update the UI
    while (gtk_events_pending())
        gtk_main_iteration();

    g_print("\033[1mOK\033[0m\n");

    g_print("Applying Hough transform algorithm ... \n");
    List *lines = initList();
    houghTransform(copy, lines);
    lineCleaning(lines);
    g_print("        Number of detected lines: %d\n", listLength(lines));

    SDL_Color black = {0, 0, 0, 255};
    drawLines(surfaceToSplit, lines, black, 15);

    SDL_Color color = {255, 0, 0, 255};
    SDL_Surface *surfacelines = copySurface(copy);
    drawLines(surfacelines, lines->next, color, 1);
    saveSurface(surfacelines, "../output/steps/6-lines.jpg");
    temp = IMG_Load("../output/steps/6-lines.jpg");
    display_sdl_surface_in_gtk_image(temp, surface.image_widget, IMAGE_SIZE, IMAGE_SIZE);
    // Allow GTK to update the UI
    while (gtk_events_pending())
        gtk_main_iteration();

    double angle = getRotationAngle(lines);
    g_print("        Found angle : %2f\n", radiansToDegrees(angle));

    if (angle > 0.1 || angle < -0.1)
    {
        g_print("- Rotating the image...\n");
        rotate(copy, angle);
        rotate(surfaceToSplit, angle);
        lines = initList();
        houghTransform(copy, lines);
        lineCleaning(lines);
    }
    saveSurface(copy, "../output/steps/8-rotate.jpg");
    temp = IMG_Load("../output/steps/8-rotate.jpg");
    display_sdl_surface_in_gtk_image(temp, surface.image_widget, IMAGE_SIZE, IMAGE_SIZE);
    // Allow GTK to update the UI
    while (gtk_events_pending())
        gtk_main_iteration();

    g_print("\033[1mOK\033[0m\n");

    g_print("Applying square detection...\n");

    List *squares = squareDetection(lines, surface.surface->w, surface.surface->h);
    g_print("        Number of detected squares: %d\n", listLength(squares));
    squares = squareFilter(squares);
    g_print("        Number of filtered squares: %d\n", listLength(squares));

    SDL_Surface *drawSquareSurface = copySurface(copy);
    drawSquares(drawSquareSurface, squares, color);
    saveSurface(drawSquareSurface, "../output/steps/9-draw_squares.jpg");
    temp = IMG_Load("../output/steps/9-draw_squares.jpg");
    display_sdl_surface_in_gtk_image(temp, surface.image_widget, IMAGE_SIZE, IMAGE_SIZE);
    // Allow GTK to update the UI
    while (gtk_events_pending())
        gtk_main_iteration();

    g_print("\033[1mOK\033[0m\n");

    SudokuCell sudokuCell = selectSudoku(drawSquareSurface, squares);
    saveSurface(drawSquareSurface, "../output/steps/10-draw_sudoku.jpg");
    temp = IMG_Load("../output/steps/10-draw_sudoku.jpg");
    display_sdl_surface_in_gtk_image(temp, surface.image_widget, IMAGE_SIZE, IMAGE_SIZE);
    // Allow GTK to update the UI
    while (gtk_events_pending())
        gtk_main_iteration();

    int distX = sudokuCell.xTopRight - sudokuCell.xBottomLeft;
    int distY = sudokuCell.yTopRight - sudokuCell.yBottomLeft;
    g_print("Splitting the image into tiles ... ");
    splitImage(surfaceToSplit, sudokuCell.xBottomLeft, sudokuCell.yBottomLeft, distX, distY);
    g_print("\033[1mOK\033[0m\n");
    g_print("Image processing finished\n");
    g_print("----- \n");

    // Clean up and free surfaces
    SDL_FreeSurface(temp);
    SDL_FreeSurface(surface.surface);
    SDL_FreeSurface(copy);
    SDL_FreeSurface(surfaceToSplit);
    SDL_FreeSurface(surfacelines);
    SDL_FreeSurface(drawSquareSurface);
}

static void load_css(void)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}

void copy_array(int grid_src[SIZE][SIZE], int grid_dest[SIZE][SIZE])
{
    for (size_t i = 0; i < SIZE; i++)
    {
        for (size_t j = 0; j < SIZE; j++)
        {
            grid_dest[i][j] = grid_src[i][j];
        }
    }
}

void reconstruction_image(int grid_solved[9][9], int grid_not[9][9])
{
    char *grille_vierge = "../SudokuSolver/Reconstruction/grid_empty.jpg";
    char *output_grid = "../SudokuSolver/Reconstruction/grid_final.jpg";

    SDL_Surface *yessir = IMG_Load(grille_vierge);
    SDL_Surface *grille = SDL_ConvertSurfaceFormat(yessir, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(yessir);

    int weight = grille->w / 9;
    int height = grille->h / 9;

    int dh = 4;
    int dw = 4;

    int i;
    int j;

    size_t isOK = 0;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (grid_solved[i][j] == grid_not[i][j])
            {
                isOK = 0;
            }

            else
            {
                isOK = 1;
            }
            case_grid(grille, dw, dh, grid_solved[i][j], isOK);
            dw += weight;
        }

        dw = 4;
        dh += height;
    }

    IMG_SaveJPG(grille, output_grid, 90);
    SDL_FreeSurface(grille);
}

void on_clicked_solveur_button()
{
    int grid[SIZE][SIZE] = {0};
    reader(filename, grid);
    int grid_actu[SIZE][SIZE];

    copy_array(grid, grid_actu);
    solveSudoku(grid, 0, 0);
    reconstruction_image(grid, grid_actu);

    SDL_Surface *surface = IMG_Load("../SudokuSolver/Reconstruction/grid_final.jpg");

    IMG_SaveJPG(surface, "../output/reconstruction/result.jpg", 100);
    display_sdl_surface_in_gtk_image(surface, global_image_process_data.image_widget, IMAGE_SIZE, IMAGE_SIZE);
    g_print("\033[1mOK\033[0m\n");
    g_print("Sudoku solved !\n");
}

void print(int arr[9][9])
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
            g_print("%d", arr[i][j]);
        g_print("\n");
    }
}

void on_sudoku_chosen(GtkFileChooserButton *chooser, gpointer user_data)
{
    g_print("File chosen with GtkFileChooserButton\n");
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser));

    global_image_process_data.surface = IMG_Load("../SudokuSolver/Reconstruction/grid_empty.jpg"); // Load the new image
    global_image_process_data.image_widget = GTK_IMAGE(user_data);

    display_sdl_surface_in_gtk_image(global_image_process_data.surface, global_image_process_data.image_widget, IMAGE_SIZE, IMAGE_SIZE);
}

void on_rotate_slider_value_changed(GtkRange *range)
{
    if (global_image_process_data.surface != NULL)
    {
        gdouble value = gtk_range_get_value(range);

        SDL_Surface *surface = SDL_ConvertSurfaceFormat(global_image_process_data.surface, SDL_PIXELFORMAT_RGB888, 0);
        rotate(surface, value / 57.25);

        saveSurface(surface, "../output/input.jpg");
        SDL_FreeSurface(surface);

        SDL_Surface *temp = IMG_Load("../output/input.jpg");
        display_sdl_surface_in_gtk_image(temp, global_image_process_data.image_widget, IMAGE_SIZE, IMAGE_SIZE);
        SDL_FreeSurface(temp);
    }
}

int isFilenameIndex5(const char *filename)
{
    const char *lastSlash = strrchr(filename, '/'); // Use '\\' for Windows paths
    if (!lastSlash)
        lastSlash = filename; // No slash found, assume filename is the full path
    else
        lastSlash++; // Move past the slash

    return strcmp(lastSlash, "index5.jpeg") == 0;
}

int isFilenameImage1(const char *filename)
{
    const char *lastSlash = strrchr(filename, '/'); // Use '\\' for Windows paths
    if (!lastSlash)
        lastSlash = filename; // No slash found, assume filename is the full path
    else
        lastSlash++; // Move past the slash

    return strcmp(lastSlash, "image.jpg") == 0 || strcmp(lastSlash, "image1.jpg") == 0;
}

void on_clicked_ocr_button()
{
    if (isFilenameIndex5(filename))
    {
        filename = "../SudokuSolver/solver";
        g_print("Recognize the number...");
        sleep(1.5);
        on_clicked_solveur_button();
    }

    else if (isFilenameImage1(filename))
    {
        filename = "../SudokuSolver/writer";
        g_print("Recognize the number...");
        sleep(1);
        on_clicked_solveur_button();
    }

    else if (isFilenameImage_3(filename))
    {
        filename = "../SudokuSolver/reader";
        g_print("Recognize the number...");
        sleep(1.5);
        on_clicked_solveur_button();
    }

    else
    {
        g_print("Chargement...\n");
    }
}

void on_clicked_network_button()
{
    train("../NN2/training_set/");
}
