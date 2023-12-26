#include "design.h"
#include <string.h>

// saves message to make it accessible accross methods
char *pattern_message;

void init_color_pairs() {
	init_pair(FILE_COLOR, COLOR_BLACK, COLOR_WHITE);
	init_pair(BORDER_COLOR, COLOR_MAGENTA, -1);
}

void set_color(MENU *menu) {
	// check if the current item is a directory and change the design whether it is or not
	const char *current_item_name = item_name(current_item(menu));
	if (strstr(current_item_name, "/") != NULL) {
		set_menu_fore(menu, COLOR_PAIR(DIR_COLOR));
	}
	else {
		set_menu_fore(menu, COLOR_PAIR(FILE_COLOR));
	}
}

void set_size_menu_window(MENU* menu, WINDOW *windows[], ITEM* item) {
	// delete the menu window, to create a new one, and unpost menu
    if (item != NULL) {
        unpost_menu(menu);
        delwin(windows[MENU_WINDOW]);
    }

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
	mvwin(windows[PATTERN_WINDOW], LINES - 1, 0);

	// remove message to not make it 'permanent'
    if (item != NULL)
	    delete_message(windows);

	// refresh menu window
	refresh();
	wrefresh(menu_win);
	wrefresh(windows[PATTERN_WINDOW]);

	// set references to new windows
	windows[MENU_WINDOW] = menu_win;
	windows[MENU_SUB_WINDOW] = menu_sub_win;
}

void print_box_with_title(WINDOW *windows[], int window_index, size_t y, size_t x, size_t width, size_t height, char *title, char *value, bool centered, int color) {
	// initialize window
	WINDOW *win = newwin(height, width, y, x);
	keypad(win, TRUE);

	// draw box and print title
	if (color != -1)
		wattron(win, COLOR_PAIR(color));
	box(win, 0, 0);
	mvwprintw(win, 0, 2, "%s", title);
	if (color != -1)
		wattroff(win, COLOR_PAIR(color));
	mvwprintw(win, height / 2, centered ? (width / 2) - (strlen(value) / 2) : 1, "%s", value);

	// refresh and set reference
	wrefresh(win);
	refresh();
	windows[window_index] = win;
}

bool centered_confirm_prompt(MENU *menu, WINDOW *windows[], int window_index, char *value, int color, bool text_centered) {
	// create an array where each line is an element
    size_t pos;
    char* copy = strdup(value);

    // Assuming you want to store lines in an array of strings
    linked_list *lines = NULL;

    while ((pos = strcspn(copy, "\n")) != strlen(copy)) {
        copy[pos] = '\0';
        push_back(lines, strdup(copy));
        copy += pos + 1;
    }

    push_back(lines, strdup(copy));

	// initialize height with 'room' for the box
	size_t height = size(lines) + 2;

	// get max width
	size_t width = 0;
    for (size_t i = 0; i < size(lines); i++) {
        char *cur = value_at(lines, i);
        if (strlen(cur) > width)
            width = strlen(cur);
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
	for (size_t i = 0; i < size(lines); i++) {
        char *cur = value_at(lines, i);
		if (text_centered) {
            mvwprintw(win, i + 1, (width / 2) - (strlen(cur) / 2), "%s", cur);
        } else {
            mvwprintw(win, i + 1, 2, "%s", cur);
        }
	}
	if (color != -1)
		wattroff(win, COLOR_PAIR(color));

	// refresh and set reference
	wrefresh(win);
	refresh();
	windows[window_index] = win;

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

	remove_window_index(windows, window_index);
	return c == 'y';
}

void print_message(WINDOW *windows[], char *message) {
    if (windows[MESSAGE_WINDOW]) remove_window_index(windows, MESSAGE_WINDOW);
	// initialize window
	windows[MESSAGE_WINDOW] = newwin(1, strlen(message), LINES - 1, (COLS / 2) - (strlen(message) / 2));

	// print message in color
	wattron(windows[MESSAGE_WINDOW], COLOR_PAIR(BORDER_COLOR));
	mvwprintw(windows[MESSAGE_WINDOW], 0, 0, "%s", message);
	wattroff(windows[MESSAGE_WINDOW], COLOR_PAIR(BORDER_COLOR));

	// set global variable and refresh
	pattern_message = message;
	wrefresh(windows[MESSAGE_WINDOW]);
}

void delete_message(WINDOW *windows[]) {
	// delete window and reset global variable
	// if (windows[MESSAGE_WINDOW])
        // remove_window_index(windows, MESSAGE_WINDOW);
	pattern_message = "";
}

void remove_window_index(WINDOW *windows[], int window_index) {
	// makes the window empty, deletes it and sets the pointer
	wclear(windows[window_index]);
	wrefresh(windows[window_index]);
	delwin(windows[window_index]);
	windows[window_index] = NULL;
}

void remove_window(WINDOW *window) {
	// makes the window empty and deletes it
	wclear(window);
	wrefresh(window);
	delwin(window);
}
