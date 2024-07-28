#include "unp.h"
#include <string.h>

static void recvfrom_alarm(int);

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *servaddr,
            socklen_t servlen) {
  int n;
  const int on = 1;
  char sendline[MAX_LINE], recvline[MAX_LINE + 1];
  socklen_t len;
  struct sockaddr *reply_addr;
  reply_addr = Malloc(servlen);
  Setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
  Signal(SIGALRM, recvfrom_alarm);
  while (Fgets(sendline, MAX_LINE, fp) != NULL) {
    Sendto(sockfd, sendline, strlen(sendline), 0, servaddr, servlen);
    alarm(5);
    for (;;) {
      len = servlen;
      n = Recvfrom(sockfd, recvline, MAX_LINE, 0, reply_addr, &len);
      if (n < 0) {
        printf("error: %s", strerror(errno));
        if (errno == EINTR) {
          printf("waited long enough for replies\n");
          break;
        } else
          err_sys("recvfrom()");
      } else {
        recvline[n] = 0;
        printf("from %s: %s", Sock_ntop(reply_addr, len), recvline);
        // break;
      }
    }
  }

  free(reply_addr);
}

static void recvfrom_alarm(int signo) {
  printf("recvfrom_alarm\n");
  return; // just interrupt the recvfrom
}
