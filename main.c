/*
 *  lspp - ls plus plus 
 *  ===========================
 *  A minimal and alternative to 
 *  classic old coreutils "ls"
 *
 *  This project is under GNU Public
 *  License Version 2 (GPLv2), please 
 *  see LICENSE.txt
 *
 *  Written by ngn - https://ngn13.fun
 *
 */

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>

#define RED "\e[31m"
#define GRAY "\e[37m"
#define BOLD "\e[1m"
#define UNDERLINE "\e[4m"
#define GREEN "\e[32m"
#define BLUE "\e[36m"
#define RESET "\e[0m"

struct FileMap {
  char* name;
  char* icon;
};

struct FileMap fm[] = {
  {.name=".py", .icon=""},
  {.name=".js", .icon=""},
  {.name=".c", .icon=""},
  {.name=".cpp", .icon=""},
  {.name=".go", .icon="󰟓"},
  {.name=".asm", .icon=""},
  {.name=".s", .icon=""},
  {.name=".html", .icon=""},
  {.name=".css", .icon=""},
  {.name=".java", .icon=""},
  {.name=".bin", .icon=""},
  {.name=".dat", .icon=""},
  {.name=".php", .icon="󰌟"},
  {.name=".sh", .icon="󱆃"},
  {.name=".bash", .icon=""},
  {.name=".md", .icon=""},
  {.name=".json", .icon=""},
  {.name=".conf", .icon=""},
  {.name=".cfg", .icon=""},
  {.name="Makefile", .icon=""},
  {.name="LICENSE", .icon="󱤹"},
  {.name="LICENSE.txt", .icon="󱤹"},
  {.name="LICENSE.md", .icon="󱤹"},
};

int fm_len = sizeof(fm)/sizeof(struct FileMap);
bool show_hidden;

char* get_user(uid_t uid){
  char* res = (char*)malloc(sizeof(char)*9);
  struct passwd *pws;
  pws = getpwuid(uid);

  for(int i = 0; i < 8; i++){
    res[i] = ' ';
  }

  if (strlen(pws->pw_name) > 5) {
    for(int i = 0; i<5; i++) {
      res[i] = pws->pw_name[i];
    }
    res[5] = '.';
    res[6] = '.';
    res[7] = '.';
  }else {
    for(int i = 0; i<strlen(pws->pw_name); i++) {
      res[i] = pws->pw_name[i];
    }
  }

  return res;
}

char* time_to_str(time_t epoch) {
  struct tm *gm = gmtime(&epoch);
  char* res = asctime(gm);
  res[strlen(res) - 1] = ' ';
  return res;
}

char* get_icon(char* fn) {
  for (int i = 0; i < fm_len; i++){
    if (strstr(fn, fm[i].name)!=NULL) {
      return fm[i].icon;
    }
  }

  return "";
}

