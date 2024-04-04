#pragma once
#include <grp.h>
#include <pwd.h>
#include <stdbool.h>
#include <time.h>

bool eq(char *, char *);
bool ends(char *, char *);
bool exists(char *);
bool is_dir(char *);
char *get_user(uid_t);
char *get_ug(uid_t, gid_t);
char *get_group(gid_t);
void time_to_str(char *, time_t);
void extend(char *, char *, int);
