#ifndef __CACHE_H_
#define __CACHE_H_
#include "./tiny/csapp.h"
typedef struct cacheunit
{
    char host[MAXLINE];     // �����������Ϊkey
    char *value;            // ���value
    char *write;            // ���嵱ǰ��buf��дָ�룬����������д����
    struct cacheunit *prev; // ˫������ǰ��
    struct cacheunit *next; // ˫��������
    int size;               // cache��Ԫ�Ĵ�С
} cacheUnit;
typedef struct
{
    cacheUnit *head; // ָ��ǰcache�ĵ�һ������
    cacheUnit *tail; // ָ��cache��β�����������
    int size;        // ��ǰcache���ܴ�С
    int readcnt;
    sem_t wmutex; // �����д��
    sem_t rmutex; // ����Ķ���
} cache;
void insertNode(cacheUnit *node, cache *cache); // ����һ��node����β
void removeNode(cacheUnit *node, cache *cache); // ɾ����ǰ��node
cacheUnit *findNode(cache *cache, char *host);
void initNode(cacheUnit *node, int size, char *url); // ��ʼ����ǰnode
void initCache(cache *cache);                        // ���Ի���ǰ��cache
#endif