void print_file(char* dir, struct dirent* f) {
  if (!show_hidden && f->d_name[0] == '.') {
    return;
  }

  struct stat st;
  char *fp, *perms, *icon, *resolved, *user, *lastmod;

  perms = (char*)malloc(sizeof(char)*50);
  resolved = (char*)malloc(sizeof(char)*80);
  fp = (char*)malloc(sizeof(char)*(strlen(dir)+strlen(f->d_name)+10));

  if (dir[strlen(dir)-1] == '/') {
    sprintf(fp, "%s%s", dir, f->d_name);
  } else {
    sprintf(fp, "%s/%s", dir, f->d_name);
  }

  if (lstat(fp, &st) != 0) {
    printf(RED" Cannot stat file: "RESET BOLD"%s "RESET RED"Error: %d\n"RESET, fp, errno);
    return;
  }

  sprintf(perms, "%s", (st.st_mode & S_IRUSR) ? "r" : "-");
  sprintf(perms, "%s%s", perms, (st.st_mode & S_IWUSR) ? "w" : "-");
  sprintf(perms, "%s%s ", perms, (st.st_mode & S_IXUSR) ? "x" : "-");

  sprintf(perms, "%s%s", perms, (st.st_mode & S_IRGRP) ? "r" : "-");
  sprintf(perms, "%s%s", perms, (st.st_mode & S_IWGRP) ? "w" : "-");
  sprintf(perms, "%s%s ", perms, (st.st_mode & S_IXGRP) ? "x" : "-");

  sprintf(perms, "%s%s", perms, (st.st_mode & S_IROTH) ? "r" : "-");
  sprintf(perms, "%s%s", perms, (st.st_mode & S_IWOTH) ? "w" : "-");
  sprintf(perms, "%s%s ", perms, (st.st_mode & S_IXOTH) ? "x" : "-");
  
  user = get_user(st.st_uid);
  lastmod = time_to_str(st.st_mtime);

  switch (f->d_type) {
    case DT_DIR:
      printf(GRAY"%s %s %s"RESET BLUE"   %s\n"RESET, perms, lastmod, user, f->d_name);
      break;
    case DT_REG:
      icon = get_icon(f->d_name);
      printf(GRAY"%s %s %s"RESET GREEN"  %s %s\n"RESET, perms, lastmod, user, icon, f->d_name);
      break;
    case DT_LNK:
      if(readlink(fp, resolved, 50)==-1) {
        printf(GRAY"%s %s %s"RESET RED"   %s\n"RESET, perms, lastmod, user, f->d_name);
        break;
      }
      printf(GRAY"%s %s %s"RESET RED"   %s  %s\n"RESET, perms, lastmod, user, f->d_name, resolved);
      break;
    default: 
      printf(GRAY"%s %s %s"RESET"  󰈔 %s\n"RESET, perms, lastmod, user, f->d_name);
      break;
  }

  free(resolved);
  free(user);
  free(perms);
  free(fp);
}

int main(int argc, char** argv) {
  struct dirent* f;
  DIR* current;
  char* dir;

  if (argc == 1) {
    printf(BOLD"  Usage: %s [OPTIONS] [PATH]\n"RESET, argv[0]);
    printf(GRAY"  Run "RESET BOLD"%s -h"RESET GRAY" get more information\n"RESET, argv[0]);
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "-h")==0) {
    printf("\n");
    printf(BOLD BLUE UNDERLINE"lspp - ls plus plus\n"RESET);
    printf(BLUE"A minimal alternative to coreutils \"ls\"\n\n"RESET);

    printf(BOLD"  Usage: %s [OPTIONS] [PATH]\n\n"RESET, argv[0]);
  
    printf(BOLD"PATH: "RESET GRAY"   Path to the directory, if not specified, then refers to the current directory\n"RESET);
    printf(BOLD"OPTIONS: "RESET GRAY"Command options\n"RESET);
    printf(GRAY"   -h     Shows the help output, a.k.a what you are looking at rn\n");
    printf(GRAY"   -c     Lists the content of the directory (without the hidden files)\n");
    printf(GRAY"   -a     Lists the content of the directory (with the hidden files)\n"RESET);
    printf("\n");

    return EXIT_SUCCESS;
  }else if (strcmp(argv[1], "-c")==0) {
    show_hidden = false;
  }else if (strcmp(argv[1], "-a")==0) {
    show_hidden = true;
  }else {
    printf(RED"  Invalid option, run "RESET BOLD"%s -h"RESET RED" to see all the options\n"RESET, argv[0]);
    return EXIT_FAILURE;
  }

  if (argc == 3) {
    current = opendir(argv[2]);
    dir = argv[2];
  }else {
    current = opendir(".");
    dir = ".";
  }

  if (current == NULL) {
    printf(RED" Cannot open the directory\n"RESET);
    return EXIT_FAILURE;
  }

  printf("\n");

  printf(BOLD UNDERLINE"Permissions"RESET); 
  printf("  ");
  printf(BOLD UNDERLINE"Last Modification       "RESET); 
  printf("  ");
  printf(BOLD UNDERLINE"Owned By"RESET); 
  printf("  ");
  printf(BOLD UNDERLINE"Filename\n"RESET);
  while((f = readdir(current) ) != NULL) {
    print_file(dir, f);
  }

  printf("\n");
  closedir(current);
  return EXIT_SUCCESS;
}
