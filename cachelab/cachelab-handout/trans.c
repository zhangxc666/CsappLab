/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if(M==32){
        int d=8;
        for(int i=0;i<N;i+=d){
            for(int j=0;j<N;j+=d){
                for(int ii=0;ii<d;ii++){
                    int a[8]={0};
                    for(int jj=0;jj<d;jj++){
                        a[jj]=A[i+ii][j+jj];
                    }
                    for(int jj=0;jj<d;jj++){
                        B[jj+j][i+ii]=a[jj];
                    }
                }
            }
        }
    }else if(M==64){
        int d=8;
        for(int i=0;i<N;i+=d){
            for(int j=0;j<N;j+=d){
                for(int ii=0;ii<4;ii++){
                    int temp[8]={0};
                    for(int jj=0;jj<8;jj++)temp[jj]=A[i+ii][j+jj];
                    for(int jj=0;jj<4;jj++)B[j+jj][ii+i]=temp[jj],B[j+jj][ii+4+i]=temp[jj+4];
                }
                for(int ii=0;ii<4;ii++){
                    int temp[8]={0};
                    for(int jj=0;jj<4;jj++)temp[jj]=A[i+jj+4][j+ii];
                    for(int jj=0;jj<4;jj++)temp[jj+4]=B[j+ii][i+jj+4];
                    for(int jj=0;jj<4;jj++)B[j+ii][i+jj+4]=temp[jj];
                    for(int jj=0;jj<4;jj++)B[j+ii+4][i+jj]=temp[jj+4];
                }
                for(int ii=0;ii<4;ii++){
                    int temp[4]={0};
                    for(int jj=0;jj<4;jj++)temp[jj]=A[ii+i+4][jj+j+4];
                    for(int jj=0;jj<4;jj++)B[jj+j+4][ii+i+4]=temp[jj];
                }
                
            }
        }
    }else{
        int d=14;
        for(int i=0;i<N;i+=d)
            for(int j=0;j<N;j+=d){
                for(int ii=0;ii+i<N && ii<d;ii++){
                    int a[14]={0};
                    for(int jj=0;j+jj<N && jj<d;jj++){
                        a[jj]=A[i+ii][j+jj];
                    }
                    for(int jj=0;j+jj<N && jj<d;jj++){
                        B[j+jj][i+ii]=a[jj];
                    }
                }
            }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    
}

char trans1_desc[]="trans1";
void trans1(int M,int N,int A[N][M],int B[M][N]){
    if(M==N){
        int d=4;
        for(int i=0;i<N;i+=d){
            for(int j=0;j<N;j+=d){
                for(int ii=0;ii<d;ii++)
                    for(int jj=0;jj<d;jj++){
                        int jjj=jj+j,iii=ii+i;
                        B[jjj][iii]=A[iii][jjj];
                    }
            }
        }
    }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 
    // registerTransFunction(&trans1,trans1_desc);
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

