#include "unp.h"
#include <setjmp.h>
#include <stdlib.h>
#include <sys/socket.h>

static void recvfrom_alarm(int);
static sigjmp_buf jmpbuf;

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *servaddr,
            socklen_t servlen) {
  int n;
  // const int on = 1;
  char sendline[MAX_LINE], recvline[MAX_LINE + 1];
  socklen_t len;
  struct sockaddr *reply_addr;
  reply_addr = Malloc(servlen);
  // Setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
  printf("do test");
  Signal(SIGALRM, recvfrom_alarm);
  while (Fgets(sendline, MAX_LINE, fp) != NULL) {
    Sendto(sockfd, sendline, strlen(sendline), 0, servaddr, servlen);
    alarm(5);
    for (;;) {
      if (sigsetjmp(jmpbuf, 1) != 0)
        break;
      len = servlen;
      n = Recvfrom(sockfd, recvline, MAX_LINE, 0, reply_addr, &len);
      recvline[n] = 0;
      printf("from %s: %s", Sock_ntop(reply_addr, len), recvline);
    }
  }
  free(reply_addr);
}

static void recvfrom_alarm(int signo) {
  printf("recvfrom_alarm\n");
  siglongjmp(jmpbuf, 1);
}
