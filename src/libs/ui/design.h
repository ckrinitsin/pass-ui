#include <menu.h>
#include <ncurses.h>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>

// DEFINE WINDOWS MACROS
#define MENU_WINDOW 0
#define PATTERN_WINDOW 1
#define MENU_SUB_WINDOW 2
#define USERNAME_WINDOW 3
#define PASSWORD_WINDOW 4
#define BORDER_WINDOW 5
#define MESSAGE_WINDOW 6
#define CONFIRM_PROMPT_WINDOW 7

// DEFINE COLOR MACROS
#define DIR_COLOR 0
#define FILE_COLOR 1
#define BORDER_COLOR 2

void init_color_pairs();

void set_color(MENU *menu);

void set_size_menu_window(MENU* menu, std::vector<WINDOW*> *windows, ITEM* item);

void print_box_with_title(std::vector<WINDOW*> *windows, int window_index, size_t y, size_t x, size_t width, size_t height, std::string title, std::string value, bool centered, int color);
bool centered_confirm_prompt(std::vector<WINDOW*> *windows, int window_index, std::string value, int color);

void print_message(std::vector<WINDOW*> *windows, std::string message);
void delete_message(std::vector<WINDOW*> *windows);

void remove_window(std::vector<WINDOW*> *windows, int window_index);

void remove_window(WINDOW *window);

void resize_menu();
