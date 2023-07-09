#include <stdio.h>
#include "./tiny/csapp.h"
/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *http_prefix="http://";

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
void receiveRespense(rio_t *rio,int clientfd) 
{
    char buf[MAXLINE];
    int n;
    // 参考https://github.com/Guo749/ProxyLab/ 的读取，可读二进制文件
    while((n = Rio_readnb(rio, buf, MAXLINE))!= 0){ // 读取server ===> proxy 发来的消息buf
        Rio_writen(clientfd, buf, n);  // 将buf消息转发至 proxy ===> client 
    }
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
    rio_t rio,server_rio;
    Rio_readinitb(&rio,fd);
    if(!Rio_readlineb(&rio,buf,MAXLINE))return;
    sscanf(buf,"%s %s %s",method,url,version);
    if(strcasecmp(method,"GET") && strcasecmp(method,"POST")){
        clienterror(fd,method,"501","Not Implemented","Proxy does not implement this method");
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
    receiveRespense(&server_rio,fd);
}
int main(int argc,char **argv)
{
    int listenfd,connfd;
    char hostname[MAXLINE],port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    if(argc!=2){
        fprintf(stderr,"usage: %s <port>\n",argv[0]);
        exit(1);
    }
    listenfd=Open_listenfd(argv[1]);
    while(1){
        clientlen=sizeof(clientaddr);
        connfd=Accept(listenfd,(SA *)&clientaddr,&clientlen);
        Getnameinfo((SA *)&clientaddr,clientlen,hostname,MAXLINE,port,MAXLINE,0);
        printf("Accepted connection from (%s,%s)\n",hostname,port);
        handleRequest(connfd);
        Close(connfd);
    }
    return 0;
}
