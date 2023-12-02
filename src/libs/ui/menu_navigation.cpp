#include "menu_navigation.h"

void navigation(MENU *menu, std::vector<WINDOW *> *windows, struct vault *vault) {
	// for some operations (like RESIZE), we need to save the current item to restore it later
	ITEM *cur_item;

	// vector which stores all items, which are matched with the pattern, index for navigating through them
	std::vector<ITEM *> pattern_vector;
	size_t pattern_index = 0;

	// input char
	int c;

	// checker for double g
	bool g = false;

	while ((c = wgetch(windows->at(MENU_WINDOW))) != 'q') {
		switch (c) {
		// called when the window is resized
		case KEY_RESIZE:

			// save the current item, to restore it later
			cur_item = current_item(menu);

			// unpost menu and post it to a new window with new specs
			unpost_menu(menu);
			set_size_menu_window(menu, windows);

			// move the pattern window accordingly to new window
			mvwin(windows->at(PATTERN_WINDOW), LINES - 1, 0);

			// restore current item
			set_current_item(menu, cur_item);

			break;

		// bottom
		case KEY_DOWN:
		case 'j':
			menu_driver(menu, REQ_DOWN_ITEM);
			break;

		// up
		case KEY_UP:
		case 'k':
			menu_driver(menu, REQ_UP_ITEM);
			break;

		// down a page
		case KEY_NPAGE:
			menu_driver(menu, REQ_SCR_DPAGE);
			break;

		// up a page
		case KEY_PPAGE:
			menu_driver(menu, REQ_SCR_UPAGE);
			break;

		// go to last item
		case 'G':
			menu_driver(menu, REQ_LAST_ITEM);
			break;

		// go to first item
		case 'g':
			if (g) {
				menu_driver(menu, REQ_FIRST_ITEM);
				g = false;
			}
			else {
				g = true;
			}
			break;

		// get next entry of matched pattern
		case 'n':
			if (pattern_vector.size() > 1) {
				pattern_index = (pattern_index == pattern_vector.size() - 1) ? 0 : pattern_index + 1;
				set_current_item(menu, pattern_vector.at(pattern_index));
			}
			break;

		// get previous entry of matched pattern
		case 'N':
			if (pattern_vector.size() > 1) {
				pattern_index = (pattern_index == 0) ? pattern_vector.size() - 1 : pattern_index - 1;
				set_current_item(menu, pattern_vector.at(pattern_index));
			}
			break;

		// start pattern matching
		case '/':
			pattern_vector = start_pattern_matching(menu, windows, vault->api_entry);
			pattern_index = 0;
			curs_set(0);
			break;
		}

		// reset 'g' input
		if (c != 'g')
			g = false;

		// if the cursor is on a directory, change the color and remove the mark
		set_mark_color(menu);

		// refresh window
		wrefresh(windows->at(MENU_WINDOW));
		wrefresh(windows->at(PATTERN_WINDOW));
		refresh();
	}
}
