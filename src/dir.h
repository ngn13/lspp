#pragma once
#include <sys/stat.h>
#include <stdbool.h>
#include <limits.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "icons.h"
#include "options.h"
#include "util.h"
#include "log.h"

struct FileInfo {
  struct stat st;
  char* user_grp;
  char* name;
  int ug_len;
};

bool listdir(char*);
bool print_file(struct FileInfo);
