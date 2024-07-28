#include "unp.h"

ssize_t Read(int fd, void *buf, size_t count) {
  ssize_t n;
  if ((n = read(fd, buf, count)) == -1)
    err_sys("read()");
  return n;
}

void Write(int fd, const void *buf, size_t count) {
  if (write(fd, buf, count) != count)
    err_sys("write()");
}

void *Malloc(size_t size) {
  void *ptr;
  if ((ptr = malloc(size)) == NULL)
    err_sys("malloc()");
  return ptr;
}
