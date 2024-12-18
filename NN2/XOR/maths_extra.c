#include <math.h>
#include "../maths_extra.h"

double abs_val(double x)
{
    if (x < 0)
    {
        return -x;
    }
    return x;
}

static const double EPSILON = 0.00000001;

bool double_equals(double a, double b)
{
    return abs_val(a - b) < EPSILON;
}

double sigmoid(double x)
{
    return (1.0 / (1.0 + exp(-x)));
}

double d_sigmoid(double x)
{
    return (x * (1.0 - x));
}

double rand_weight()
{
    return ((double)rand()) / ((double)RAND_MAX);
}

double rand_input_XOR()
{
    double res = ((double)rand()) / ((double)RAND_MAX);
    if (res > 0.5)
    {
        return 0.1;
    }
    return 0.0;
}

double relu(double x)
{
    /*
    if(x >= 0){
        return 1.0;
    }
    else{
        return 0.01 * x;
    }*/
    return 0.01 * x;
}

double d_relu()
{
    /*if(x >= 0){
        return 1;
    }
    else{
        return 0.01;
    }*/
    return 0.01;
}
