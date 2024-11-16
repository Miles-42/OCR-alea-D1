#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <dirent.h>
#include "network.h"
#include "matrix.h"
#include "backprop.h"
#include "file_manager.h"

Uint32 getPixel(SDL_Surface *surface, int x, int y)
{
    //int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;  // 4=bpp

    return *(Uint32 *)p;
}

struct matrix vectorize(char* image_path)
{
	SDL_Surface* surface = IMG_Load(image_path);
	surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);

	if (surface->w != 28 || surface->h != 28)
		err(EXIT_FAILURE, "vectorize(): h or w != 28");

	Uint8 r, g, b;
	Uint32 pixel;
	struct matrix res = init_matrix(28 * 28, 1);


	for (int y = 0; y < 28; y++)
	{
		for (int x = 0; x < 28; x++)
		{
			pixel = getPixel(surface, x, y);
			SDL_GetRGB(pixel, surface->format, &r, &g, &b);
			if (r + g + b < 380)
				res.data[x * 28 + y] = 1;
			else
				res.data[x * 28 + y] = 0;
		}
	}


	SDL_FreeSurface(surface);

	return res;
}

void train_one(struct network network, double eta, char* image_path, int expected)
{
	double data[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	data[expected] = 1;
	struct matrix result = init_matrix_data(10, 1, data);
	struct matrix image = vectorize(image_path);
	struct matrix training_data[2] = { image, result };
	
	update_network(network, eta, training_data);
}

void train_set(struct network network, double eta)
{
	char dir_path[30];
	char image_path[30];
	DIR* directory;
	for (int i = 0; i < 10; i++)
	{
		sprintf(dir_path, "./images/%i/", i);
		directory = opendir(dir_path);
		if (directory == NULL)
			err(EXIT_FAILURE, "train_set(): opendir()");
		struct dirent* lecture;
		const char* extension = ".png";
		while((lecture = readdir(directory)))
		{
			if (strstr(lecture->d_name, extension) != NULL)
			{
				printf("%s\n", dir_path);
				strcpy(image_path, dir_path);
				strcat(image_path, lecture->d_name);
				printf("path = '%s', dir = '%s'\n", image_path, dir_path);
				train_one(network, eta, image_path, i);
			}
		}
	}
	closedir(directory);
}

void train_file_all(char* file_name, double eta, size_t n)
{
	struct network network = load_network(file_name);
	for (size_t i = 0; i < n; i++)
		train_set(network, eta);

	write_network(network, file_name);
}


void train_file_one(char* file_name, char* data_path, double eta, int res)
{
	struct network network = load_network(file_name);
        train_one(network, eta, data_path, res);
        write_network(network, file_name);
}

int max(struct matrix matrix)
{
	double max = matrix.data[0];
	int res = 0;
	for (int i = 1; i < 10; i++)
	{
		if (matrix.data[i] > max)
		{
			max = matrix.data[i];
			res = i;
		}
	}
	return res;
}

int use_file(char* file_name, char* image_path)
{
	struct matrix input = vectorize(image_path);
	struct matrix output = use_network_file(file_name, input);
	return max(output);
}
