#include "cache.h"

void insertNode(cacheUnit *node, cache *cache)// 插入一个node至表尾
{
    P(&cache->wmutex);
    if (cache->head == NULL)
    {
        cache->head = cache->tail = node;
    }
    else
    {
        cache->tail->next = node;
        node->prev = cache->tail;
        node->next=NULL;
        cache->tail = node;
    }
    cache->size += node->size;
    V(&cache->wmutex);
    // printf("finish insert\n");
}
void removeNode(cacheUnit *node, cache *cache)// 删除当前的node
{ 
    P(&cache->wmutex);
    if (node->prev == NULL)
    {
        cache->head = node->next;

        if (cache->tail == node)
            cache->tail = NULL;
    }
    else if (node->next == NULL)
    {
        node->prev->next = NULL;
        cache->tail = node->prev;
    }
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    cache->size -= node->size;
    V(&cache->wmutex);
    // printf("finish remove\n");
}
void initNode(cacheUnit *node, int size, char *url)// 初始化当前node
{ 
    node->value = (char *)Malloc(size);
    strcpy(node->host,url);
    // printf("\nphost:\n%s\nurl:\n%s\n",node->host,url);
    node->write = node->value;
    node->prev = NULL;
    node->next = NULL;
    node->size = 0;
}
void initCache(cache *cache)// 初始化当前的cache
{ 
    cache->head = NULL;
    cache->tail = NULL;
    cache->size = 0;
    cache->readcnt = 0;
    sem_init(&cache->wmutex, 0, 1);
    sem_init(&cache->rmutex, 0, 1);
}
cacheUnit *findNode(cache *cache, char *host)// 查找在当前cache中查找当前host是否存在
{ 
    P(&cache->rmutex);
    cache->readcnt++;
    if (cache->readcnt == 1)
        P(&cache->wmutex);
    V(&cache->rmutex);
    cacheUnit *ans = NULL;
    for (cacheUnit *p = cache->head; p; p = p->next)
    {
        if (strcmp(host, p->host) == 0)
        {
            ans = p;
            break;
        }
    }
    P(&cache->rmutex);
    cache->readcnt--;
    if (cache->readcnt == 0)
        V(&cache->wmutex);
    V(&cache->rmutex);
    return ans;
}
