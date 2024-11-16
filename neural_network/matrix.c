#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <err.h>
#include "matrix.h"

void print_matrix(struct matrix A)
{
	for (size_t i = 0; i < A.row; i++)
	{
		for (size_t j = 0; j < A.col; j++)
		{
			printf("%lf     ", A.data[i * A.col + j]);
		}
		printf("\n");
	}
	printf("\n");
}


struct matrix init_matrix(size_t row, size_t col)
{
	struct matrix res;
	res.row = row;
	res.col = col;
	res.data = malloc((row * col + 1) * sizeof(double));
	res.data[row * col] = '\0';
	return res;
}

struct matrix init_matrix_data(size_t row, size_t col, double* data)
{
	struct matrix res;
	res.row = row;
	res.col = col;
	res.data = data;
	return res;
}

void free_matrix(struct matrix* matrix)
{
	free(matrix->data);
	matrix->data = NULL;
	free(matrix);
}

struct matrix add(struct matrix A, struct matrix B)
{
	if (A.row != B.row || A.col != B.col)
		err(EXIT_FAILURE, "add() : wrong dimentions : A.row = %lu, A.col = %lu, B.row = %lu, B.col = %lu", A.row, A.col, B.row, B.col);

	struct matrix res = init_matrix(A.row, A.col);

	for (size_t i = 0; i < A.row * A.col; i++)
		res.data[i] = A.data[i] + B.data[i];

	return res;
}

struct matrix sub(struct  matrix A,struct  matrix B)
{
        if (A.row != B.row || A.col != B.col)
                err(EXIT_FAILURE, "sub() : wrong dimentions : A.row = %lu, A.col = %lu, B.row = %lu, B.col = %lu", A.row, A.col, B.row, B.col);

        struct matrix res = init_matrix(A.row, A.col);

        for (size_t i = 0; i < A.row * A.col; i++)
                res.data[i] = A.data[i] - B.data[i];

        return res;
}

struct matrix mul_scal(struct matrix A, double scal) //not really a scalar but well...
{
	struct matrix res = init_matrix(A.row, A.col);
	for (size_t i = 0; i < A.row * A.col; i++)
		res.data[i] = A.data[i] * scal;
	return res;
}

int scalar_product(struct matrix A, struct matrix B)
{
	if (A.row != 1 || B.row != 1)
		err(EXIT_FAILURE, "scalar_product() : vectors only (row = 1)");
        if (A.col != B.col)
                err(EXIT_FAILURE, "scalar_product() : vectors sizes are different (A.col = %lu, B.col = %lu", A.col, B.col);

	double res = 0;
	for (size_t i = 0; i < A.col; i++)
	{
		res += A.data[i] * B.data[i];
	}
	return res;
}

struct matrix mul(struct matrix A, struct matrix B)
{
	if (A.col != B.row)
		err(EXIT_FAILURE, "mul() : A.col != B.row (A.col = %lu, B.row = %lu)", A.col, B.row);
	
	struct matrix res = init_matrix(A.row, B.col);
	double var = 0;
	for (size_t r = 0; r < res.row; r++)
	{
		for (size_t c = 0; c < res.col; c++)
		{
			var = 0;
			for (size_t k = 0; k < A.col; k++)
			{
				var += A.data[r * A.col + k] * B.data[k * B.col + c];
			}
			res.data[r * res.col + c] = var;
		}
	}
	return res;
}

struct matrix transpose(struct matrix A)
{
	struct matrix res = init_matrix(A.col, A.row);
	for (size_t i = 0; i < A.row; i++)
	{
		for (size_t j = 0; j < A.col; j++)
		{
			res.data[j * res.col + i] = A.data[i * A.col + j];
		}
	}
	return res;
}

struct matrix to_vector(double k, size_t l)
{
	struct matrix res = init_matrix(l, 1);;
	for (size_t i = 0; i < l; i++)
	{
		res.data[i] = k;
	}
	return res;
}

struct matrix hadamard_product(struct matrix A, struct matrix B)
{
	if (A.col != 1 || B.col != 1)
                err(EXIT_FAILURE, "hadamard_product() : col != 1");
        if (A.row != B.row)
                err(EXIT_FAILURE, "hadamard_product() : A.row != B.row (A.row = %lu, B.row = %lu", A.row, B.row);
	
	struct matrix res = init_matrix(A.row, 1);
	for (size_t i = 0; i < A.row; i++)
	{
		res.data[i] = A.data[i] * B.data[i];
	}
	return res;
}
