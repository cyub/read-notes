#include "unp.h"
#include <net/if.h>
#include <netinet/in.h>
#include <stdlib.h>

void Mcast_join(int sockfd, const struct sockaddr *grp, socklen_t grplen,
                const char *ifname, u_int ifindex) {
  if (mcast_join(sockfd, grp, grplen, ifname, ifindex) < 0)
    err_sys("mcast_join()");
}
int mcast_join(int sockfd, const struct sockaddr *grp, socklen_t grplen,
               const char *ifname, u_int ifindex) {
#undef MCAST_JOIN_GROUP // 似乎MCAST_JOIN_GROUP不起作用，这里面强制不适用它
#ifdef MCAST_JOIN_GROUP
  struct group_req req;
  if (ifindex > 0) { // 给定了接口索引
    req.gr_interface = ifindex;
  } else if (ifname != NULL) { // 给定了接口名称
    if ((req.gr_interface = if_nametoindex(ifname)) == 0) {
      errno = ENXIO; // name not found
      return -1;
    }
  } else
    req.gr_interface = 0; // 由内核选择接口

  if (grplen > sizeof(req.gr_group)) { // 防止传入值超过gr_group尺寸造成溢出
    errno = EINVAL;
    return -1;
  }
  memcpy(&req.gr_group, grp, grplen);
  return setsockopt(sockfd, family_to_level(grp->sa_family), MCAST_JOIN_GROUP,
                    &req, sizeof(req));
#endif
  switch (grp->sa_family) {
  case AF_INET: {
    struct ip_mreq mreq;
    struct ifreq ifreq;
    memcpy(&mreq.imr_multiaddr, &((const struct sockaddr_in *)grp)->sin_addr,
           sizeof(struct in_addr)); // 设置多播组地址
    if (ifindex > 0) {
      if (if_indextoname(ifindex, ifreq.ifr_name) == NULL) {
        errno = ENXIO;
        return (-1);
      }
      goto doioctl;
    } else if (ifname !=
               NULL) { // 根据接口名称查询到接口地址，然后设置成本地地址
      strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);
    doioctl:
      if (ioctl(sockfd, SIOCGIFADDR, &ifreq) <
          0) // 获取网络接口信息，ifreq: interface request
        return -1;
      memcpy(&mreq.imr_interface,
             &((struct sockaddr_in *)&ifreq.ifr_addr)->sin_addr,
             sizeof(struct in_addr));
    } else // 否则，由系统决定接口地址
      mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    return setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq,
                      sizeof(mreq));
  }
#ifdef IPV6
#ifndef IPV6_JOIN_GROUP
#define IPV6_JOIN_GROUP
#endif
  case AF_INET6: {
    print("@@@@@");
    struct ipv6_mreq mreq6;
    memcpy(&mreq6.ipv6mr_multiaddr,
           &((const struct sockaddr_in6 *)grp)->sin6_addr,
           sizeof(struct in6_addr));

    if (ifindex > 0) {
      mreq6.ipv6mr_interface = ifindex;
    } else if (ifname != NULL) {
      if ((mreq6.ipv6mr_interface = if_nametoindex(ifname)) == 0) {
        errno = ENXIO; /* i/f name not found */
        return (-1);
      }
    } else
      mreq6.ipv6mr_interface = 0;

    return (setsockopt(sockfd, IPPROTO_IPV6, IPV6_JOIN_GROUP, &mreq6,
                       sizeof(mreq6)));
  }
#endif
  default:
    errno = EAFNOSUPPORT;
    return -1;
  }
  return 0;
}

int mcast_set_loop(int sockfd, int onoff) {
  switch (sockfd_to_family(sockfd)) {
  case AF_INET: {
    u_char flag;
    flag = onoff;
    return setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &flag,
                      sizeof(flag));
  }
#ifdef IPV6
  case AF_INET6: {
    u_int flag;
    flag = onff;
    return (setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &flag,
                       sizeof(flag)));
  }
#endif
  default:
    errno = EAFNOSUPPORT;
    return -1;
  }
}

void Mcast_set_loop(int sockfd, int onoff) {
  if (mcast_set_loop(sockfd, onoff) < 0)
    err_sys("mcast_set_loop()");
}
