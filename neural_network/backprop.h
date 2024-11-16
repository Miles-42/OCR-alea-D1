#ifndef BACKPROP_H
#define BACKPROP_H

#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include "matrix.h"
#include "network.h"

struct nabla
{
        struct matrix* nabla_b;
        struct matrix* nabla_w;
};

struct nabla init_nabla(struct network network);
void print_nabla(struct nabla nabla, struct network network);
struct matrix sigmoid_prime(struct matrix Z);
struct matrix cost_derivative(struct matrix output_activations, struct matrix y);
struct nabla backprop(struct network network, struct matrix X, struct matrix Y);
void update_mini_batch(struct network network, double eta, struct matrix** mini_batch, size_t n);
void train_XOR_file(char* file_name, size_t n, double eta);
void update_network(struct network network, double eta, struct matrix* training_data);

#endif
