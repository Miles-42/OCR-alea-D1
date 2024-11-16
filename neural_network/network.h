#ifndef NETWORK_H
#define NETWORK_H

#include "matrix.h"

struct network
{
        size_t* sizes;
        size_t nb_layers;
        struct matrix* biases;
        struct matrix* weights;
};

struct network init_network(size_t* sizes, size_t nb_layers);
void print_network(struct network network);
struct matrix weighted_sum(struct matrix X, struct matrix W, struct matrix b);
double sigma(double x);
struct matrix sigmoid(struct matrix Z);
struct matrix feedforward(struct network network, struct matrix a);
struct matrix use_network_file(char* file_name, struct matrix input);

#endif
