#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include "matrix.h"
#include "network.h"
#include "file_manager.h"

struct network init_network(size_t* sizes, size_t nb_layers)
{
	struct network res;
	res.sizes = sizes;
	res.nb_layers = nb_layers;


	res.biases = malloc((res.nb_layers - 1) * sizeof(struct matrix));
	for (size_t i = 0; i < res.nb_layers - 1; i++)
	{
		res.biases[i] = init_matrix(sizes[i + 1], 1);
		for (size_t j = 0; j < sizes[i + 1]; j++)
		{
			res.biases[i].data[j] = 0;
		}
	}

	res.weights = malloc((res.nb_layers - 1) * sizeof(struct matrix));
	double limit = sqrt(6 / (sizes[0] + sizes[res.nb_layers - 1])); // Xavier/Glorot
	for (size_t k = 0; k < res.nb_layers - 1; k++)
	{
		res.weights[k] = init_matrix(sizes[k + 1], sizes[k]);
		for (size_t p = 0; p < res.weights[k].row * res.weights[k].col; p++)
		{
			res.weights[k].data[p] = (((double)rand() / RAND_MAX) * 2 - 1) * limit;
		}

	}

	return res;
}

void print_network(struct network network)
{
	printf("----- ----- ----- ----- -----\n\n");
	printf("sizes : [");
	for (size_t i = 0; i < network.nb_layers - 1; i++)
	{
		printf("%lu, ", network.sizes[i]);
	}
	printf("%lu]\n\n", network.sizes[network.nb_layers - 1]);

	printf("nb_layers : %lu\n\n", network.nb_layers);
	
	printf("biases :\n");
        for (size_t k = 0; k < network.nb_layers - 1; k++)
        {
                printf("biases[%lu] =\n", k);
                print_matrix(network.biases[k]);
        }

	printf("weights :\n");
	for (size_t k = 0; k < network.nb_layers - 1; k++)
	{
		printf("weights[%lu] =\n", k);
		print_matrix(network.weights[k]);
	}
	printf("----- ----- ----- ----- -----\n");
}

struct matrix weighted_sum(struct matrix X, struct matrix W, struct matrix b)
{
	return add(mul(W, X), b); // Z = X.W + b
}

double sigma(double x)
{
	return 1 / (1 + exp((-1) * x));
}

struct matrix sigmoid(struct matrix Z)
{
	struct matrix res = init_matrix(Z.row, 1);
	for (size_t i = 0; i < Z.row; i++)
	{
		res.data[i] = sigma(Z.data[i]);
	}
	return res;
}

struct matrix feedforward(struct network network, struct matrix a)
{
	for (size_t i = 0; i < network.nb_layers - 1; i++)
	{
		a = sigmoid(weighted_sum(a, network.weights[i], network.biases[i]));
	}
	return a;
}

struct matrix use_network_file(char* file_name, struct matrix input)
{
	struct network network = load_network(file_name);
	return feedforward(network, input);
}
