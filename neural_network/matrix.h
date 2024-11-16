#ifndef MATRIX_H
#define MATRIX_H

struct matrix
{
        size_t row;
        size_t col;
        double* data;
};

void print_matrix(struct matrix A);
struct matrix init_matrix(size_t row, size_t col);
struct matrix init_matrix_data();
void free_matrix(struct matrix* matrix);
struct matrix add(struct matrix A, struct matrix B);
struct matrix sub(struct  matrix A,struct  matrix B);
struct matrix mul_scal(struct matrix A, double scal);
int scalar_product(struct matrix A, struct matrix B);
struct matrix mul(struct matrix A, struct matrix B);
struct matrix transpose(struct matrix A);
struct matrix to_vector(double k, size_t l);
struct matrix hadamard_product(struct matrix A, struct matrix B);

#endif
