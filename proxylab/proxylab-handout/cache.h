#ifndef __CACHE_H_
#define __CACHE_H_
#include "./tiny/csapp.h"
typedef struct cacheunit
{
    char host[MAXLINE];     // 存放域名，作为key
    char *value;            // 存放value
    char *write;            // 定义当前的buf的写指针，不断往里面写数据
    struct cacheunit *prev; // 双向链表前驱
    struct cacheunit *next; // 双向链表后继
    int size;               // cache单元的大小
} cacheUnit;
typedef struct
{
    cacheUnit *head; // 指向当前cache的第一个表结点
    cacheUnit *tail; // 指向cache的尾，方便插如结点
    int size;        // 当前cache的总大小
    int readcnt;
    sem_t wmutex; // 链表的写锁
    sem_t rmutex; // 链表的读锁
} cache;
void insertNode(cacheUnit *node, cache *cache); // 插入一个node至表尾
void removeNode(cacheUnit *node, cache *cache); // 删除当前的node
cacheUnit *findNode(cache *cache, char *host);
void initNode(cacheUnit *node, int size, char *url); // 初始化当前node
void initCache(cache *cache);                        // 初试话当前的cache
#endif