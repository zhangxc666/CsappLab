#include "cachelab.h"
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
int  S,E,B,flag;
int miss,hit,drop,time;
char file[100];
typedef struct cacheUnit_{
    int tag;
    int isVaild;
    int timeStamp;
}cacheUnit;
cacheUnit **head;
FILE *pFile;
void printHelpInfo(){// 打印help 
    printf("Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("-h         Print this help message.\n");
    printf("-v         Optional verbose flag.\n");
    printf("-s <num>   Number of set index bits.\n");
    printf("-E <num>   Number of lines per set.\n");
    printf("-b <num>   Number of block offset bits.\n");
    printf("-t <file>  Trace file.\n\n\n");
    printf("Examples:\n");
    printf("linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}
void getParams(int argc,char **argv){// 获取参数
    int opt;
    while ((opt=getopt(argc,argv,"hvs:E:b:t:"))!=-1)
    {
        switch (opt)
        {
        case 'h':
            printHelpInfo();break;
        case 'v':
            flag=1;break;
        case 's':
            S=atoi(optarg);break;
        case 'E':
            E=atoi(optarg);break;
        case 'b':
            B=atoi(optarg);break;
        case 't':
            strcpy(file,optarg);break;
        default:
            printHelpInfo();
            exit(-1);
            break;
        }
    }
}
void mallocCache(){ // 分配缓存快
    head=(cacheUnit **)malloc(sizeof (cacheUnit*)*(1<<S));
    for(int i=0;i<(1<<S);i++){
        head[i]=(cacheUnit *)malloc(sizeof (cacheUnit)*E);
        for(int j=0;j<E;j++){
            head[i][j].isVaild=0;
        }
    }
}
void freeCache(){ // 释放缓存块
    for(int i=0;i<(1<<S);i++)free(head[i]);
    free(head);
}
int isHitCache(unsigned s,unsigned tag){ // 当前cache是否命中
    for(int i=0;i<E;i++){
        if(head[s][i].isVaild==1 && head[s][i].tag==tag){
            hit++;
            if(flag)printf("hit ");
            return i;
        }
    }
    miss++;
    if(flag)printf("miss ");
    return -1;
}
int getLRUCache(unsigned s){ // 查当前是否有空块，否则返回lru替换的索引
    int st=-1;
    for(int i=0;i<E;i++){
        if(head[s][i].isVaild==0)return i;
        if(st==-1 || (head[s][i].timeStamp<head[s][st].timeStamp))st=i;
    }
    drop++;
    if(flag)printf("eviction ");
    return st;
}
void updateCacheUnit(unsigned s,unsigned i,unsigned tag){  // 更新cache单元
    head[s][i].tag=tag;
    head[s][i].isVaild=1;
    head[s][i].timeStamp=time;
}
void getTag(unsigned *tag,unsigned *s,unsigned addr){ // 根据地址获取tag
    (*s)=(addr)&((1<<S)-1);
    (*tag)=((addr-(*s))>>S);
}
void accessCache(unsigned addr){ // 根据地址访问cache
    unsigned tag,s;
    getTag(&tag,&s,addr);
    int index=isHitCache(s,tag);
    if(index!=-1){
        updateCacheUnit(s,index,tag);
        return;
    }
    index=getLRUCache(s);
    updateCacheUnit(s,index,tag);
}
void processInstructions(){ // 处理指令
    pFile=fopen(file,"r");
    char indentifier;
    unsigned address;
    int size;
    int backFlag=flag;
    while(fscanf(pFile," %c %x,%d",&indentifier,&address,&size)>0){
        unsigned backAddr=address;
        address>>=B;
        size=(size+(1<<B)-1)/(1<<B); // 考虑size>一个block的大小
        if(indentifier!='L' && indentifier!='M' && indentifier!='S')continue;
        if(backFlag)printf("%c %x %d ",indentifier,backAddr,size);
        flag=backFlag;
        while(size--){
            if(indentifier=='M'){
                accessCache(address);
                accessCache(address);
            }else if(indentifier=='L'){
                accessCache(address);
            }else if(indentifier=='S'){
                accessCache(address);
            }
            address+=1;
            flag=0;
        }
        if(backFlag)printf("\n");
        time++;
    }
}
int main(int argc,char **argv)
{
    getParams(argc,argv);
    mallocCache();
    processInstructions();
    freeCache();
    printSummary(hit,miss,drop);
    // printf("-----------------------------------\n");
    return 0;
}
