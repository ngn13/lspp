/*
 *  lspp | ls plus plus 
 *  Copyright (C) 2023-2024 ngn
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <pwd.h>

#include "options.h"
#include "util.h"
#include "dir.h"
#include "log.h"

int main(int argc, char** argv) {
  char* target = ".";

  if (argc == 1) {
    info("Usage: %s [OPTIONS] [PATH]", argv[0]);
    printf(GRAY"  Run "RESET BOLD"%s -h"RESET GRAY" get more info\n"RESET, argv[0]);
    return EXIT_FAILURE;
  }

  if (eq(argv[1], "-h")) {
    printf("\n");
    printf(BOLD BLUE UNDERLINE"lspp - ls plus plus\n"RESET);
    printf(BLUE"A minimal alternative to coreutils' \"ls\"\n\n"RESET);
    info("Usage: %s [OPTIONS] [PATH]\n", argv[0]);
    printf(
        BOLD"PATH: "RESET GRAY
        "   Path to the directory/file, if not specified, then refers to the current directory\n"
        RESET
    );
    printf(BOLD"OPTIONS: "RESET GRAY"Command options\n"RESET);
    printf(GRAY"   -h     Shows the help output, a.k.a what you are looking at rn\n");
    printf(GRAY"   -l     Lists the content of the directory (without the hidden files)\n");
    printf(GRAY"   -a     Lists the content of the directory (with the hidden files)\n"RESET);
    printf("\n");
    return EXIT_SUCCESS;
  }else if (eq(argv[1], "-l")) {
    show_hidden = false;
  }else if (eq(argv[1], "-a")) {
    show_hidden = true;
  }else {
    error(
        "Invalid option: run "RESET RED BOLD"%s -h"RESET BOLD" to see all the options"RESET, argv[0]
    );
    return EXIT_FAILURE;
  }

  if (argc == 3) target = argv[2];
  if (!exists(target)) {
    error("No such a file or directory: %s", target);
    return EXIT_FAILURE;
  }

  bool res = false;
  if(is_dir(target)) {
    res = listdir(target);
  }else {
    struct stat st;
    struct FileInfo inf;

    if(lstat(target, &st) < 0){
      error("Cannot stat: %s", target);
      goto EXIT;
    }

    inf.st = st; inf.name = strdup(target);
    inf.user_grp = get_ug(st.st_uid, st.st_gid);
    inf.ug_len = strlen(inf.user_grp);
    print_file(inf);
  }

EXIT:
  if(res) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}
