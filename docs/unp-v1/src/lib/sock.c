#include "unp.h"
#include <netinet/in.h>
#include <sys/socket.h>

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
  if (connect(sockfd, addr, addrlen) < 0)
    err_sys("connect()");
}

int Socket(int domain, int type, int protocol) {
  int fd;
  if ((fd = socket(domain, type, protocol)) < 0) {
    err_sys("socket()");
  }
  return fd;
}

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
  if (bind(sockfd, addr, addrlen) < 0) {
    err_sys("bind()");
  }
}

ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *addr, socklen_t *addr_len) {
  ssize_t n;
  if ((n = recvfrom(sockfd, buf, len, flags, addr, addr_len)) < 0)
    err_sys("recvfrom()");

  return n;
}

ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *addr, socklen_t addr_len) {
  ssize_t n;
  if ((n = sendto(sockfd, buf, len, flags, addr, addr_len)) < 0)
    err_sys("sendto()");
  return n;
}

// Inet_pton
// p: presentation 中文含义为表示，这里指的是转换
// to: 目的
// n: network中的n，表示网络地址
// af : address family 地址族
void Inet_pton(int af, const char *src, void *dst) {
  int n;
  if ((n = inet_pton(af, src, dst)) < 0) {
    err_sys("inet_pton()");
  } else if (n == 0)
    err_quit("inet_pton error: %s", src);
}

char *sock_ntop(const struct sockaddr *sa, socklen_t salen);
char *Sock_ntop(const struct sockaddr *sa, socklen_t salen) {
  char *ptr;
  if ((ptr = sock_ntop(sa, salen)) == NULL)
    err_sys("sock_ntop()");
  return ptr;
}

/* include sock_ntop */
char *sock_ntop(const struct sockaddr *sa, socklen_t salen) {
  char portstr[8];
  static char str[128]; /* Unix domain is largest */

  switch (sa->sa_family) {
  case AF_INET: {
    struct sockaddr_in *sin = (struct sockaddr_in *)sa;

    if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
      return (NULL);
    if (ntohs(sin->sin_port) != 0) {
      snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
      strcat(str, portstr);
    }
    return (str);
  }
  /* end sock_ntop */

#ifdef IPV6
  case AF_INET6: {
    struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)sa;

    str[0] = '[';
    if (inet_ntop(AF_INET6, &sin6->sin6_addr, str + 1, sizeof(str) - 1) == NULL)
      return (NULL);
    if (ntohs(sin6->sin6_port) != 0) {
      snprintf(portstr, sizeof(portstr), "]:%d", ntohs(sin6->sin6_port));
      strcat(str, portstr);
      return (str);
    }
    return (str + 1);
  }
#endif

#ifdef AF_UNIX
  case AF_UNIX: {
    struct sockaddr_un *unp = (struct sockaddr_un *)sa;

    /* OK to have no pathname bound to the socket: happens on
       every connect() unless client calls bind() first. */
    if (unp->sun_path[0] == '\0')
      strcpy(str, "(no pathname bound)");
    else
      snprintf(str, sizeof(str), "%s", unp->sun_path);
    return (str);
  }
#endif

#ifdef HAVE_SOCKADDR_DL_STRUCT
  case AF_LINK: {
    struct sockaddr_dl *sdl = (struct sockaddr_dl *)sa;

    if (sdl->sdl_nlen > 0)
      snprintf(str, sizeof(str), "%*s (index %d)", sdl->sdl_nlen,
               &sdl->sdl_data[0], sdl->sdl_index);
    else
      snprintf(str, sizeof(str), "AF_LINK, index=%d", sdl->sdl_index);
    return (str);
  }
#endif
  default:
    snprintf(str, sizeof(str), "sock_ntop: unknown AF_xxx: %d, len %d",
             sa->sa_family, salen);
    return (str);
  }
  return (NULL);
}

void Setsockopt(int sockfd, int level, int optname, const void *optval,
                socklen_t optlen) {
  if (setsockopt(sockfd, level, optname, optval, optlen) < 0)
    err_sys("setsockopt()");
}

int family_to_level(int family) {
  switch (family) {
  case AF_INET:
    return IPPROTO_IP;
#ifdef IPV6
  case AF_INET6:
    return IPPROTO_IPV6;
#endif
  default:
    return -1;
  }
}

int sockfd_to_family(int sockfd) {
  struct sockaddr_storage ss;
  socklen_t len;
  len = sizeof(ss);
  if (getsockname(sockfd, (struct sockaddr *)&ss, &len) < 0)
    return -1;
  return ss.ss_family;
}

int Sockfd_to_family(int sockfd) {
  int rc;
  if (((rc = sockfd_to_family(sockfd))) < 0)
    err_sys("sockfd_to_family()");

  return rc;
}

void sock_set_wild(struct sockaddr *sa, socklen_t salen) {
  const void *wildptr;
  switch (sa->sa_family) {
  case AF_INET: {
    static struct in_addr in4addr_any;
    in4addr_any.s_addr = htonl(INADDR_ANY);
    wildptr = &in4addr_any;
    break;
  }
#ifdef IPV6
  case AF_INET6: {
    wildptr = &in6addr_any;
    break;
  }
#endif
  default:
    return;
  }

  sock_set_addr(sa, salen, wildptr);
}

void sock_set_addr(struct sockaddr *sa, socklen_t salen, const void *addr) {
  switch (sa->sa_family) {
  case AF_INET: {
    struct sockaddr_in *sin = (struct sockaddr_in *)sa;
    memcpy(&sin->sin_addr, addr, sizeof(struct in_addr));
    return;
  }
#ifdef IPV6
  case AF_INET6: {
    struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)sa;
    memcpy(&sin6->sin6_addr, addr, sizeof(struct in6_addr));
  }
#endif
  }
  return;
}
