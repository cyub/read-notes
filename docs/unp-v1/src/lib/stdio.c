#include "unp.h"

char *Fgets(char *s, int size, FILE *stream) {
  char *rs;
  if ((rs = fgets(s, size, stream)) == NULL && ferror(stream))
    err_sys("fgets()");
  return rs;
};

void Fputs(const char *s, FILE *stream) {
  if (fputs(s, stream) == EOF)
    err_sys("fputs()");
}
