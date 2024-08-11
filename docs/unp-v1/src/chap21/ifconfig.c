/**
 * 简易版本ifconfig命令
 * 列出所有网络接口，没有进行合并去重操作
 * 网络接口定义信息见文档：
 https://man7.org/linux/man-pages/man7/netdevice.7.html
 *
 * 几个常见版本:
 * linux系统内置版本：https://github.com/ecki/net-tools/blob/master/ifconfig.c#L118
 * 安卓版本：https://android.googlesource.com/platform/system/core.git/+/froyo/toolbox/ifconfig.c
 * bsd版本：https://github.com/openbsd/src/blob/master/sbin/ifconfig/ifconfig.c
 * macos版本：https://github.com/apple-oss-distributions/network_cmds/blob/main/ifconfig.tproj/ifconfig.c#L1885
 */

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

void print_interface_info() {
  struct ifaddrs *ifaddr, *ifa;
  int sock;

  // 创建一个socket用于获取接口信息
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  // 获取网络接口信息
  if (getifaddrs(&ifaddr) == -1) {
    perror("getifaddrs");
    close(sock);
    exit(EXIT_FAILURE);
  }

  // 遍历所有接口
  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL) {
      continue; // 跳过没有地址的接口
    }

    // 打印接口名称
    printf("%s: ", ifa->ifa_name);

    // 使用ioctl获取接口的标志
    struct ifreq ifr, mtuifr;
    unsigned int flags;
    strncpy(ifr.ifr_name, ifa->ifa_name, IFNAMSIZ - 1);
    strncpy(mtuifr.ifr_name, ifa->ifa_name, IFNAMSIZ - 1);
    if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0) {
      perror("ioctl");
      continue;
    }

#ifdef __APPLE__
    flags = ifa->ifa_flags; // macos获取flags字段不同，并且以16进制显示
    printf("flags=%x", flags);
#else
    flags = ifr.ifr_flags;
    printf("flags=%d", flags);
#endif

    putchar('<');
    // 检查是否启动
    if (flags && IFF_UP) {
      printf("UP");
    } else {
      printf("DOWN");
    }

    // 检查循环地址支持
    if (flags & IFF_LOOPBACK) {
      printf(",LOOPBACK");
    }

    // 检查广播支持
    if (flags & IFF_BROADCAST) {
      printf(",BROADCAST");
    }

    // 检查接口状态
    if (flags & IFF_RUNNING) {
      printf(",RUNNING");
    }

    // 检查多播支持
    if (flags & IFF_MULTICAST) {
      printf(",MULTICAST");
    }
    putchar('>');

    // 获取MTU值
    if (ioctl(sock, SIOCGIFMTU, &mtuifr) != -1) {
      printf(" mtu %d\n", mtuifr.ifr_mtu);
    }

    // 打印IP地址
    if (ifa->ifa_addr->sa_family == AF_INET) {
      char ip[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr, ip,
                sizeof(ip));
      printf("\t inet %s", ip);
      if (ioctl(sock, SIOCGIFNETMASK, &ifr) < 0) {
        perror("ioctl");
        continue;
      }

      // 打印网络掩码
      unsigned int addr;
      addr = ((struct sockaddr_in *)(&ifr.ifr_addr))->sin_addr.s_addr;
      printf(" netmask %d.%d.%d.%d", addr & 0xff, (addr >> 8) & 0xff,
             (addr >> 16) & 0xff, (addr >> 24) & 0xff); // IP地址是大端法存储
    } else if (ifa->ifa_addr->sa_family == AF_INET6) {
      char ip[INET6_ADDRSTRLEN];
      inet_ntop(AF_INET6, &((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr,
                ip, sizeof(ip));
      printf("\t inet6 %s", ip);
    }

    printf("\n\n");
  }

  // 释放资源
  freeifaddrs(ifaddr);
  close(sock);
}

int main() {
  print_interface_info();
  return 0;
}
