#pragma once
#include <stdbool.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define ends(str, sub) strlen(str) > 4 && !strcmp(str + strlen(str) - strlen(sub), sub) 
#define eq(s1, s2) strcmp(s1, s2)==0 
#define neq(s1, s2) strcmp(s1, s2)!=0 

bool exists(char*);
bool is_dir(char*);
char* get_user(uid_t);
char* get_ug(uid_t, gid_t);
char* get_group(gid_t);
void time_to_str(char*, time_t);
void extend(char*, char*, int);
