#include "functs.h"

struct pixel gradient(int iter, int max){
    struct pixel px;

    px.R = (char) (255 * (sin( (double) iter / (double) max * 2.0 * PI + 1.0 * PI_3) + 1.0));
    px.G = (char) (255 * (sin( (double) iter / (double) max * 2.0 * PI             ) + 1.0));
    px.B = (char) (255 * (sin( (double) iter / (double) max * 2.0 * PI - 2.2 * PI_3) + 1.0));

    return px;
};

struct pixel mandelbrot_test(struct complex* c, int max_iter){
    struct complex z = {0.0,0.0};
    int i = 0;
    while( z.i * z.i + z.r * z.r <= 4 && i < max_iter){
        // f(z) = z^2 + c
        z = c_prod(&z, &z);
        z.i += c->i;
        z.r += c->r;
        ++i;
    }


    if(i == max_iter) return palette[0];

    //return palette[i % NUM_COLORS];
    return gradient(i, max_iter);
};

struct pixel julia_test(struct complex* _z, struct complex* c, int max_iter){
    struct complex z;
    z.i = _z->i;
    z.r = _z->r;
    int i = 0;
    while( z.i * z.i + z.r * z.r <= 4 && i < max_iter){
        // f(z) = z^2 + c
        z = c_prod(&z, &z);
        z.i += c->i;
        z.r += c->r;
        ++i;
    }


    if(i == max_iter) return palette[0];

    //return palette[i % NUM_COLORS];
    return gradient(i, max_iter);
};

struct pixel burning_ship_test(struct complex* c, int max_iter){
    struct complex z = {0.0,0.0};
    for(int i = 0; i < max_iter; ++i){

        z.r = fabs(z.r);
        z.i = fabs(z.i);
        z = c_prod(&z, &z);
        z.i += c->i;
        z.r += c->r;

        if(z.i > 4 || z.r > 4){
            return gradient(i, max_iter);
        }
    }
    return palette[0];
};