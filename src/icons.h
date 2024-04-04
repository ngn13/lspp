#pragma once
#include <sys/stat.h>

struct IconMap {
  char *name;
  char *icon;
};

char *find_icon(char *, struct stat);
