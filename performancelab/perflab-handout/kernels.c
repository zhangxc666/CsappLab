/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/*
 * Please fill in the following team struct
 */
team_t team = {
    "bovik646", /* Team name */

    "Harr45k",    /* First member full name */
    "bovik@4qq.com", /* First member email address */

    " 646", /* Second member full name (leave blank if none) */
    " 64646"  /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/*
 * naive_rotate - The naive baseline version of rotate
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst)
{
    int i, j;
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[RIDX(dim - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
}

/*
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst)
{
    naive_rotate(dim, src, dst);
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/
char rotate1_desrc[] = "rotate: Optimization block8";
void rotate1(int dim, pixel *src, pixel *dst)
{
    int d=8;
    for(int i=0;i<dim;i+=d)
        for(int j=0;j<dim;j+=d)
            for(int ii=i;ii<i+d;ii++)
                for(int jj=j;jj<j+d;jj++){
                    dst[RIDX(dim-1-ii,jj, dim)] = src[RIDX(jj,ii, dim)];
                }
}
char rotate2_desrc[] = "rotate: Optimization block32";
void rotate2(int dim, pixel *src, pixel *dst)
{
    int d=32;
    for(int i=0;i<dim;i+=d)
        for(int j=0;j<dim;j+=d)
            for(int ii=i;ii<i+d;ii++)
                for(int jj=j;jj<j+d;jj++){
                    dst[RIDX(dim-1-jj,ii, dim)] = src[RIDX(ii,jj, dim)];
                }
}
char rotate3_desrc[] = "rotate: Optimization loop unrolling";
void rotate3(int dim, pixel *src, pixel *dst)
{
    int d=4;
    for (int i = 0; i < dim; i += d){
            for (int j = 0; j < dim; j += d){
                for (int ii = i; ii < i+d; ii++){
                    dst[RIDX(dim-1-j,ii, dim)]= src[RIDX(ii,j, dim)];
                    dst[RIDX(dim-2-j,ii, dim)]= src[RIDX(ii,j+1, dim)];
                    dst[RIDX(dim-3-j,ii, dim)]= src[RIDX(ii,j+2, dim)];
                    dst[RIDX(dim-4-j,ii, dim)]= src[RIDX(ii,j+3, dim)];
                    // dst[RIDX(dim-5-j,ii, dim)] = src[RIDX(ii,j+4, dim)];
                    // dst[RIDX(dim-6-j,ii, dim)] = src[RIDX(ii,j+5, dim)];
                    // dst[RIDX(dim-7-j,ii, dim)] = src[RIDX(ii,j+6, dim)];
                    // dst[RIDX(dim-8-j,ii, dim)] = src[RIDX(ii,j+7, dim)];
                }
            }
        }
}
void register_rotate_functions()
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);
    add_rotate_function(&rotate, rotate_descr);
    /* ... Register additional test functions here */
    add_rotate_function(&rotate1, rotate1_desrc);
    add_rotate_function(&rotate2,rotate2_desrc);
    add_rotate_function(&rotate3,rotate3_desrc);
}

/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct
{
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
static void initialize_pixel_sum(pixel_sum *sum)
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/*
 * accumulate_sum - Accumulates field values of p in corresponding
 * fields of sum
 */
static void accumulate_sum(pixel_sum *sum, pixel p)
{
    sum->red += (int)p.red;
    sum->green += (int)p.green;
    sum->blue += (int)p.blue;
    sum->num++;
    return;
}

/*
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum)
{
    current_pixel->red = (unsigned short)(sum.red / sum.num);
    current_pixel->green = (unsigned short)(sum.green / sum.num);
    current_pixel->blue = (unsigned short)(sum.blue / sum.num);
    return;
}

/*
 * avg - Returns averaged pixel value at (i,j)
 */
static pixel avg(int dim, int i, int j, pixel *src)
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for (ii = max(i - 1, 0); ii <= min(i + 1, dim - 1); ii++)
        for (jj = max(j - 1, 0); jj <= min(j + 1, dim - 1); jj++)
            accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst)
{
    int i, j;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth.
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst)
{
    naive_smooth(dim, src, dst);
}

/*********************************************************************
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_smooth_functions()
{
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}
