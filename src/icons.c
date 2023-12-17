#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include "icons.h"
#include "util.h"

struct IconMap extmap[] = {
  {.name=".py",           .icon=""},
  {.name=".js",           .icon=""},
  {.name=".c",            .icon=""},
  {.name=".h",            .icon=""},
  {.name=".cpp",          .icon=""},
  {.name=".hpp",          .icon=""},
  {.name=".go",           .icon="󰟓"},
  {.name=".rs",           .icon="󱘗"},
  {.name=".asm",          .icon=""},
  {.name=".ko",           .icon=""},
  {.name=".s",            .icon=""},
  {.name=".html",         .icon=""},
  {.name=".css",          .icon=""},
  {.name=".java",         .icon=""},
  {.name=".bin",          .icon=""},
  {.name=".elf",          .icon=""},
  {.name=".exe",          .icon=""},
  {.name=".dat",          .icon=""},
  {.name=".php",          .icon="󰌟"},
  {.name=".sh",           .icon="󱆃"},
  {.name=".bash",         .icon=""},
  {.name=".md",           .icon=""},
  {.name=".json",         .icon=""},
  {.name=".conf",         .icon=""},
  {.name=".toml",         .icon=""},
  {.name=".ini",          .icon=""},
  {.name=".cfg",          .icon=""},
  {.name=".deb",          .icon=""},
  {.name=".tgz",          .icon=""},
  {.name=".gz",           .icon=""},
  {.name=".tar",          .icon=""},
  {.name=".tar.gz",       .icon=""},
  {.name=".tar.xz",       .icon=""},
  {.name=".tar.bz2",      .icon=""},
  {.name=".xz",           .icon=""},
  {.name=".bz2",          .icon=""},
  {.name=".txt",          .icon=""},
  {.name=".png",          .icon="󰋩"},
  {.name=".jpg",          .icon="󰋩"},
  {.name=".jpeg",         .icon="󰋩"},
  {.name=".webp",         .icon="󰋩"},
  {.name=".gif",          .icon=""},
  {.name=".mp4",          .icon=""},
  {.name=".mkv",          .icon=""},
  {.name=".ovpn",         .icon="󰖂"},
  {.name=".pub",          .icon=""},
  {.name=".priv",         .icon=""},
  {.name=".key",          .icon=""},
  {.name=".cert",         .icon="󰄤"},
  {.name=".asc",          .icon=""},
  {.name=".sig",          .icon="󰏪"},
  {.name=".bat",          .icon=""},
  {.name=".iso",          .icon=""},
  {.name=".qcow2",        .icon=""},
  {.name=".qcow",         .icon=""},
  {.name=".qcow",         .icon=""},
  {.name=".vdi",          .icon=""},
  {.name=".vbox",         .icon=""},
};

struct IconMap namemap[] = {
  {.name=".git",              .icon="󰊢"},
  {.name=".gitignore",        .icon="󰊢"},
  {.name="Makefile",          .icon=""},
  {.name="package.json",      .icon="󰏗"},
  {.name="package-lock.json", .icon="󰏗"},
  {.name="LICENSE",           .icon="󱤹"},
  {.name="LICENSE.txt",       .icon="󱤹"},
  {.name="LICENSE.md",        .icon="󱤹"},
};

char* find_icon(char* e, struct stat s) {
  for(int i = 0; i < sizeof(namemap)/sizeof(struct IconMap); i++) {
    if(eq(e, namemap[i].name)) {
      return namemap[i].icon;
    }
  }
  
  for(int i = 0; i < sizeof(extmap)/sizeof(struct IconMap); i++) {
    if(eq(e, ".") || eq(e, "..")){
      return "";
    }

    if(extmap[i].name[0] == '.' && ends(e, extmap[i].name)) {
      return extmap[i].icon;
    }
  }

  switch (s.st_mode & S_IFMT) {
    case S_IFDIR:
      return "";
      break;
    case S_IFCHR:        
      return "󰾹";
      break;
    case S_IFBLK:        
      return "";
      break;
    case S_IFLNK: 
      return "󱅷";
      break;
    case S_IFIFO: 
      return "󰟥"; 
      break;
    case S_IFSOCK:
      return "󰟩";
      break;
  }
  
  return "󰈔";
}
