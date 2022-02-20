#ifndef __COMPLEX_H__
#define __COMPLEX_H__
#include <math.h>

// complex number
struct complex {
    double r; // real component
    double i; // imaginary component
};

// complex sum
struct complex c_sum(struct complex*, struct complex*);

// complex product
struct complex c_prod(struct complex*, struct complex*);

// complex conjugate
struct complex c_conj(struct complex*);

// scalar
struct complex c_scale(struct complex*, double);

// modulus aka absolute value
double modulus(struct complex*);

#endif