/**
 * 获取所有网卡名称与其索引值
 */
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  // 打印所有的网络卡索引
  struct if_nameindex *if_nidxs, *intf;
  if_nidxs = if_nameindex();
  if (if_nidxs != NULL) {
    for (intf = if_nidxs; intf->if_index != 0 || intf->if_name != NULL;
         intf++) {
      printf("%d: %s\n", intf->if_index, intf->if_name);
    }
    if_freenameindex(if_nidxs);
  }

  struct ifaddrs *ifaddr, *ifa;
  if (getifaddrs(&ifaddr) == -1) {
    perror("getifaddrs");
    return -1;
  }

  // 打印网络类型
  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL)
      continue;
    printf("%-8s %s(%d)\n", ifa->ifa_name,
           (ifa->ifa_addr->sa_family == AF_INET) ? "AF_INTET"
#ifdef AF_LINK // Macos特有
           : (ifa->ifa_addr->sa_family == AF_LINK) ? "AF_LINK"
#endif
           :
#ifdef AF_PACKET // Linux特有
           (ifa->ifa_addr->sa_family == AF_PACKET) ? "AF_PACKET"
           :
#endif
           (ifa->ifa_addr->sa_family == AF_INET6) ? "AF_INET6"
                                                  : "???",
           ifa->ifa_addr->sa_family);
  }
  freeifaddrs(ifaddr);
  return 0;
}
