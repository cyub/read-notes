#include "unp.h"

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *servaddr,
            socklen_t servlen) {
  int n;
  char sendline[MAX_LINE];
  char recvline[MAX_LINE + 1]; // 注意此处要+1，以便放字符串终止符
  socklen_t len;
  struct sockaddr *reply_addr = NULL; // 用于验证服务端地址
  while (fgets(sendline, MAX_LINE, fp) != NULL) {
    Sendto(sockfd, sendline, strlen(sendline), 0, servaddr, servlen);
    len = servlen;
    n = Recvfrom(sockfd, recvline, MAX_LINE - 1, 0, reply_addr,
                 &len); // 不关心服务端地址信息，所以值设置为NULL

    if (len != servlen || memcmp(servaddr, reply_addr, len) !=
                              0) { // 校验收到的信息是不是真正的服务端发送过来的
      printf("reply from %s (ignored)\n", Sock_ntop(reply_addr, len));
      continue;
    }
    recvline[n] = 0; // 设置null结尾
    Fputs(recvline, stdout);
  }
}
