#ifndef IMAGE_H
#define IMAGE_H

Uint32 getPixel(SDL_Surface *surface, int x, int y);
struct matrix vectorize(char* image_path);
void train_one(struct network network, double eta, char* image_path, int expected);
void train_set(struct network network, double eta);
void train_file_all(char* file_name, double eta, size_t n);
void train_file_one(char* file_name, char* data_path, double eta, int res);

#endif

