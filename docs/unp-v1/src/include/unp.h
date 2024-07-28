#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#ifdef HAVE_SOCKADDR_DL_STRUCT
#include <net/if_dl.h>
#endif

#define MAX_LINE 1024

#ifndef SERV_PORT
#define SERV_PORT 6767
#endif

// 错误处理相关
void err_sys(const char *fmt, ...);
void err_quit(const char *fmt, ...);

// Socket相关
void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int Socket(int domain, int type, int protocol);
void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
ssize_t Recvfrom(int sockfd, void *buff, size_t len, int flags,
                 struct sockaddr *addr, socklen_t *addr_len);
ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *addr, socklen_t addr_len);

// 网络选项相关
void Inet_pton(int af, const char *src, void *dst);
char *Sock_ntop(const struct sockaddr *sa, socklen_t salen);
void Setsockopt(int sockfd, int level, int optname, const void *optval,
                socklen_t optlen);

// IO相关
ssize_t Read(int fd, void *buf, size_t count);
void Write(int fd, const void *buf, size_t count);

char *Fgets(char *s, int size, FILE *stream);
void Fputs(const char *s, FILE *stream);

// 内存相关
void *Malloc(size_t size);

// 信号相关
// typedef void (*sighandler_t)(int);
typedef void Sigfun(int);
Sigfun *Signal(int signum, Sigfun handler);
