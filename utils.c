#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>

/***********************************************************************************************************************
 * Exit with error messages
 **********************************************************************************************************************/
void ExitWithError(char *fmt,...)
{
  va_list va;

  fprintf(stderr, "Error: ");
  va_start(va, fmt);
  vfprintf(stderr, fmt, va);
  va_end(va);
  fprintf(stderr, "\n");
  if(errno !=0 ) {
    perror("Details");
  }

  exit(EXIT_FAILURE);
}
