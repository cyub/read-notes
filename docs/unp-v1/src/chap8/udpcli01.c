#include "unp.h"

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *servaddr,
            socklen_t servlen);

int main(int argc, char **argv) {
  int sockfd;
  struct sockaddr_in servaddr;
  if (argc != 2) {
    err_quit("usage: udpcli <IPaddress>");
  }
  memset(&servaddr, '\0', sizeof(struct sockaddr_in));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);
  Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
  sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

  dg_cli(stdin, sockfd, (struct sockaddr *)&servaddr,
         sizeof(struct sockaddr_in));
  return 0;
}
