#include "design.h"

void init_color_pairs() {
	init_pair(1, COLOR_RED, -1);
	init_pair(2, COLOR_BLUE, -1);
}

void set_mark_color(MENU *menu) {
    // check if the current item is a directory and change the design whether it is or not
	std::string current_item_name(item_name(current_item(menu)));
	if (current_item_name.find('/') != std::string::npos) {
		set_menu_mark(menu, "   ");
		set_menu_fore(menu, COLOR_PAIR(2));
	}
	else {
		set_menu_mark(menu, " * ");
		set_menu_fore(menu, COLOR_PAIR(1));
	}
}

void set_size_menu_window(MENU *menu, std::vector<WINDOW *> *windows) {

    // delete the menu window, to create a new one
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
	set_menu_mark(menu, "   ");
	box(menu_win, 0, 0);

	// post menu
	post_menu(menu);

    // refresh menu window
	refresh();
	wrefresh(menu_win);

    // set references to new windows
	windows->at(MENU_WINDOW) = menu_win;
	windows->at(MENU_SUB_WINDOW) = menu_sub_win;
}
