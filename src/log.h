#pragma once
#include "log.h"

#define UNDERLINE "\e[4m"
#define YELLOW "\e[33m"
#define RESET "\e[0m"
#define GREEN "\e[32m"
#define GRAY "\e[37m"
#define BLUE "\e[36m"
#define BOLD "\e[1m"
#define RED "\e[31m"

void info(const char *, ...);
void error(const char *, ...);
void table();
