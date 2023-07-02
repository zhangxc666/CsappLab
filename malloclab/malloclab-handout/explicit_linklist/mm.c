/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ateam",
    /* First member's full name */
    "Harry Bovik",
    /* First member's email address */
    "bovik@cs.cmu.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))
#define WSIZE 4 // 单字长
#define DSIZE 8 // 双字长
#define MINSIZE 16
#define CHUNKSIZE (1 << 12)                                         // 初始化堆时的大小和扩展堆的默认大小
#define MAX(x, y) (((x) > (y)) ? (x) : (y))                         // max操作
#define PACK(size, alloc) ((size) | (alloc))                        // 将当前size和alloc值组合
#define GET(x) (*(unsigned int *)(x))                               // 取出当前x地址的值
#define PUT(x, val) (*(unsigned int *)(x) = (val))                  // 将x地址的值置为val
#define GET_SIZE(x) ((GET(x)) & (~0x7))                             // 得到x地址块的大小
#define GET_ALLOC(x) ((GET(x)) & 0x1)                               // 得到x地址块的分配位
#define HDRP(bp) ((char *)(bp)-WSIZE)                               // 获取当前块的头部
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)        // 获取当前块的尾部
#define NEXT_BLKP(bp) ((char *)bp + GET_SIZE(((char *)(bp)-WSIZE))) // 获取下一块的地址
#define PREV_BLKP(bp) ((char *)bp - GET_SIZE(((char *)(bp)-DSIZE))) // 获取上一块的地址

#define PRECESSOR_P(bp) (bp)                    // 获取当前结点的prev
#define SUCCESSOR_P(bp) ((char *)bp+WSIZE)      // 获取当前结点的next
#define GET_PRECESSOR(bp) ((void *)(*(unsigned int *)(PRECESSOR_P(bp))))      // 获取当前结点的prev的值
#define GET_SUCCESSOR(bp) ((void *)(*(unsigned int *)(SUCCESSOR_P(bp))))      // 获取当前结点的next的值

/*
 * mm_init - initialize the malloc package.
 */
static void insertLinkNode(void *bp){
    if(bp==NULL)return;
    PUT(SUCCESSOR_P(bp),NULL);
    PUT(PRECESSOR_P(bp),NULL);
    if(head!=NULL){ // 若当前链表为空
        PUT(PRECESSOR_P(head),bp); // 设置原来的第一个结点的前驱指向新插入的结点
        PUT(SUCCESSOR_P(bp),head); // 设置新结点后继->原来的第一个结点
    }
    head=bp;
    // printDoubleLinkList();

}
static void removeLinkNode(void *bp){
    if(bp==NULL || GET_ALLOC(HDRP(bp)))return;
    void *prev=GET_PRECESSOR(bp);
    void *next=GET_SUCCESSOR(bp);
    PUT(SUCCESSOR_P(bp),0);
    PUT(PRECESSOR_P(bp),0);
    if(prev==NULL && next==NULL){ // 若当前仅有一个结点
        head=NULL;
    }
    else if(prev==NULL){ // 若当前是第一个结点
        PUT(PRECESSOR_P(next),0);
        head=next;
    }
    else if(next==NULL){ // 若当前是最后一个结点
        PUT(SUCCESSOR_P(prev),0);
    }
    else{ // 删除双链表中的一个结点（正常）
        PUT(SUCCESSOR_P(prev),next);
        PUT(PRECESSOR_P(next),prev);
    }
}


