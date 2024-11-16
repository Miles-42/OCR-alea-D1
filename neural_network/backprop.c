#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include "matrix.h"
#include "network.h"
#include "backprop.h"
#include "file_manager.h"

struct nabla init_nabla(struct network network)
{
        struct nabla res;
        res.nabla_b = malloc((network.nb_layers - 1) * sizeof(struct matrix*));
        for (size_t i = 0; i < network.nb_layers - 1; i++)
        {
                res.nabla_b[i] = init_matrix(network.sizes[i + 1], 1);
                for (size_t j = 0; j < network.sizes[i + 1]; j++)
                {
                        res.nabla_b[i].data[j] = 0;
                }
        }

	res.nabla_w = malloc((network.nb_layers - 1) * sizeof(struct matrix*));
        for (size_t n = 0; n < network.nb_layers - 1; n++)
        {
                res.nabla_w[n] = init_matrix(network.sizes[n + 1], network.sizes[n]);
                for (size_t p = 0; p < network.sizes[n] * network.sizes[n + 1]; p++)
                {
			res.nabla_w[n].data[p] = 0;
                }
        }
        return res;
}

void print_nabla(struct nabla nabla, struct network network)
{
	printf("----- ----- ----- ----- -----\n");
	printf("nabla_b :\n");
        for (size_t k = 0; k < network.nb_layers - 1; k++)
        {
                printf("nabla_b[%lu] =\n", k);
                print_matrix(nabla.nabla_b[k]);
        }

        printf("nabla_w :\n");
        for (size_t k = 0; k < network.nb_layers - 1; k++)
        {
                printf("nabla_w[%lu] =\n", k);
                print_matrix(nabla.nabla_w[k]);
        }
	printf("----- ----- ----- ----- -----\n");
}

struct matrix sigmoid_prime(struct matrix Z)
{
	if (Z.col != 1)
		err(EXIT_FAILURE, "sigmoid_prime() : Z.col != 1");
        struct matrix res = init_matrix(Z.row, 1);
        double var;
        for (size_t i = 0; i < Z.row; i++)
        {

                var = sigma(Z.data[i]);
                res.data[i] = var * (1 - var);
        }
        return res;
}

struct matrix cost_derivative(struct matrix output_activations, struct matrix y)
{
        return sub(output_activations, y);
}

struct nabla backprop(struct network network, struct matrix X, struct matrix Y)
{

        struct nabla nabla = init_nabla(network);
        struct matrix activation = X;

	struct matrix* activations = malloc(network.nb_layers * sizeof(struct matrix));
        activations[0] = X;

	struct matrix* zs = malloc((network.nb_layers - 1) * sizeof(struct matrix));
	struct matrix Z;
	for (size_t i = 0; i < network.nb_layers - 1; i++)
        {
                Z = weighted_sum(activation, network.weights[i], network.biases[i]);
                zs[i] = Z;
                activation = sigmoid(Z);
                activations[i + 1] = activation;
        }

        struct matrix delta = hadamard_product(cost_derivative(activations[network.nb_layers - 1], Y), sigmoid_prime(zs[network.nb_layers - 2]));
        nabla.nabla_b[network.nb_layers - 2] = delta;
        nabla.nabla_w[network.nb_layers - 2] = mul(delta, transpose(activations[network.nb_layers - 2]));
	struct matrix sp;
	for (size_t j = 2; j < network.nb_layers; j++)
	{
		Z = zs[network.nb_layers - 1 - j];
		sp = sigmoid_prime(Z);
		delta = hadamard_product(mul(transpose(network.weights[network.nb_layers - 1 - j + 1]), delta), sp);
		nabla.nabla_b[network.nb_layers - 1 - j] = delta;
		nabla.nabla_w[network.nb_layers - 1 - j] = mul(delta, transpose(activations[network.nb_layers - j - 1]));
	}
	
	return nabla;
}

void update_mini_batch(struct network network, double eta, struct matrix** mini_batch, size_t n)
{
	struct nabla nabla = init_nabla(network);
	struct nabla delta_nabla;
	for (size_t i = 0; i < n; i++)
	{
		delta_nabla = backprop(network, mini_batch[i][0], mini_batch[i][1]);
		for (size_t j = 0; j < network.nb_layers - 1; j++)
		{
			nabla.nabla_b[j] = add(nabla.nabla_b[j], delta_nabla.nabla_b[j]);
			nabla.nabla_w[j] = add(nabla.nabla_w[j], delta_nabla.nabla_w[j]);	
		}
	}

	for (size_t k = 0; k < network.nb_layers - 1; k++)
	{
		network.weights[k] = sub(network.weights[k], mul_scal(nabla.nabla_w[k], (eta / (double)n)));
		network.biases[k] = sub(network.biases[k], mul_scal(nabla.nabla_b[k], (eta / (double)n)));
	}
}

void update_network(struct network network, double eta, struct matrix* training_data)
{

	struct nabla nabla = init_nabla(network);

	struct nabla delta_nabla = backprop(network, training_data[0], training_data[1]);

	for (size_t j = 0; j < network.nb_layers - 1; j++)
	{
		nabla.nabla_b[j] = add(nabla.nabla_b[j], delta_nabla.nabla_b[j]);
		nabla.nabla_w[j] = add(nabla.nabla_w[j], delta_nabla.nabla_w[j]);	
	}

	for (size_t k = 0; k < network.nb_layers - 1; k++)
	{
		network.weights[k] = sub(network.weights[k], mul_scal(nabla.nabla_w[k], eta));
		network.biases[k] = sub(network.biases[k], mul_scal(nabla.nabla_b[k], eta));
	}

}

void train_XOR_file(char* file_name, size_t n, double eta)
{
	struct network network = load_network(file_name);
	
        double data_a1[] = { 0, 0 };
        struct matrix a1 = init_matrix_data(2, 1, data_a1);
        double data_y1[] = { 0 };
        struct matrix y1 = init_matrix_data(1, 1, data_y1);
        struct matrix test1[] = { a1, y1 };

        double data_a2[] = { 0, 1 };
        struct matrix a2 = init_matrix_data(2, 1, data_a2);
        double data_y2[] = { 1 };
        struct matrix y2 = init_matrix_data(1, 1, data_y2);
        struct matrix test2[] = { a2, y2 };

        double data_a3[] = { 1, 0 };
        struct matrix a3 = init_matrix_data(2, 1, data_a3);
        double data_y3[] = { 1 };
        struct matrix y3 = init_matrix_data(1, 1, data_y3);
        struct matrix test3[] = { a3, y3 };

        double data_a4[] = { 1, 1 };
        struct matrix a4 = init_matrix_data(2, 1, data_a4);
        double data_y4[] = { 0 };
        struct matrix y4 = init_matrix_data(1, 1, data_y4);
        struct matrix test4[] = { a4, y4 };

        struct matrix* mini_batch1[] = { test1 };
        struct matrix* mini_batch2[] = { test2 };
        struct matrix* mini_batch3[] = { test3 };
        struct matrix* mini_batch4[] = { test4 };

        struct matrix** big_mini_batch[] = { mini_batch1, mini_batch2, mini_batch3, mini_batch4 };
	
	for (size_t i = 0; i < n; i++)
        {
                for (size_t j = 0; j < 4; j++)
                {
                        update_mini_batch(network, eta, big_mini_batch[j], 1);
                }
        }

	write_network(network, file_name);
}
