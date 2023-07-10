#ifndef __SBUF_H_        //命名规则一般为xxx.h文件变为XXX_H(大写)
#define __SBUF_H_
#include "./tiny/csapp.h"
typedef struct
{
    int *buf;
    int n;
    int front;
    int rear;
    sem_t mutex;
    sem_t full;
    sem_t empty;
}sbuf_t;
void sbuf_init(sbuf_t *sp,int n);
void sbuf_deinit(sbuf_t *sp);
void sbuf_insert(sbuf_t *sp,int item);
int sbuf_remove(sbuf_t *sp);
#endif 