static void *extend_heap(size_t words)
{
    char *bp;
    size_t size;
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE; // 地址按双字对齐，若奇数，字节再加1
    if ((long)(bp = mem_sbrk(size)) == -1)
    {
        return NULL;
    }
    // printf("--打印操作：进行 extend_heap 操作，扩充 %d 大小的内存，地址为 %p \n",size,bp);
    PUT(HDRP(bp), PACK(size, 0));           // 设置头部
    PUT(FTRP(bp), PACK(size, 0));           // 设置尾部
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));   // 设置结尾块
    bp=coalesce(bp); 
    return bp;                  // 如果上一块是空闲块，合并上一块
}
static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(HDRP(PREV_BLKP(bp))); // 上一块是否分配
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp))); // 下一块是否分配
    // printf("--打印操作：进行 coalesce 操作，地址为 %p 的上一块分配状态：%s ，下一块分配状态：%s\n",bp,(prev_alloc)?"是":"否",(next_alloc)?"是":"否");
    size_t size = GET_SIZE(HDRP(bp));
    if(prev_alloc && !next_alloc)
    { // 下一块是空闲块
        size_t x = (GET_SIZE(HDRP(NEXT_BLKP(bp)))) + size;
        removeLinkNode(NEXT_BLKP(bp));
        PUT(HDRP(bp), PACK(x, 0));
        PUT(FTRP(bp), PACK(x, 0));
    }
    else if (!prev_alloc && next_alloc)
    { // 上一块是空闲块
        size_t x = (GET_SIZE(HDRP(PREV_BLKP(bp)))) + size;
        removeLinkNode(PREV_BLKP(bp));
        PUT(HDRP(PREV_BLKP(bp)), PACK(x, 0)); // 先设置上一块的头部
        PUT(FTRP(PREV_BLKP(bp)), PACK(x, 0)); // 设置当前块尾部
        bp = PREV_BLKP(bp);                   // 修改bp为上一块的位置
    }
    else if (!prev_alloc && !next_alloc)
    { // 上下两块均是空闲块
        size_t x = (GET_SIZE(HDRP(PREV_BLKP(bp)))) + size + (GET_SIZE(HDRP(NEXT_BLKP(bp))));
        removeLinkNode(PREV_BLKP(bp));
        removeLinkNode(NEXT_BLKP(bp));
        PUT(HDRP(PREV_BLKP(bp)), PACK(x, 0)); // 设置上一块头部
        PUT(FTRP(NEXT_BLKP(bp)), PACK(x, 0)); // 设置下一块尾部
        bp = PREV_BLKP(bp);                   // 修改bp为上一块的位置
    }
    insertLinkNode(bp);
    return bp;
}
static void* find_fit(size_t asize)
{
    for (void* bp = head; bp != 0; bp = GET_SUCCESSOR(bp))
    {
        if (GET_SIZE(HDRP(bp)) >= asize)
        {
            return bp;
        }
    }
    return NULL;
}

static void place(void *bp, size_t new_size)
{
    // printf("--打印操作：进行 place 操作，当前 bp 的地址为 %p，空闲块大小为 %d ，分割大小为 %d \n",heap_listp,GET_SIZE(HDRP(bp)),new_size);
    size_t dsize = GET_SIZE(HDRP(bp)) - new_size;
    removeLinkNode(bp);
    if (dsize < MINSIZE)
    { // 当前剩余碎片小于MINSIZE
        PUT(HDRP(bp), PACK(GET_SIZE(HDRP(bp)), 1));
        PUT(FTRP(bp), PACK(GET_SIZE(HDRP(bp)), 1));
    }
    else
    {
        PUT(HDRP(bp), PACK(new_size, 1)); // 分配当前new_size
        PUT(FTRP(bp), PACK(new_size, 1));
        PUT(HDRP(NEXT_BLKP(bp)), PACK(dsize, 0)); // 割下剩下的块作为空闲块
        PUT(FTRP(NEXT_BLKP(bp)), PACK(dsize, 0));   
        coalesce(NEXT_BLKP(bp));
    }
    // debugAllBlock();
}
static size_t getBlockSize(size_t size)
{ // 获取实际的块的大小
    size_t new_size;
    if (size <= DSIZE)
        new_size = DSIZE * 2; // 原因：当前首尾需要一个DSIZE，然后小于size需要对齐到dsize
    else
        new_size = DSIZE * (((size) + (DSIZE) + (DSIZE - 1)) / (DSIZE)); // payload+首尾开销，进行上取整
    return new_size;
}

int mm_init(void)
{
    
    if ((heap_listp = mem_sbrk(WSIZE << 2)) == (void *)-1)
    { // 当前初始化失败
        return -1;
    }
    // 初始化成功，当前堆占4个word（一个word==4Bytes）
    PUT(heap_listp, 0);
    PUT(heap_listp + WSIZE, PACK(DSIZE, 1));        // 设置序言头部大小
    PUT(heap_listp + (WSIZE *2), PACK(DSIZE, 1));   // 设置序言尾部大小
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1));      // 设置结尾
    heap_listp += DSIZE;
    head=NULL; // 初始化head指针 -> 指向下一个链表头
    // printf("--打印操作：开始初始化 当前 heap_listp 的地址为 %p \n",heap_listp);
    if (extend_heap(CHUNKSIZE / WSIZE) == NULL) // 初始化默认空闲块大小
        return -1;
    return 0;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */

