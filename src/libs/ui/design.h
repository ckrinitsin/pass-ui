#include "../utils/structs.h"

#include <menu.h>
#include <ncurses.h>
#include <string.h>

// DEFINE WINDOWS MACROS
#define MENU_WINDOW 0
#define PATTERN_WINDOW 1
#define MENU_SUB_WINDOW 2
#define USERNAME_WINDOW 3
#define PASSWORD_WINDOW 4
#define TITLE_WINDOW 5
#define BORDER_WINDOW 6
#define MESSAGE_WINDOW 7
#define CONFIRM_PROMPT_WINDOW 8

// DEFINE COLOR MACROS
#define DIR_COLOR 0
#define FILE_COLOR 1
#define BORDER_COLOR 2

void init_color_pairs();

void set_color(MENU *menu);

void set_size_menu_window(MENU* menu, WINDOW *windows[], ITEM* item);

void print_box_with_title(WINDOW *windows[], int window_index, size_t y, size_t x, size_t width, size_t height, char *title, char *value, bool centered, int color);
bool centered_confirm_prompt(MENU *menu, WINDOW *windows[], int window_index, char *value, int color, bool text_centered);

void print_message(WINDOW *windows[], char *message);
void delete_message(WINDOW *windows[]);

void remove_window_index(WINDOW *windows[], int window_index);

void remove_window(WINDOW *window);

void resize_menu();
