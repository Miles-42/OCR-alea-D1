#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include "matrix.h"
#include "network.h"
#include "backprop.h"
#include "file_manager.h"
#include "image.h"

void exit_help()
{
	char help[] =
		"Usage:\n"
		"--demo <-d : print network> <iterations>\n"
		"--init <file>\n"
		"--train <file> <iterations> <learning rate>\n"
		"--print <file>\n"
		"--xor <file>";

	errx(EXIT_FAILURE, "%s", help);
}

void demo(int argc, char** argv)
{
	if (argc > 4)
		exit_help();

	size_t n = 10000;
	int d = 0;

	if (argc == 3)
	{
		if (strcmp(argv[2], "-d") == 0)
			d = 1;
		else
			n = (size_t)atoi(argv[2]);
	}

	if (argc == 4)
	{
		if (strcmp(argv[2], "-d") == 0)
			d = 1;
		else
			exit_help();
		n = (size_t)atoi(argv[3]);
	}

	size_t sizes[] = { 2, 12, 1 };
	struct network network = init_network(sizes, 3);

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

	double eta = 0.1;


	printf("---------------------------\nInitialisation\n---------------------------\n");

	if (d == 1)
	{
		printf("Starting network :\n");
		print_network(network);
	}

	printf("test (0, 0) :\n");
	print_matrix(feedforward(network, a1));
	printf("test (0, 1) :\n"); 
        print_matrix(feedforward(network, a2));
	printf("test (1, 0) :\n"); 
        print_matrix(feedforward(network, a3));
	printf("test (1, 1) :\n"); 
        print_matrix(feedforward(network, a4));

	printf("---------------------------\nTraining ...\n---------------------------\n");	

	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			update_mini_batch(network, eta, big_mini_batch[j], 1);
		}
	}
	
	if (d == 1)
	{
		printf("New network :\n");
		print_network(network);
	}

	printf("test (0, 0) :\n");
        print_matrix(feedforward(network, a1));
        printf("test (0, 1) :\n");
        print_matrix(feedforward(network, a2));
        printf("test (1, 0) :\n");
        print_matrix(feedforward(network, a3));
        printf("test (1, 1) :\n");
        print_matrix(feedforward(network, a4));
}

int init_file(int argc, char** argv)
{
	if (argc != 3)
		exit_help();


	size_t sizes[] = { 2, 5, 1 };
	init_network_file(argv[2], sizes, 3);
	return 1;
}

int train_file(int argc, char** argv)
{
	if (argc < 3 || argc > 5)
		exit_help();

	size_t n = 100;
	double eta = 0.1;

	if (argc == 4)
		n = strtod(argv[3], NULL);

	if (argc == 5)
	{
		n = (size_t)atoi(argv[3]);
		eta = strtod(argv[4], NULL);
	}

	train_XOR_file(argv[2], n, eta);
	return 1;
}

int print_network_file(int argc, char** argv)
{
	if (argc != 3)
		exit_help();
	
	struct network network = load_network(argv[2]);
	print_network(network);
	return 1;
}

int test_XOR_file(int argc, char** argv)
{
	if (argc != 3)
		exit_help();
	
	struct network network = load_network(argv[2]);
	
	double data_a1[] = { 0, 0 };
        struct matrix a1 = init_matrix_data(2, 1, data_a1);
        
	double data_a2[] = { 0, 1 };
        struct matrix a2 = init_matrix_data(2, 1, data_a2);
	
	double data_a3[] = { 1, 0 };
        struct matrix a3 = init_matrix_data(2, 1, data_a3);
       
        double data_a4[] = { 1, 1 };
        struct matrix a4 = init_matrix_data(2, 1, data_a4);
        
	printf("test (0, 0) :\n");
        print_matrix(feedforward(network, a1));
        printf("test (0, 1) :\n");
        print_matrix(feedforward(network, a2));
        printf("test (1, 0) :\n");
        print_matrix(feedforward(network, a3));
        printf("test (1, 1) :\n");
        print_matrix(feedforward(network, a4));

	return 1;
}

int main(int argc, char** argv)
{
	/*
	if (argc == 1)
		exit_help();
	
	if (strcmp(argv[1], "--demo") == 0)
		demo(argc, argv);
	else if (strcmp(argv[1], "--init") == 0)
		init_file(argc, argv);	
	else if (strcmp(argv[1], "--train") == 0)
		train_file(argc, argv);
	else if (strcmp(argv[1], "--print") == 0)
		print_network_file(argc, argv);
	else if (strcmp(argv[1], "--xor") == 0)
		test_XOR_file(argc, argv);	
	else
		exit_help();
*/

	//print_matrix(vectorize("blanc.png"));


	//size_t sizes[3] = { 784, 10, 10 };
	//init_network_file("network.bin", sizes, 3);
	train_file_all("network.bin", 0.1, 1);//, "images/0/0_Aller_Rg.png", 0.1, 0);



	//printf("res = %i\n", use_file("network.bin", "images/0/0_Aller_Rg.png"));


	return EXIT_SUCCESS;
}

