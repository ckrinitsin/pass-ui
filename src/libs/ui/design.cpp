#include "design.h"

void init_color_pairs() {
	init_pair(FILE_COLOR, COLOR_BLACK, COLOR_WHITE);
    init_pair(BORDER_COLOR, COLOR_MAGENTA, -1);
}

void set_color(MENU *menu) {
	// check if the current item is a directory and change the design whether it is or not
	std::string current_item_name(item_name(current_item(menu)));
	if (current_item_name.find('/') != std::string::npos) {
		set_menu_fore(menu, COLOR_PAIR(DIR_COLOR));
	}
	else {
		set_menu_fore(menu, COLOR_PAIR(FILE_COLOR));
	}
}

void set_size_menu_window(MENU *menu, std::vector<WINDOW *> *windows, ITEM* item) {
	// delete the menu window, to create a new one, and unpost menu
	unpost_menu(menu);
	delwin(windows->at(MENU_WINDOW));

	// create menu window
	WINDOW *menu_win = newwin(LINES - 1, COLS, 0, 0);
	WINDOW *menu_sub_win = derwin(menu_win, LINES - 3, COLS - 1, 1, 1);
	keypad(menu_win, TRUE);

	// set main and sub window
	set_menu_win(menu, menu_win);
	set_menu_sub(menu, menu_sub_win);
	set_menu_format(menu, LINES - 3, 1);

	// print a border and set mark_length
	set_menu_mark(menu, " * ");
	box(menu_win, 0, 0);

	// post menu
	post_menu(menu);
    if (item != NULL) set_current_item(menu, item);
	
    // move the pattern window to the right position
    mvwin(windows->at(PATTERN_WINDOW), LINES - 1, 0);

	// refresh menu window
	refresh();
	wrefresh(menu_win);
	wrefresh(windows->at(PATTERN_WINDOW));

	// set references to new windows
	windows->at(MENU_WINDOW) = menu_win;
	windows->at(MENU_SUB_WINDOW) = menu_sub_win;
}

void print_box_with_title(std::vector<WINDOW*> *windows, int window_index, size_t y, size_t x, size_t width, size_t height, std::string title, std::string value, int color) {
    // initialize window
    WINDOW *win = newwin(height, width, y, x);
    keypad(win, TRUE);

    // draw box and print title
    if (color != -1) wattron(win, COLOR_PAIR(color));
	box(win, 0, 0);
    mvwprintw(win, 0, 2, "%s", title.c_str());
    if (color != -1) wattroff(win, COLOR_PAIR(color));
    if (value.compare("") != 0) mvwprintw(win, height/2, 1, "%s", value.c_str());

    // refresh and set reference
    wrefresh(win);
    refresh();
    windows->at(window_index) = win;
}

void remove_window(std::vector<WINDOW*> *windows, int window_index) {
    // makes the window empty, deletes it and sets the pointer
    wclear(windows->at(window_index));
    wrefresh(windows->at(window_index));
    delwin(windows->at(window_index));
    windows->at(window_index) = NULL;
}

void remove_window(WINDOW *window) {
    // makes the window empty and deletes it
    wclear(window);
    wrefresh(window);
    delwin(window);
}
