#include "display.h"

#include <form.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <string>

char* trim_whitespaces(char *str);

std::string rename(std::vector<WINDOW *> *windows, MENU* menu, std::string value);
