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

char* get_ug(uid_t uid, gid_t gid) {
  struct group* grp;
  struct passwd* pws;
  char res[70]; 

  pws = getpwuid(uid);
  if(NULL == pws) sprintf(res, "???:");
  else sprintf(res, "%s:", pws->pw_name);

  grp = getgrgid(gid);
  if(NULL == grp) sprintf(res, "%s???", res);
  else sprintf(res, "%s%s", res, grp->gr_name);
 
  return strdup(res);
}

void time_to_str(char* buff, time_t epoch) {
  strftime(buff, 20, "%H:%M:%S:%d/%m/%Y", localtime(&epoch));
}

bool exists(char* path){
  DIR* dir = opendir(path);
  if (dir) {
    closedir(dir);
    return true;
  }else if(errno == ENOTDIR){
    if (access(path, F_OK)!=0) {
      return false;
    }
    return true;
  }

  return false;
}

bool is_dir(char* path) {
  DIR* dir = opendir(path);
  if(dir) {
    closedir(dir);
    return true;
  }

  return false;
}
