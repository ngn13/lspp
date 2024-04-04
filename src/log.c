#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void info(const char *msg, ...) {
  va_list args;
  va_start(args, msg);

  printf(BOLD BLUE " " RESET BOLD);
  vprintf(msg, args);
  printf(RESET "\n");

  va_end(args);
}

void error(const char *msg, ...) {
  va_list args;
  va_start(args, msg);

  printf(BOLD RED " " RESET BOLD);
  vprintf(msg, args);
  printf(RESET "\n");

  va_end(args);
}

void table() {
  printf("\n");
  printf(BOLD UNDERLINE "Permissions" RESET);
  printf("  ");
  printf(BOLD UNDERLINE "Last Modification       " RESET);
  printf("  ");
  printf(BOLD UNDERLINE "Owned By" RESET);
  printf("  ");
  printf(BOLD UNDERLINE "Filename\n" RESET);
}
