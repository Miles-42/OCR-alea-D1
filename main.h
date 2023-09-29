#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdbool.h>
#include "GridDetection/hough_transform.h"
#include "GridDetection/local_threshold.h"
#include "GridDetection/sobel.h"
#include "Traitement/contrast.h"
#include "Traitement/grayscale.h"
#include "Traitement/noise_reduction.h"
#include "utils/lines.h"
#include "utils/linked_list.h"
#include "utils/splitting.h"
#include "utils/utils.h"

/**
 * @brief Does the image processing
 * @param verbose verbose or not
 *
 */
void imageProcessingUi(SDL_Surface *surface, bool verbose);

/**
 * @brief The main function for the rotation of an image
 * @param argv The number of arguments
 * @param argc The array of arguments
 * @return The exit code
 */
int imageRotateMain(int argv, char **argc);

/**
 * @brief The main function for the image processing
 * @param argc The number of arguments
 * @param argv The array of arguments
 *
 */
int imageProcessingMain(int argv, char **argc);
