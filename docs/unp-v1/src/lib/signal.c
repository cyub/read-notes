#include "unp.h"
#include <signal.h>

Sigfun *Signal(int signo, Sigfun *func) {
  Sigfun *sigfunc;
  if ((sigfunc = signal(signo, func)) == SIG_ERR)
    err_sys("signal()");
  return sigfunc;
}
