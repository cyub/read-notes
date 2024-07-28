#include "unp.h"
#include <sys/socket.h>

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *servaddr,
            socklen_t servlen) {
  int n;
  char sendline[MAX_LINE], recvline[MAX_LINE + 1];
  Connect(sockfd, servaddr, servlen);
  while (Fgets(sendline, MAX_LINE, stdin) != NULL) {
    Write(sockfd, sendline, strlen(sendline));
    // 或者 Sendto(sockfd, sendline, strlen(sendline), 0, NULL, 0);
    n = Read(sockfd, recvline, MAX_LINE);
    recvline[n] = 0;
    Fputs(recvline, stdout);
  }
}
