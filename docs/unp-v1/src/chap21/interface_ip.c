/**
 * 根据网络接口名称获取ip
 * https://stackoverflow.com/questions/2283494/get-ip-address-of-an-interface-on-linux
 */
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fd;
  struct ifreq ifr;

  if (argc < 2) {
    fprintf(stderr, "%s <interface_name>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  /* I want to get an IPv4 IP address */
  ifr.ifr_addr.sa_family = AF_INET;

  /* I want IP address attached to "eth0" */
  strncpy(ifr.ifr_name, argv[1], IFNAMSIZ - 1);

  if (ioctl(fd, SIOCGIFADDR, &ifr) < 0) {
    // fprintf(stderr, "%s ip get error:", perr)
    perror("ioctl()");
    exit(EXIT_FAILURE);
  }

  close(fd);

  /* display result */
  printf("%s: %s\n", argv[1],
         inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

  return 0;
}
