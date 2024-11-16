#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "matrix.h"
#include "network.h"

void serializeMatrix(struct matrix* matrix, FILE* file)
{
	fwrite(&(matrix->row), sizeof(size_t), 1, file);
	fwrite(&(matrix->col), sizeof(size_t), 1, file);
	fwrite(matrix->data, sizeof(double), matrix->row * matrix->col + 1, file);
}

void deserializeMatrix(struct matrix* matrix, FILE* file)
{
	fread(&(matrix->row), sizeof(size_t), 1, file);
        fread(&(matrix->col), sizeof(size_t), 1, file);
	
	matrix->data = malloc((matrix->row * matrix->col + 1) * sizeof(double));
	fread(matrix->data, sizeof(double), matrix->row * matrix->col + 1, file);
}

void serializeNetwork(struct network* network, FILE* file)
{
	fwrite(&(network->nb_layers), sizeof(size_t), 1, file);
	fwrite(network->sizes, sizeof(size_t), network->nb_layers, file);
	for (size_t i = 0; i < network->nb_layers - 1; i++)
	{
		serializeMatrix(&(network->weights[i]), file);
	}
	for(size_t j = 0; j < network->nb_layers - 1; j++)
	{
		serializeMatrix(&(network->biases[j]), file);
	}
}

void deserializeNetwork(struct network* network, FILE* file)
{
	fread(&(network->nb_layers), sizeof(size_t), 1, file);
	network->sizes = malloc(network->nb_layers * sizeof(size_t));
	fread(network->sizes, sizeof(size_t), network->nb_layers, file);
		
	network->weights = malloc((network->nb_layers - 1) * sizeof(struct matrix));
	for (size_t i = 0; i < network->nb_layers - 1; i++)
        {
                deserializeMatrix(&(network->weights[i]), file);	
        }
	network->biases = malloc((network->nb_layers - 1) * sizeof(struct matrix));
	for (size_t j = 0; j < network->nb_layers - 1; j++)
	{
		deserializeMatrix(&(network->biases[j]), file);
	}
}

void init_network_file(char* file_name, size_t* sizes, size_t nb_layers)
{
	struct network network = init_network(sizes, nb_layers);
	FILE* file;
	file = fopen(file_name, "wb");
	if (file == NULL)
		err(EXIT_FAILURE, "init_network_file() : failure to open file");
	serializeNetwork(&network, file);
	fclose(file);
}

struct network load_network(char* file_name)
{
	struct network network;
	FILE* file;
	file = fopen(file_name, "rb");
	if (file == NULL)
		err(EXIT_FAILURE, "get_network() : failure to open file");
	deserializeNetwork(&network, file);
	fclose(file);
	return network;
}

void write_network(struct network network, char* file_name)
{
	FILE* file;
        file = fopen(file_name, "wb");
        if (file == NULL)
                err(EXIT_FAILURE, "writre_network() : failure to open file");
        serializeNetwork(&network, file);
        fclose(file);
}
