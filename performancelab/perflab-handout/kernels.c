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
#define COPY(d,s) *(d)=*(s)
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
    int d=16;
    for(int i=0;i<dim;i+=d)
        for(int j=0;j<dim;j+=d)
            for(int ii=i;ii<i+d;ii++){
                int del=ii*dim,del2=(dim-1-j)*dim+ii;
                for(int jj=j;jj<j+d;jj++,del2-=dim){
                    dst[del2] = src[del+jj];
                }
            }
}
char rotate3_desrc[] = "rotate: Optimization loop unrolling";
void rotate3(int dim, pixel *src, pixel *dst)
{
    int d=16;
    for (int i = 0; i < dim; i += d){
            for (int j = 0; j < dim; j += d){
                for (int ii = i; ii < i+d; ii++){
                    pixel*d=dst+RIDX(dim-1-j,ii,dim);
                    pixel*s=src+RIDX(ii,j,dim);
                    COPY(d,s);d=d-dim;
                    COPY(d,s+1);d=d-dim;
                    COPY(d,s+2);d=d-dim;
                    COPY(d,s+3);d=d-dim;
                    COPY(d,s+4);d=d-dim;
                    COPY(d,s+5);d=d-dim;
                    COPY(d,s+6);d=d-dim;
                    COPY(d,s+7);d=d-dim;
                    COPY(d,s+8);d=d-dim;
                    COPY(d,s+9);d=d-dim;
                    COPY(d,s+10);d=d-dim;
                    COPY(d,s+11);d=d-dim;
                    COPY(d,s+12);d=d-dim;
                    COPY(d,s+13);d=d-dim;
                    COPY(d,s+14);d=d-dim;
                    COPY(d,s+15);
                    // int del=ii*dim+j;
                    // dst[RIDX(dim-1-j,ii, dim)]= src[del];
                    // dst[RIDX(dim-2-j,ii, dim)]= src[del+1];
                    // dst[RIDX(dim-3-j,ii, dim)]= src[del+2];
                    // dst[RIDX(dim-4-j,ii, dim)]= src[del+3];
                    // dst[RIDX(dim-5-j,ii, dim)] = src[del+4];
                    // dst[RIDX(dim-6-j,ii, dim)] = src[del+5];
                    // dst[RIDX(dim-7-j,ii, dim)] = src[del+6];
                    // dst[RIDX(dim-8-j,ii, dim)] = src[del+7];
                }
            }
        }
}
//宏定义一个复制函数，方便程序编写

char rotate4_descr[] = "rotate4";
void rotate4( int dim,pixel *src,pixel *dst)
{
   int i,j;
   for(i=0;i<dim;i+=32)//32路循环展开，32个像素依次存储
         for(j=dim-1;j>=0;j-=1)
            {       
                pixel*dptr=dst+RIDX(dim-1-j,i,dim);
                pixel*sptr=src+RIDX(i,j,dim);
                COPY(dptr,sptr);sptr+=dim;
                COPY(dptr+1,sptr);sptr+=dim;
                COPY(dptr+2,sptr);sptr+=dim;
                COPY(dptr+3,sptr);sptr+=dim;
                COPY(dptr+4,sptr);sptr+=dim;
                COPY(dptr+5,sptr);sptr+=dim;
                COPY(dptr+6,sptr);sptr+=dim;
                COPY(dptr+7,sptr);sptr+=dim;
                COPY(dptr+8,sptr);sptr+=dim;
                COPY(dptr+9,sptr);sptr+=dim;
                COPY(dptr+10,sptr);sptr+=dim;
                COPY(dptr+11,sptr);sptr+=dim;
                COPY(dptr+12,sptr);sptr+=dim;
                COPY(dptr+13,sptr);sptr+=dim;
                COPY(dptr+14,sptr);sptr+=dim;
                COPY(dptr+15,sptr);sptr+=dim;
                COPY(dptr+16,sptr);sptr+=dim;
                COPY(dptr+17,sptr);sptr+=dim;
                COPY(dptr+18,sptr);sptr+=dim;
                COPY(dptr+19,sptr);sptr+=dim;
                COPY(dptr+20,sptr);sptr+=dim;
                COPY(dptr+21,sptr);sptr+=dim;
                COPY(dptr+22,sptr);sptr+=dim;
                COPY(dptr+23,sptr);sptr+=dim;
                COPY(dptr+24,sptr);sptr+=dim;
                COPY(dptr+25,sptr);sptr+=dim;
                COPY(dptr+26,sptr);sptr+=dim;
                COPY(dptr+27,sptr);sptr+=dim;
                COPY(dptr+28,sptr);sptr+=dim;
                COPY(dptr+29,sptr);sptr+=dim;
                COPY(dptr+30,sptr);sptr+=dim;
                COPY(dptr+31,sptr);
            }
}
void register_rotate_functions()
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);
    // add_rotate_function(&rotate, rotate_descr);
    /* ... Register additional test functions here */
    // add_rotate_function(&rotate1, rotate1_desrc);
    // add_rotate_function(&rotate2,rotate2_desrc);
    add_rotate_function(&rotate3,rotate3_desrc);
    add_rotate_function(&rotate4,rotate4_descr);
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

