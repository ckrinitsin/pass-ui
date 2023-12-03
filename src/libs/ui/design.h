#include <menu.h>
#include <ncurses.h>
#include <string>
#include <string.h>
#include <vector>

// DEFINE WINDOWS MACROS
#define MENU_WINDOW 0
#define PATTERN_WINDOW 1
#define MENU_SUB_WINDOW 2
#define USERNAME_WINDOW 3
#define PASSWORD_WINDOW 4
#define BORDER_WINDOW 5

// DEFINE COLOR MACROS
#define DIR_COLOR 0
#define FILE_COLOR 1
#define BORDER_COLOR 2

void init_color_pairs();

void set_color(MENU *menu);

void set_size_menu_window(MENU* menu, std::vector<WINDOW*> *windows, ITEM* item);

void print_box_with_title(std::vector<WINDOW*> *windows, int window_index, size_t y, size_t x, size_t width, size_t height, std::string title, std::string value, int color);

void remove_window(std::vector<WINDOW*> *windows, int window_index);

void remove_window(WINDOW *window);

void resize_menu();
