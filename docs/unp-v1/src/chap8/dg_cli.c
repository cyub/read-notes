#include "unp.h"

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *servaddr,
            socklen_t servlen) {
  int n;
  char sendline[MAX_LINE];
  char recvline[MAX_LINE + 1]; // 注意此处要+1，以便放字符串终止符
  while (Fgets(sendline, MAX_LINE, fp) != NULL) {
    Sendto(sockfd, sendline, strlen(sendline), 0, servaddr, servlen);
    n = Recvfrom(sockfd, recvline, MAX_LINE - 1, 0, NULL,
                 NULL); // 不关心服务端地址信息，所以值设置为NULL
    recvline[n] = 0;    // 设置null结尾
    Fputs(recvline, stdout);
  }
}
