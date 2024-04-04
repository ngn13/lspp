#pragma once
#include <stdbool.h>
#include <sys/stat.h>

struct FileInfo {
  struct stat st;
  char *user_grp;
  char *name;
  int ug_len;
};

bool listdir(char *);
bool print_file(struct FileInfo);