char smooth2_descr[]="smooth: Reducing function call";
void smooth2(int dim,pixel *src, pixel *dst){
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
        {
            int red=0,blue=0,green=0,count=0;
            for (int ii = max(i - 1, 0); ii <= min(i + 1, dim - 1); ii++)
            for (int jj = max(j - 1, 0); jj <= min(j + 1, dim - 1); jj++)
            {
                pixel *s=src+RIDX(ii,jj,dim);
                red+=s->red;
                blue+=s->blue;
                green+=s->green;
                count++;
            }
            pixel *d=dst+RIDX(i,j,dim);
            d->blue=blue/count;
            d->red=red/count;
            d->green=green/count;
        }

}
char smooth3_descr[]="smooth3";
void smooth3(int dim,pixel *src, pixel *dst){
    // 右下角
    pixel *d=dst+RIDX(dim-1,dim-1,dim);
    pixel *s=src+RIDX(dim-1,dim-1,dim);
    d->blue=(s->blue+(s-1)->blue+(s-dim)->blue+(s-1-dim)->blue)/4;
    d->red=(s->red+(s-1)->red+(s-dim)->red+(s-1-dim)->red)/4;
    d->green=(s->green+(s-1)->green+(s-dim)->green+(s-1-dim)->green)/4;

    // 右上角
    d=dst+RIDX(0,dim-1,dim);
    s=src+RIDX(0,dim-1,dim);
    d->blue=(s->blue+(s-1)->blue+(s+dim)->blue+(s-1+dim)->blue)/4;
    d->red=(s->red+(s-1)->red+(s+dim)->red+(s-1+dim)->red)/4;
    d->green=(s->green+(s-1)->green+(s+dim)->green+(s-1+dim)->green)/4;

    // 左下角
    d=dst+RIDX(dim-1,0,dim);
    s=src+RIDX(dim-1,0,dim);
    d->blue=(s->blue+(s+1)->blue+(s-dim)->blue+(s+1-dim)->blue)/4;
    d->red=(s->red+(s+1)->red+(s-dim)->red+(s+1-dim)->red)/4;
    d->green=(s->green+(s+1)->green+(s-dim)->green+(s+1-dim)->green)/4;
    
    // 左上角
    d=dst+RIDX(0,0,dim);
    s=src+RIDX(0,0,dim);
    d->blue=(s->blue+(s+1)->blue+(s+dim)->blue+(s+1+dim)->blue)/4;
    d->red=(s->red+(s+1)->red+(s+dim)->red+(s+1+dim)->red)/4;
    d->green=(s->green+(s+1)->green+(s+dim)->green+(s+1+dim)->green)/4;

    // 上边
    d=dst+RIDX(0,1,dim);
    s=src+RIDX(0,1,dim);
    for(int j=1;j<dim-1;j++){
        d->blue=(s->blue+(s-1)->blue+(s+1)->blue+(s+dim)->blue+(s+1+dim)->blue+(s-1+dim)->blue)/6;
        d->red=(s->red+(s-1)->red+(s+1)->red+(s+dim)->red+(s+1+dim)->red+(s-1+dim)->red)/6;
        d->green=(s->green+(s-1)->green+(s+1)->green+(s+dim)->green+(s+1+dim)->green+(s-1+dim)->green)/6;
        d++,s++;
    }
    // 下边
    d=dst+RIDX(dim-1,1,dim);
    s=src+RIDX(dim-1,1,dim);
    for(int j=1;j<dim-1;j++){
        d->blue=(s->blue+(s-1)->blue+(s+1)->blue+(s-dim)->blue+(s+1-dim)->blue+(s-1-dim)->blue)/6;
        d->red=(s->red+(s-1)->red+(s+1)->red+(s-dim)->red+(s+1-dim)->red+(s-1-dim)->red)/6;
        d->green=(s->green+(s-1)->green+(s+1)->green+(s-dim)->green+(s+1-dim)->green+(s-1-dim)->green)/6;
        d++,s++;
    }
    // 左边
    d=dst+RIDX(1,0,dim);
    s=src+RIDX(1,0,dim);
    for(int i=1;i<dim-1;i++){
        d->blue=(s->blue+(s+1)->blue+(s+1+dim)->blue+(s+dim)->blue+(s+1-dim)->blue+(s-dim)->blue)/6;
        d->red=(s->red+(s+1)->red+(s+1+dim)->red+(s+dim)->red+(s+1-dim)->red+(s-dim)->red)/6;
        d->green=(s->green+(s+1)->green+(s+1+dim)->green+(s+dim)->green+(s+1-dim)->green+(s-dim)->green)/6;
        d+=dim;
        s+=dim;
    }
    // 右边
    d=dst+RIDX(1,dim-1,dim);
    s=src+RIDX(1,dim-1,dim);
    for(int i=1;i<dim-1;i++){
        d->blue=(s->blue+(s-1)->blue+(s-1+dim)->blue+(s+dim)->blue+(s-1-dim)->blue+(s-dim)->blue)/6;
        d->red=(s->red+(s-1)->red+(s-1+dim)->red+(s+dim)->red+(s-1-dim)->red+(s-dim)->red)/6;
        d->green=(s->green+(s-1)->green+(s-1+dim)->green+(s+dim)->green+(s-1-dim)->green+(s-dim)->green)/6;
        d+=dim;
        s+=dim;
    }

    for(int i=1;i<dim-1;i++)
        for(int j=1;j<dim-1;j++){
            d=dst+RIDX(i,j,dim);
            s=src+RIDX(i,j,dim);
            d->blue=((s)->blue+(s+1)->blue+(s-1)->blue+(s-dim)->blue+(s+1-dim)->blue+(s-1-dim)->blue+(s+dim)->blue+(s+1+dim)->blue+(s-1+dim)->blue)/9;
            d->red=((s)->red+(s+1)->red+(s-1)->red+(s-dim)->red+(s+1-dim)->red+(s-1-dim)->red+(s+dim)->red+(s+1+dim)->red+(s-1+dim)->red)/9;
            d->green=((s)->green+(s+1)->green+(s-1)->green+(s-dim)->green+(s+1-dim)->green+(s-1-dim)->green+(s+dim)->green+(s+1+dim)->green+(s-1+dim)->green)/9;
        }
    // for (int i = 0; i < dim; i++)
    //     for (int j = 0; j < dim; j++)
    //     {
    //         int red=0,blue=0,green=0,count=0;
    //         for (int ii = max(i - 1, 0); ii <= min(i + 1, dim - 1); ii++)
    //         for (int jj = max(j - 1, 0); jj <= min(j + 1, dim - 1); jj++)
    //         {
    //             pixel *s=src+RIDX(ii,jj,dim);
    //             red+=s->red;
    //             blue+=s->blue;
    //             green+=s->green;
    //             count++;
    //         }
    //         pixel *d=dst+RIDX(i,j,dim);
    //         d->blue=blue/count;
    //         d->red=red/count;
    //         d->green=green/count;
    //     }

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
    // add_smooth_function(&naive_smooth, naive_smooth_descr);
    add_smooth_function(&smooth2,smooth2_descr);
    add_smooth_function(&smooth3,smooth3_descr);
}
