#include "unp.h"

void dg_echo(int sockfd, struct sockaddr *cliaddr, socklen_t len);

int main(int argc, char **argv) {
  int sockfd;
  struct sockaddr_in servaddr, cliaddr;
  sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
  memset(&servaddr, '\0', sizeof(struct sockaddr_in));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);
  Bind(sockfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr_in));
  dg_echo(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
}

void dg_echo(int sockfd, struct sockaddr *cliaddr, socklen_t clilen) {
  int n;
  socklen_t len;
  char mesg[MAX_LINE + 1];

  for (;;) {
    len = clilen;
    n = Recvfrom(sockfd, mesg, MAX_LINE, 0, cliaddr, &len);
    mesg[n] = 0;
    printf("recvfrom: %s\n", mesg);
    Sendto(sockfd, mesg, n, 0, cliaddr, len);
  }
}
