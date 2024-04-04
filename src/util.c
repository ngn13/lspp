#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

bool eq(char *s1, char *s2) {
  if (strlen(s1) != strlen(s2))
    return false;
  return strcmp(s1, s2) == 0;
}

bool ends(char *str, char *sub) {
  int str_len = strlen(str);
  int sub_len = strlen(sub);

  if (str_len < sub_len)
    return false;
  return !strcmp(str + str_len - sub_len, sub);
}

char *get_ug(uid_t uid, gid_t gid) {
  struct group *grp;
  struct passwd *pws;
  char res[70] = {0};

  pws = getpwuid(uid);
  if (NULL == pws)
    sprintf(res, "???:");
  else
    sprintf(res, "%s:", pws->pw_name);

  grp = getgrgid(gid);
  if (NULL == grp)
    sprintf(res, "%s???", res);
  else
    sprintf(res, "%s%s", res, grp->gr_name);

  return strdup(res);
}

void time_to_str(char *buff, time_t epoch) {
  strftime(buff, 20, "%H:%M:%S:%d/%m/%Y", localtime(&epoch));
}

bool exists(char *path) {
  return access(path, F_OK) == 0;
  /*  DIR* dir = opendir(path);
    if (dir) {
      closedir(dir);
      return true;
    }else if(errno == ENOTDIR){
      if (access(path, F_OK)!=0)
        return false;
      return true;
    }

    return false;*/
}

bool is_dir(char *path) {
  struct stat st;
  stat(path, &st);
  return st.st_mode & S_IFDIR;
}
