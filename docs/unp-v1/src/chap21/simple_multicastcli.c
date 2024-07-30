#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAXBUF 256
#define PUERTO 5000
#define GRUPO "224.0.1.1"

// 源码来源：
// http://www.linuxfocus.org/English/January2001/article144.meta.shtml
int main(void) {
  int s, r;
  socklen_t n;
  struct sockaddr_in srv, cli;
  struct ip_mreq mreq;
  char buf[MAXBUF];

  // bzero(&srv, sizeof(srv));
  memset(&srv, '\0', sizeof(srv));
  srv.sin_family = AF_INET;
  srv.sin_port = htons(PUERTO);
  if (inet_aton(GRUPO, &srv.sin_addr) < 0) {
    perror("inet_aton");
    return 1;
  }

  if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    return 1;
  }

  if (bind(s, (struct sockaddr *)&srv, sizeof(srv)) < 0) {
    perror("bind");
    return 1;
  }

  if (inet_aton(GRUPO, &mreq.imr_multiaddr) < 0) {
    perror("inet_aton");
    return 1;
  }
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);

  if (setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
    perror("setsockopt");
    return 1;
  }

  n = sizeof(cli);
  while (1) {
    if ((r = recvfrom(s, buf, MAXBUF, 0, (struct sockaddr *)&cli, &n)) < 0) {
      perror("recvfrom");
    } else {
      buf[r] = 0;
      fprintf(stdout, "read from server %s: %s\n", inet_ntoa(cli.sin_addr),
              buf);
    }
  }
}
