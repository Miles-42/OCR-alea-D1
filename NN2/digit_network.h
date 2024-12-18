#ifndef DIGIT_NETWORK_H
#define DIGIT_NETWORK_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "maths_extra.h"
#include "matrix.h"
#include "saveLoad.h"

#define __num_inputs 28 * 28
#define __num_hidden 120
#define __num_outputs 10

void train(char *dir);

#endif
