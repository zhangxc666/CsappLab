/* $begin tinymain */
/*
 * tiny.c - A simple, iterative HTTP/1.0 Web server that uses the 
 *     GET method to serve static and dynamic content.
 *
 * Updated 11/2019 droh 
 *   - Fixed sprintf() aliasing issue in serve_static(), and clienterror().
 */
#include "csapp.h"

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);

int main(int argc, char **argv) 
{
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    listenfd = Open_listenfd(argv[1]); // ����һ������socket
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); // ��ʼ�ȴ�client�ķ�������
        Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0); // ��ȡclient��host��port
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        doit(connfd);                                             // ִ������
        Close(connfd);                                            //line:netp:tiny:close
    }
}
/* $end tinymain */

/*
 * doit - handle one HTTP request/response transaction
 */
/* $begin doit */
void doit(int fd) 
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;

    /* Read request line and headers */
    Rio_readinitb(&rio, fd);
    if (!Rio_readlineb(&rio, buf, MAXLINE))  //line:netp:doit:readrequest
        return;
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version);       // ȡ��method��url��http�汾
    if (strcasecmp(method, "GET")) {                     // Ŀǰ��֧��get����
        clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return;
    }                                                    
    read_requesthdrs(&rio);                              // ������ͷ

    /* Parse URI from GET request */
    is_static = parse_uri(uri, filename, cgiargs);       
    if (stat(filename, &sbuf) < 0) {                     // �ļ�δ�ҵ�
	    clienterror(fd, filename, "404", "Not found","Tiny couldn't find this file");
	    return;
    }                                                    //line:netp:doit:endnotfound

    if (is_static) { /* Serve static content */          
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) { // �����ǰ�û��޶�ȡȨ��
            clienterror(fd, filename, "403", "Forbidden","Tiny couldn't read the file");
            return;
	    }
	    serve_static(fd, filename, sbuf.st_size);        // ����̬����
    }
    else { /* Serve dynamic content */
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) { // �����ǰ�û��޶�ȡȨ��
            clienterror(fd, filename, "403", "Forbidden","Tiny couldn't run the CGI program");
            return;
        }
        serve_dynamic(fd, filename, cgiargs);            // ����̬����
    }
}
/* $end doit */

/*
 * read_requesthdrs - read HTTP request headers
 */
/* $begin read_requesthdrs */
void read_requesthdrs(rio_t *rp) 
{
    char buf[MAXLINE];

    Rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
    while(strcmp(buf, "\r\n")) {          // ��ʱ����������ͷ
        Rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
    }
    return;
}
/* $end read_requesthdrs */

/*
 * parse_uri - parse URI into filename and CGI args
 *             return 0 if dynamic content, 1 if static
 */
/* $begin parse_uri */
int parse_uri(char *uri, char *filename, char *cgiargs)  // ����url
{
    char *ptr;

    if (!strstr(uri, "cgi-bin")) {                       // ������̬��Դ
        strcpy(cgiargs, "");                             // �����̬cgiargs
        strcpy(filename, ".");                           
        strcat(filename, uri);                           // ת�����·��
        if (uri[strlen(uri)-1] == '/')                   // ����� '/' ��β������Ĭ�ϵ���ҳ
            strcat(filename, "home.html");               
        return 1;
    }
    else {  /* Dynamic content */                        // ������̬��Դ
        ptr = index(uri, '?');                           
        if (ptr) {                                       // ����в���
            strcpy(cgiargs, ptr+1);
            *ptr = '\0';
	    }
	    else strcpy(cgiargs, "");                        // ���û�в���
        strcpy(filename, ".");                           // ��ʼת��
        strcat(filename, uri);                           
	return 0;
    }
}
/* $end parse_uri */

/*
 * serve_static - copy a file back to the client 
 */
/* $begin serve_static */
void serve_static(int fd, char *filename, int filesize) // ����̬��Դ
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];

    /* Send response headers to client */
    get_filetype(filename, filetype);    // ��ȡ�ļ����ͣ��������Ӧͷ
    sprintf(buf, "HTTP/1.0 200 OK\r\n"); 
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n", filesize);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: %s\r\n\r\n", filetype); // ע�⣺��Ҫ���н�����Ӧͷ
    Rio_writen(fd, buf, strlen(buf));    //line:netp:servestatic:endserve

    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0); // ���ļ�
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0); // ���ļ�ӳ���������ڴ�
    Close(srcfd);                       //  �ر��ļ�
    Rio_writen(fd, srcp, filesize);     //line:netp:servestatic:write
    Munmap(srcp, filesize);             //  �ͷŵ�ӳ��������ڴ�
}

/*
 * get_filetype - derive file type from file name
 */
void get_filetype(char *filename, char *filetype) 
{
    if (strstr(filename, ".html"))
	strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))
	strcpy(filetype, "image/gif");
    else if (strstr(filename, ".png"))
	strcpy(filetype, "image/png");
    else if (strstr(filename, ".jpg"))
	strcpy(filetype, "image/jpeg");
    else
	strcpy(filetype, "text/plain");
}  
/* $end serve_static */

/*
 * serve_dynamic - run a CGI program on behalf of the client
 */
/* $begin serve_dynamic */
void serve_dynamic(int fd, char *filename, char *cgiargs) 
{
    char buf[MAXLINE], *emptylist[] = { NULL };

    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n"); 
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));
  
    if (Fork() == 0) { /* Child */ //line:netp:servedynamic:fork
        /* Real server would set all CGI vars here */
        setenv("QUERY_STRING", cgiargs, 1); // �������⻷���������ڸ�exe����
        Dup2(fd, STDOUT_FILENO);            // ��stdout�ض�����fd���ӽ��̿��Է������ݸ�fd
        Execve(filename, emptylist, environ); // ִ�ж������ļ�
    }
    Wait(NULL); /* Parent waits for and reaps child */ //line:netp:servedynamic:wait
}
/* $end serve_dynamic */

/*
 * clienterror - returns an error message to the client
 */
/* $begin clienterror */
void clienterror(int fd, char *cause, char *errnum,char *shortmsg, char *longmsg) // ����һ�������html��client
{
    char buf[MAXLINE];

    /* Print the HTTP response headers */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));

    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Tiny Error</title>");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Tiny Web server</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
}
/* $end clienterror */
