#include "./tiny/csapp.h"
#include "./sbuf.h"
#include "./cache.h"
/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1048576
#define MAX_OBJECT_SIZE 102400
#define MAX_THREAD_NUMBER 512
/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *http_prefix="http://";
static sbuf_t sbuf;
static cache Cache;
void debug(char *s){
    printf("%s\n",s);
}
void getPort(char *host,char *port){
    int n=strlen(host),index=-1;
    char buf[MAXLINE];
    for(int i=0;i<n;i++){
        if(host[i]==':'){
            index=i;
            break;
        }
        buf[i]=host[i];
    }
    buf[index]='\0';
    if(index==-1){
        *port="80";
    }else{
        strcpy(host,buf);
        strcpy(port,host+index+1);
    }
}
int parseURL(char *url,char *host,char *filename){
    char *ptr,temp[MAXLINE];
    int n,index;
    if(strlen(url)<7)return 1;
    for(int i=0;i<7;i++){
        if(url[i]!=http_prefix[i])return 1;
    }    
    strcpy(temp,url+7);
    n=strlen(temp);
    for(int i=0;i<n;i++){
        index=i;
        if(temp[i]=='/'){
            break;
        }
    }
    strncpy(host,temp,index);
    strcpy(filename,temp+index);
    // printf("temp: %s\nhost: %s\nfilename: %s\n",temp,host,filename);
    return 0;
}   
void sendRequestHeader(char *method,char *host,char *uri,int fd){
    char buf[MAXLINE];
    sprintf(buf,"%s %s %s\r\n",method,uri,"http/1.0");
    Rio_writen(fd,buf,strlen(buf));
    sprintf(buf,"Host:%s\r\n",host);
    Rio_writen(fd,buf,strlen(buf));
    sprintf(buf,"Connection: close\r\n");
    Rio_writen(fd,buf,strlen(buf));
    sprintf(buf,"Proxy-Connection: close\r\n\r\n");
    Rio_writen(fd,buf,strlen(buf));
}
void receiveRespense(rio_t *rio,int clientfd,char *url) 
{
    char buf[MAXLINE];
    int n,flag=0;
    cacheUnit *p=(cacheUnit *)Malloc(sizeof(cacheUnit));
    initNode(p,MAX_OBJECT_SIZE,url);

    // 参考https://github.com/Guo749/ProxyLab/ 的读取，可读二进制文件
    while((n = Rio_readnb(rio, buf, MAXLINE))!= 0){ // 读取server ===> proxy 发来的消息buf
        if(flag==1 || (n+p->size)>MAX_OBJECT_SIZE){   // 查找当前的文件是否已超过max_object_size
            flag=1;
        }else{
            strcpy(p->write,buf);                     // 未超过存入缓存中
            p->size+=n;
            p->write+=n;
        }
        Rio_writen(clientfd, buf, n);                // 将buf消息转发至 proxy ===> client
    }
    if(flag){//
        Free(p->value);
        Free(p);
        return;
    }
    if((Cache.size+p->size)>MAX_CACHE_SIZE){          // 如果超过最大的缓存数
        while((Cache.size+p->size)<MAX_CACHE_SIZE){   // 一直删除
            cacheUnit *temp=Cache.head;
            removeNode(Cache.head,&Cache);
            Free(temp->value);
            Free(temp);
        }
    } 
    insertNode(p,&Cache);                              // 没有超过最大的缓存数量
}
void clienterror(int fd, char *cause, char *errnum,char *shortmsg, char *longmsg) // 返回一个错误的html给client
{
    char buf[MAXLINE];
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<html><title>Tiny Error</title>");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Proxy Web server</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
}
void handleRequest(int fd){
    char buf[MAXLINE],method[MAXLINE],url[MAXLINE],version[MAXLINE],host[MAXLINE],uri[MAXLINE],port[MAXLINE];
    cacheUnit *p;
    rio_t rio,server_rio;
    Rio_readinitb(&rio,fd);
    if(!Rio_readlineb(&rio,buf,MAXLINE))return;
    sscanf(buf,"%s %s %s",method,url,version);
    printf("Url: %s\n",url);
    if(strcasecmp(method,"GET") && strcasecmp(method,"POST")){
        clienterror(fd,method,"501","Not Implemented","Proxy does not implement this method");
        return;
    }
    if(p=findNode(&Cache,url)){ // 如果在缓存中找到host，返回对应的缓存值
        Rio_writen(fd,p->value,p->size);
        removeNode(p,&Cache);
        insertNode(p,&Cache); 
        return;
    }
    if(parseURL(url,host,uri)){
        clienterror(fd,method,"501","URL is wrong","This URL is wrong!");
        return;
    }
    getPort(host,port);
    int serverfd=Open_clientfd(host,port); // 发送到服务器的
    Rio_readinitb(&server_rio,serverfd);
    sendRequestHeader(method,host,uri,serverfd);
    receiveRespense(&server_rio,fd,url);
}
void *thread(void *param){
    Pthread_detach(Pthread_self());
    while(1){
        int connfd=sbuf_remove(&sbuf);
        handleRequest(connfd);
        Close(connfd);
    }
    
}

int main(int argc,char **argv)
{
    int listenfd,connfd;
    char hostname[MAXLINE],port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;

    if(argc!=2){
        fprintf(stderr,"usage: %s <port>\n",argv[0]);
        exit(1);
    }
    listenfd=Open_listenfd(argv[1]);
    initCache(&Cache);
    sbuf_init(&sbuf,MAX_THREAD_NUMBER);
    for(int i=0;i<MAX_THREAD_NUMBER;i++){
        Pthread_create(&tid,NULL,thread,NULL);
    }

    while(1){
        clientlen=sizeof(clientaddr);
        connfd=Accept(listenfd,(SA *)&clientaddr,&clientlen);
        Getnameinfo((SA *)&clientaddr,clientlen,hostname,MAXLINE,port,MAXLINE,0);
        printf("Accepted connection from (%s,%s)\n",hostname,port);
        sbuf_insert(&sbuf,connfd);
    }
    return 0;
}
