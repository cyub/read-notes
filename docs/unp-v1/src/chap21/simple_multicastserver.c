#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAXBUF 256
#define PUERTO 5000
#define GRUPO "224.0.1.1"

// 源码来源：http://www.linuxfocus.org/English/January2001/article144.meta.shtml
int main(void) {
  int s;
  struct sockaddr_in srv;
  char buf[MAXBUF];

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

  while (fgets(buf, MAXBUF, stdin)) {
    if (sendto(s, buf, strlen(buf), 0, (struct sockaddr *)&srv, sizeof(srv)) <
        0) {
      perror("recvfrom");
    } else {
      fprintf(stdout, "read from client %s: %s\n", GRUPO, buf);
    }
  }
}