void *mm_malloc(size_t size)
{
    
    // printf("--打印操作：进行 malloc 操作，分配 %d 字节的虚拟内存\n",size);
    size_t new_size;
    size_t extendsize;
    char *bp;
    if (!size)return NULL;
    new_size = getBlockSize(size);
    if ((bp = find_fit(new_size)) != NULL)
    {                        // 使用某种算法，找到一个满足要求的空闲块
        place(bp, new_size); // 进行分割
        return bp;
    }
    extendsize = MAX(new_size, CHUNKSIZE); // 若当前未找到，扩展heap，向上开辟
    if ((bp = extend_heap(extendsize / WSIZE)) == NULL)
    { // 扩展失败
        return NULL;
    }
    place(bp, new_size);
    // debugAllBlock();
    return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    if(ptr==NULL)return;
    // printf("--打印操作：进行 free 操作，释放 %p 位置的虚拟内存\n",ptr);
    size_t size = GET_SIZE(HDRP(ptr));
    PUT(HDRP(ptr), PACK(size, 0)); // 修改当前块首部的分配位
    PUT(FTRP(ptr), PACK(size, 0)); // 修改当前块尾部的分配位
    coalesce(ptr);                 // 合并
    // debugAllBlock();
    // printDoubleLinkList();
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t re_size)
{
    // printf("--打印操作：进行 realloc 操作，重新给 %p 位置的虚拟内存分配 %d 字节的虚拟内存\n",ptr,re_size);
    void *oldptr = ptr;
    if(ptr==NULL){ // 当此时ptr为NULL等价于mm_malloc(size)
        return mm_malloc(getBlockSize(re_size));
    }
    if(re_size==0){ // 当此时size为0，等价于调用mm_free(ptr)
        mm_free(ptr);
        return NULL;
    }
    // 正常realloc
    void *newptr;
    size_t copySize = GET_SIZE(HDRP(ptr)) - DSIZE; // payload长度
    newptr = mm_malloc(getBlockSize(re_size));
    if (newptr == NULL)return NULL;
    if (re_size < copySize)copySize = re_size;  // 当前容量减小
    memcpy(newptr, oldptr, copySize); // 拷贝数据至新的块
    mm_free(oldptr);                  // 释放旧的块
    // debugAllBlock();
    return newptr;                    // 返回新的块地址
}
static void debugAllBlock(){
    char *bp;
    printf("\n");
    printf("---------------开始debug-------------------\n");
    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp))
    {
        
        int alloc=GET_ALLOC(HDRP(bp)),size=GET_SIZE(HDRP(bp));
        printf("--当前%p位置的空闲块：\n",bp);
        printf("--大小为：\t%d\n",size);
        printf("--分配状态：\t%s\n",(alloc)?"是":"否");
        printf("\n");
    }
    printf("--结尾块地址：%p \n",bp);
    printf("\n");
    printf("---------------结束debug-------------------\n");
    printf("\n");
}
static void printDoubleLinkList(){
    char *bp=head;
    printf("\n");
    printf("---------------开始打印链表-------------------\n");
    printf("--打印操作：双链表\n");
    while(bp){
        printf("--%p\n",bp);
        bp=GET_SUCCESSOR(bp);
    }
    printf("---------------开始打印链表-------------------\n");
    printf("\n");
    return;
}
static void *next_fit(size_t new_size){// Perf index = 44 (util) + 40 (thru) = 84/100
    // void *bp=next_fit_bp;
    // while(1){
    //     if((GET_ALLOC(HDRP(bp))==0) && (new_size<= GET_SIZE(HDRP(bp)))){
    //         next_fit_bp=bp;
    //         return bp;
    //     }
    //     if(GET_SIZE(HDRP(bp))==0)bp=heap_listp;
    //     else bp=NEXT_BLKP(bp);
    //     if(bp==next_fit_bp)break;
    // }
    return NULL;
}
static void *best_fit(size_t new_size){// Perf index = 45 (util) + 22 (thru) = 67/100
    void *bp;
    void *best_fit_bp=NULL;
    for(bp=heap_listp;GET_SIZE(HDRP(bp))>0;bp=NEXT_BLKP(bp)){
        if(GET_ALLOC(HDRP(bp))==0 && ((new_size<=(GET_SIZE(HDRP(bp)))) &&(best_fit_bp==NULL || GET_SIZE(HDRP(best_fit_bp))>=GET_SIZE(HDRP(bp))))){
            best_fit_bp=bp;
        }
    }
    return best_fit_bp;
}