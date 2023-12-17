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
#include "dir.h"

int sort_func(const void* a, const void* b){ 
  const struct FileInfo* s1 = a;
  const struct FileInfo* s2 = b;
  return strcmp(s1->name, s2->name); 
}

void sprint_perms(char* perms, struct stat s) {
  sprintf(perms, "%s", (s.st_mode & S_IRUSR) ? "r" : "-");
  sprintf(perms, "%s%s", perms, (s.st_mode & S_IWUSR) ? "w" : "-");
  sprintf(perms, "%s%s ", perms, (s.st_mode & S_IXUSR) ? "x" : "-");

  sprintf(perms, "%s%s", perms, (s.st_mode & S_IRGRP) ? "r" : "-");
  sprintf(perms, "%s%s", perms, (s.st_mode & S_IWGRP) ? "w" : "-");
  sprintf(perms, "%s%s ", perms, (s.st_mode & S_IXGRP) ? "x" : "-");

  sprintf(perms, "%s%s", perms, (s.st_mode & S_IROTH) ? "r" : "-");
  sprintf(perms, "%s%s", perms, (s.st_mode & S_IWOTH) ? "w" : "-");
  sprintf(perms, "%s%s", perms, (s.st_mode & S_IXOTH) ? "x" : "-");
} 

bool print_file(struct FileInfo f) {
  char perms[12];
  sprint_perms(perms, f.st);
  perms[11] = '\0';

  char lastmod[30];
  time_to_str(lastmod, f.st.st_mtime);

  char size[20];
  if(f.st.st_size > 1024*1024*1024) {
    sprintf(size, "%luGB", f.st.st_size/1024/1024/1024);
  }else if(f.st.st_size > 1024*1024) {
    sprintf(size, "%luMB", f.st.st_size/1024/1024);
  }else if(f.st.st_size > 1024) {
    sprintf(size, "%luKB", f.st.st_size/1024);
  }else {
    sprintf(size, "%luB", f.st.st_size);
  }

  char spaces[8];
  int curr_index = 0;

  for(int i = 0; i < 7; i++) {
    if(i+1 > strlen(size)) {
      spaces[curr_index] = ' ';
      spaces[curr_index+1] = '\0';
      curr_index++;
    }
  }

  char* icon = find_icon(f.name, f.st);
  char resolved[PATH_MAX];
  int resolve_len;

  switch (f.st.st_mode & S_IFMT) {
    case S_IFDIR:
      printf(GRAY"d%s %s %s %s%s"RESET BLUE"%s %s\n"RESET, 
          perms, f.user_grp, lastmod, size, spaces, icon, f.name);
      break;
    case S_IFCHR:        
      printf(GRAY"c%s %s %s %s%s"RESET YELLOW"%s %s\n"RESET, 
          perms, f.user_grp, lastmod, size, spaces, icon, f.name);
      break;
    case S_IFBLK:        
      printf(GRAY"b%s %s %s %s%s"RESET YELLOW"%s %s\n"RESET, 
          perms, f.user_grp, lastmod, size, spaces, icon, f.name);
      break;
    case S_IFLNK: 
      resolve_len = readlink(f.name, resolved, sizeof(resolved)-1);
      if(resolve_len < 0) {
        printf(GRAY"l%s %s %s %s%s"RESET RED"%s %s  ????\n"RESET, 
          perms, f.user_grp, lastmod, size, spaces, icon, f.name);
        break;
      }

      resolved[resolve_len] = '\0';
      printf(GRAY"l%s %s %s %s%s"RESET RED"%s %s  %s\n"RESET, 
          perms, f.user_grp, lastmod, size, spaces, icon, f.name, resolved);
      break;
    case S_IFIFO: 
      printf(GRAY"p%s %s %s %s%s"RESET YELLOW"%s %s\n"RESET, 
          perms, f.user_grp, lastmod, size, spaces, icon, f.name);
      break;
    case S_IFSOCK:
      printf(GRAY"s%s %s %s %s%s"RESET YELLOW"%s %s\n"RESET, 
          perms, f.user_grp, lastmod, size, spaces, icon, f.name);
      break;
    default:
      printf(GRAY"-%s %s %s "BOLD"%s"RESET GRAY"%s"RESET GREEN"%s %s\n"RESET, 
          perms, f.user_grp, lastmod, size, spaces, icon, f.name);
      break;
  }
  
  return true;
}

bool listdir(char* dir) {
  DIR* d = opendir(dir);
  if (NULL == d) {
    error("Cannot open directory: %s", dir);
    return false;
  }

  char cwd[NAME_MAX];
  if(NULL!=getcwd(cwd, sizeof(cwd))) {
    if(eq(cwd, dir)) {
      goto PRINT_DIR;
    }
  }

  if(chdir(dir) < 0) {
    error("Cannot change directory: %s", dir);
    return false;
  };

  struct FileInfo* entries = malloc(sizeof(struct FileInfo));
  int entry_count = 0;
  struct dirent* ent; 
  int max_ug = 0;

PRINT_DIR:
  while((ent = readdir(d))!=NULL) {
    if(!show_hidden && ent->d_name[0] == '.') {
      continue;
    }

    struct stat st;
    struct FileInfo inf;

    if(lstat(ent->d_name, &st) < 0){
      error("Cannot stat: %s", ent->d_name);
      goto DONE;
    }

    inf.st = st; inf.name = strdup(ent->d_name);
    inf.user_grp = get_ug(st.st_uid, st.st_gid);
    inf.ug_len = strlen(inf.user_grp);
    if(inf.ug_len > max_ug) max_ug = inf.ug_len;

    entries[entry_count] = inf; entry_count++;
    entries = realloc(entries, sizeof(struct FileInfo)*(entry_count+1));
  }

  if(entry_count == 0) {
    goto DONE;
  }
  
  qsort(entries, entry_count, sizeof(struct FileInfo), sort_func); 
  printf(BOLD"Listing %d entries\n"RESET, entry_count);
  for(int i = 0; i < entry_count; i++){
    if(entries[i].ug_len < max_ug) {
      for(int c = 0; c < max_ug; c++) {
        if(c+1 > entries[i].ug_len) {
          entries[i].user_grp[c] = ' ';
          entries[i].user_grp[c+1] = '\0';
        }
      }
    }
    print_file(entries[i]);
  }

DONE:
  for(int i = 0; i < entry_count; i++){
    free(entries[i].name);
  }
  free(entries);
  closedir(d);
  return true;
}
