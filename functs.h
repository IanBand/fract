#ifndef __FUNCTS_H__
#define __FUNCTS_H__

#include "bmp.h"
#include "complex.h"

#define PI 3.14159265359
#define PI_3    (PI / 3)

static struct pixel palette[5] = {
    {0,0,0}, //B G R
    {255,255,255},
    {255,0,0},
    {0,255,0},
    {0,0,255}
};

struct pixel gradient(int, int);

struct pixel mandelbrot_test(struct complex*, int);

struct pixel julia_test(struct complex*,struct complex*, int);

struct pixel burning_ship_test(struct complex*, int);

#endif