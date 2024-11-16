#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "matrix.h"
#include "network.h"

void serializeMatrix(struct matrix* matrix, FILE* file);
void deserializeMatrix(struct matrix* matrix, FILE* file);
void serializeNetwork(struct network* network, FILE* file);
void deserializeNetwork(struct network* network, FILE* file);
void init_network_file(char* file_name, size_t* sizes, size_t nb_layers);
struct network load_network(char* file_name);
void write_network(struct network network, char* file_name);

#endif
