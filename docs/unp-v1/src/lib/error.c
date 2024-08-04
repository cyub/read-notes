
#include "unp.h"

void do_err_sys(int errorflag, const char *fmt, va_list ap);

void err_sys(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  do_err_sys(1, fmt, ap);
  va_end(ap);
  exit(EXIT_FAILURE);
}

void err_quit(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  do_err_sys(0, fmt, ap);
  va_end(ap);
  exit(EXIT_FAILURE);
}

void err_msg(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  do_err_sys(0, fmt, ap);
  va_end(ap);
}

void do_err_sys(int errorflag, const char *fmt, va_list ap) {
  int n, errno_saved;
  char buf[MAX_LINE + 1];
  errno_saved = errno;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"
  n = vsnprintf(buf, MAX_LINE, fmt, ap);
#pragma clang diagnostic pop
#else
  n = vsnprintf(buf, MAX_LINE, fmt, ap);
#endif

  if (errorflag) {
    snprintf(buf + n, MAX_LINE - n, ": %s\n", strerror(errno_saved));
  } else
    strcat(buf, "\n");
  fflush(stdout); /* in case stdout and stderr are the same */
  fputs(buf, stderr);
  fflush(stderr);
}
