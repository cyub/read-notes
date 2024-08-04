/*
    基于多播实现的echo客户端
    @see https://gist.github.com/hostilefork/f7cae3dc33e7416f2dd25a402857b6c6
    Note that what this program does should be equivalent to NETCAT:
     echo "Hello World" | nc -u 239.255.255.250 1900
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "%s <ip> <port>\n", argv[0]);
    return 1;
  }

  char *group = argv[1];    // e.g. 239.255.255.250 for SSDP
  int port = atoi(argv[2]); // 0 if error, which is an invalid port

  // !!! If test requires, make these configurable via args
  const int delay_secs = 1;
  const char *message = "Hello, World!";

  // create what looks like an ordinary UDP socket
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    perror("socket");
    return 1;
  }

  // set up destination address
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(group);
  addr.sin_port = htons(port);

  // now just sendto() our destination!
  while (1) {
    int nbytes = sendto(fd, message, strlen(message), 0,
                        (struct sockaddr *)&addr, sizeof(addr));
    if (nbytes < 0) {
      perror("sendto");
      return 1;
    }
    sleep(delay_secs); // Unix sleep is seconds
  }

  return 0;
}
