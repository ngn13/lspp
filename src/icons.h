#pragma once
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include "util.h"

struct IconMap {
  char* name;
  char* icon;
};

char* find_icon(char*, struct stat);
