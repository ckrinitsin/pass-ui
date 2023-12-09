#include "design.h"

// saves message to make it accessible accross methods
std::string pattern_message;

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

void set_size_menu_window(MENU *menu, std::vector<WINDOW *> *windows, ITEM *item) {
	// delete the menu window, to create a new one, and unpost menu
	unpost_menu(menu);
	delwin(windows->at(MENU_WINDOW));

	// create menu window
	WINDOW *menu_win = newwin(LINES - 2, COLS, 1, 0);
	WINDOW *menu_sub_win = derwin(menu_win, LINES - 4, COLS - 1, 1, 1);
	keypad(menu_win, TRUE);

	// set main and sub window
	set_menu_win(menu, menu_win);
	set_menu_sub(menu, menu_sub_win);
	set_menu_format(menu, LINES - 4, 1);

	// print a border and set mark_length
	set_menu_mark(menu, " - ");
	box(menu_win, 0, 0);

	// post menu
	post_menu(menu);
	if (item != NULL)
		set_current_item(menu, item);

	// move the pattern window to the right position
	mvwin(windows->at(PATTERN_WINDOW), LINES - 1, 0);

	// remove message to not make it 'permanent'
	delete_message(windows);

	// refresh menu window
	refresh();
	wrefresh(menu_win);
	wrefresh(windows->at(PATTERN_WINDOW));

	// set references to new windows
	windows->at(MENU_WINDOW) = menu_win;
	windows->at(MENU_SUB_WINDOW) = menu_sub_win;
}

void print_box_with_title(std::vector<WINDOW *> *windows, int window_index, size_t y, size_t x, size_t width, size_t height, std::string title, std::string value, bool centered, int color) {
	// initialize window
	WINDOW *win = newwin(height, width, y, x);
	keypad(win, TRUE);

	// draw box and print title
	if (color != -1)
		wattron(win, COLOR_PAIR(color));
	box(win, 0, 0);
	mvwprintw(win, 0, 2, "%s", title.c_str());
	if (color != -1)
		wattroff(win, COLOR_PAIR(color));
	mvwprintw(win, height / 2, centered ? (width / 2) - (value.length() / 2) : 1, "%s", value.c_str());

	// refresh and set reference
	wrefresh(win);
	refresh();
	windows->at(window_index) = win;
}

bool centered_confirm_prompt(MENU *menu, std::vector<WINDOW *> *windows, int window_index, std::string value, int color, bool text_centered) {
	// create an array where each line is an element
	size_t pos;
	std::string copy(value);
	std::vector<std::string> lines;
	while ((pos = copy.find('\n')) != std::string::npos) {
		lines.push_back(copy.substr(0, pos));
		copy.erase(0, pos + 1);
	}
	lines.push_back(copy);

	// initialize height with 'room' for the box
	size_t height = lines.size() + 2;

	// get max width
	size_t width = 0;
	for (std::string s : lines) {
		if (s.size() > width)
			width = s.size();
	}

	// padding
    width += 4;

	// initialize window
	WINDOW *win = newwin(height, width, (LINES / 2) - (height / 2), (COLS / 2) - (width / 2));
	keypad(win, TRUE);

	// draw box and print value
	if (color != -1)
		wattron(win, COLOR_PAIR(color));
	box(win, 0, 0);
	for (size_t i = 0; i < lines.size(); i++) {
		if (text_centered) {
            mvwprintw(win, i + 1, (width / 2) - (lines.at(i).size() / 2), "%s", lines.at(i).c_str());
        } else {
            mvwprintw(win, i + 1, 2, "%s", lines.at(i).c_str());
        }
	}
	if (color != -1)
		wattroff(win, COLOR_PAIR(color));

	// refresh and set reference
	wrefresh(win);
	refresh();
	windows->at(window_index) = win;

	// only react if n, y or q was pressed
	int c;
	while (true) {
		c = getch();
		if (c == KEY_RESIZE) {
            mvwin(win, (LINES / 2) - (height / 2), (COLS / 2) - (width / 2));
			set_size_menu_window(menu, windows, current_item(menu));
            wrefresh(win);
        }
		else if (c == 'n' || c == 'y' || c == 'q')
			break;
	}

	remove_window(windows, window_index);
	return c == 'y';
}

void print_message(std::vector<WINDOW *> *windows, std::string message) {
    if (windows->at(MESSAGE_WINDOW)) remove_window(windows, MESSAGE_WINDOW);
	// initialize window
	windows->at(MESSAGE_WINDOW) = newwin(1, message.length(), LINES - 1, (COLS / 2) - (message.length() / 2));

	// print message in color
	wattron(windows->at(MESSAGE_WINDOW), COLOR_PAIR(BORDER_COLOR));
	mvwprintw(windows->at(MESSAGE_WINDOW), 0, 0, "%s", message.c_str());
	wattroff(windows->at(MESSAGE_WINDOW), COLOR_PAIR(BORDER_COLOR));

	// set global variable and refresh
	pattern_message = message;
	wrefresh(windows->at(MESSAGE_WINDOW));
}

void delete_message(std::vector<WINDOW *> *windows) {
	// delete window and reset global variable
	remove_window(windows, MESSAGE_WINDOW);
	pattern_message = "";
}

void remove_window(std::vector<WINDOW *> *windows, int window_index) {
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
