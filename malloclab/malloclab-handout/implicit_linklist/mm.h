#include <stdio.h>

extern int mm_init(void);
extern void *mm_malloc(size_t size);
extern void mm_free(void *ptr);
extern void *mm_realloc(void *ptr, size_t size);
static void *extend_heap(size_t);        // 给堆增加size个字节
static void *coalesce(void *);           // 合并当前块的上下块
static void *find_fit(size_t);           // 对隐式空闲链表进行首次适配搜索
static void *next_fit(size_t);
static void place(void *, size_t);       // 分割当前获取的空闲块
static size_t getBlockSize(size_t size); // size是payload，返回实际开辟的块的大小
static void debugAllBlock();      // 打印当前所有的块的情况
/*
 * Students work in teams of one or two.  Teams enter their team name,
 * personal names and login IDs in a struct of this
 * type in their bits.c file.
 */
typedef struct
{
    char *teamname; /* ID1+ID2 or ID1 */
    char *name1;    /* full name of first member */
    char *id1;      /* login ID of first member */
    char *name2;    /* full name of second member (if any) */
    char *id2;      /* login ID of second member */
} team_t;

extern team_t team;
static char *heap_listp;
static char *next_fit_bp;
