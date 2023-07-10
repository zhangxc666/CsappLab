#include "sbuf.h"
void sbuf_init(sbuf_t *sp,int n){
    sp->buf=Calloc(n,sizeof n);
    sp->n=n;
    sp->front=sp->rear=0;
    Sem_init(&sp->mutex,0,1);
    Sem_init(&sp->empty,0,n);
    Sem_init(&sp->full,0,0);
}
void sbuf_deinit(sbuf_t *sp){
    Free(sp->buf);
}
void sbuf_insert(sbuf_t *sp,int item){
    P(&sp->empty);
    P(&sp->mutex);
    sp->buf[(++sp->rear)%(sp->n)]=item;
    V(&sp->full);
    V(&sp->mutex);
}
int sbuf_remove(sbuf_t *sp){
    int item;
    P(&sp->full);
    P(&sp->mutex);
    item=sp->buf[(++sp->front)%(sp->n)];
    V(&sp->mutex);
    V(&sp->empty);
    return item;
}