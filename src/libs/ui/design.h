#include <menu.h>
#include <ncurses.h>
#include <string>
#include <vector>

// DEFINE WINDOWS MACROS
#define MENU_WINDOW 0
#define PATTERN_WINDOW 1
#define MENU_SUB_WINDOW 2

void init_color_pairs();

void set_mark_color(MENU *menu);

void set_size_menu_window(MENU* menu, std::vector<WINDOW*> *windows);
