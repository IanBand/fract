/*
    Written by Ian Band
    Winter 2021 
    CS475
*/

/*
-t  num threads 
-i  number of iterations used in fractal point calculation
-s  size of pixels
-j  center point imaginary component (vertical position)
-k  center point real component (horizontal position)
-d  image dimensions
-r  resolution modifier. Scales resolution while preserving image position & bounds. 
    Useful for exploring a fractal at a low resolution then generating the same image at a higher resolution.
-n  adds a user defined id to the output filename, useful for batch image creation
*/

/*
points of interest

burning ship:
-s 0.00000002 -j 0.0000007 -k -1.9519

mandelbrot:

julia:

jul = {-0.7269, 0.1889}
-j 0.1 -k 0.5 -i 1700 -s 0.00001 


*/


// mini makefile
// gcc main.c bmp.c complex.c functs.c -lm -fopenmp -std=c99 -o fract
// -DPXPADDING
// PXPADDING will corrupt the image, but makes the pixels 4 bytes instead of 3.

// tar --exclude-backups -c -a -f ./fract.tar.gz *.[ch]

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <values.h>
#include <stdbool.h>
#include <omp.h>

#include "complex.h"
#include "bmp.h"
#include "functs.h"


#ifndef NUM_THREADS
#define NUM_THREADS 8
#endif

#define CACHE_LINE_SIZE 64

#ifndef BENCHMARKING_ITERATIONS
#define BENCHMARKING_ITERATIONS 1 // number of tests to measure for benchmarking
#endif

#define CLAM // cache line boundary aligned memory

#ifndef MAX_ITER
#define MAX_ITER 100
#endif

#ifndef SIZE
#define SIZE 400 // image height & width in pixels. program only produces square images
#endif

#ifndef SCALE
#define SCALE 0.0075
#endif

#define MILLION_F 1000000.0f

//todo: add flags for julia set point & select fractal at runtime
struct complex jul = {-0.7269, 0.1889};

int main(int argc, char **argv){

    int opt = 0;
    int num_threads = NUM_THREADS;
    int max_iter = MAX_ITER;
    double scale = SCALE;
    unsigned int size = SIZE; 
    struct complex center;
    center.i = 0.0;
    center.r = -0.65;

    bool resmf = false;
    double resm = 1.0f;

    char* file_id = NULL;

    double time0 = 0.0f;
    double time1 = 0.0f;

    double max_mps = 0.0; // mega pixels per second
    double min_mps = MAXDOUBLE;
    double sum_mps = 0.0;
    
    while ((opt = getopt(argc, argv, "d:s:t:i:j:k:r:n:")) != -1) {
        switch(opt) {
        case 't':
            num_threads = atoi(optarg);
            if (num_threads < 1) {
                num_threads = NUM_THREADS;
            }
            break;
        case 'i':
            max_iter = atoi(optarg);
            if (max_iter < 0) {
                max_iter = MAX_ITER;
            }
            break;
        case 's':
            scale = atof(optarg);
            if (scale < 0) {
                scale = SCALE;
            }
            break;
        case 'd':
            size = atoi(optarg);
            if (size < 0) {
                size = SIZE;
            }
            break;
        case 'j':
            center.i = atof(optarg);
            break;
        case 'k':
            center.r = atof(optarg);
            break;
        case 'r':
            resmf = true;
            resm = atof(optarg);
            break;
        case 'n':
            file_id = optarg;
            break;
        default:
            fprintf(stderr, "Usage: %s [-t num threads] [-i iterations] "
                            "[-s scale num/px] [-j center imaginary comp] "
                            " [-k center real comp] [-d image dimensions] " 
                            "[-r resolution modifier] [-n filename id]" 
                            "\n",
                    argv[0]);
            exit(EXIT_FAILURE);
            return -1;
            break;
        }
    }

    // apply resolution modifier
    if(resmf){
        // keep the term (scale * (double) size) constant
        size = (unsigned int) (size * resm);
        scale = scale / resm;
    }

    printf("Generating a %dx%d image of %s\n", size, size, "CURVENAME");
    printf("Processing %d pixels\n", size * size);
    printf("Using %d threads\n", num_threads);
    printf("Centered at %0.17g %c %0.17gi\n", center.r, center.i >= 0 ? '+' : '-', fabs(center.i)); 
    printf("Viewing from (%0.17g, %0.17g) to (%0.17g, %0.17g)\n",
        center.r - scale * (double) (size >> 1),
        center.i - scale * (double) (size >> 1),
        center.r + scale * (double) (size >> 1),
        center.i + scale * (double) (size >> 1)
    );
    printf("Scale factor = %0.17g unit/px\n", scale);
    printf("Performing up to %d iterations per point\n", max_iter);
    if(BENCHMARKING_ITERATIONS > 1){
        printf("Benchmarking using %d runs\n", BENCHMARKING_ITERATIONS);
    }

    omp_set_num_threads(num_threads);

#ifdef CLAM
    // cache line boundary aligned
    void* pixel_buffer = malloc(sizeof(struct pixel) * size * size + CACHE_LINE_SIZE);
    int offset = (int) ((long int) pixel_buffer & 0x3f);
    struct pixel* pixels = (struct pixel*) (pixel_buffer + offset);
#else
    // no cache line boundary align
    struct pixel* pixels = malloc(sizeof(struct pixel) * size * size);
#endif
    


    for(int run = 0; run < BENCHMARKING_ITERATIONS; ++run){

        time0 = omp_get_wtime();

        #pragma omp parallel for default(none) shared(pixels, size, scale, center, max_iter, num_threads, jul)
        for(int i = 0; i < size * size; ++i){

            int self = omp_get_thread_num();
            struct complex c;
            int row = (i / size) - (size >> 1);
            int col = (i % size) - (size >> 1);
            c.i = -1 * ((double) row * scale + center.i);
            c.r =       (double) col * scale + center.r;
            
            //pixels[i] = mandelbrot_test(&c, max_iter);    
            //pixels[i] = burning_ship_test(&c, max_iter);    
            pixels[i] = julia_test(&c, &jul, max_iter);

            //graphically show compute blocks
            //pixels[i] = gradient(self * (int) (size * size / num_threads), size * size);
        }
        #pragma omp single
        {
            time1 = omp_get_wtime();

            double mps = ((double) (size * size)) / (time1 - time0) / MILLION_F; // mega pixels processed per second 

            sum_mps += mps;
            if(mps > max_mps) max_mps = mps;
            if(mps < min_mps) min_mps = mps;

            printf("run %d completed in %8.2lf sec\n", run, time1 - time0 );   
        }
    }

    char fname_buffer [50] = {0};
    if(file_id != NULL){
        sprintf(fname_buffer, "fract%s.bmp", file_id);
    }
    else{
        sprintf(fname_buffer, "fract.bmp");
    }
    write_bmp(fname_buffer, size, size, pixels);

    if(BENCHMARKING_ITERATIONS > 1){
        printf("Min Performance = %8.2lf MegaPixels/Sec\n", min_mps);
        printf("Max Performance = %8.2lf MegaPixels/Sec\n", max_mps);
        printf("Avg Performance = %8.2lf MegaPixels/Sec\n", sum_mps / BENCHMARKING_ITERATIONS);
    }
    else{
        printf("Performance = %8.2lf MegaPixels/Sec\n", sum_mps);
    }


#ifdef CLAM
    // cache line boundary aligned
    free(pixel_buffer);
#else
    // no cache line boundary align
    free(pixels);
#endif

    return EXIT_SUCCESS;
}

