#include "utils.h"
#include <stdlib.h>
#include <math.h>

#define DEFAULT_EPSILON 0.001

// Linearly interpolates between two double values by interpolant t. Doesn't modify any values, only returns the result.
double lerp(double a, double b, double t)
{
    if(t < 0) { t = 0; }
    if(t > 1) { t = 1; }
    return a*(1-t) + b*t;
}

// Returns true if the values are as close or closer than epsilon.
bool epsilon_equals(double a, double b, double epsilon)
{
    return fabs(a - b) < epsilon;
}

// Returns true if the values are as close or closer than an epsilon value.
bool epsilon_equals(double a, double b)
{
    return fabs(a - b) < DEFAULT_EPSILON;
}

// Returns true if the values are as close or closer than an epsilon value.
bool rounded_equals(double a, double b)
{
    return round(a) == round(b);
}
