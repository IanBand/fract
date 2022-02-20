#include "complex.h"

// complex sum
struct complex c_sum(struct complex* a, struct complex* b){
    struct complex res;
    res.r = a->r + b->r;
    res.i = a->i + b->i;
    return res;
};

// complex product
struct complex c_prod(struct complex* a, struct complex* b){
    struct complex res;
    res.r = a->r * b->r - a->i * b->i;
    res.i = a->r * b->i + a->i * b->r;
    return res;
};

// complex conjugate
struct complex c_conj(struct complex* c){
    struct complex res;
    res.r = c->r;
    res.i = -1.0 * c->i;
    return res;
};

// scalar
struct complex c_scale(struct complex* c, double a){
    struct complex res;
    res.r = c->r * a;
    res.i = c->i * a;
    return res; 
};

// modulus aka absolute value
double modulus(struct complex* c){
    return sqrt(c->r * c->r + c->i * c->i);
};