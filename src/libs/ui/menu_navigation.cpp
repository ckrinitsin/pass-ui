#include "menu_navigation.h"

void navigation(MENU *menu, WINDOW *menu_win, struct vault *vault) {
	// vector which stores all items, which are matched with the pattern, index for navigating through them
	std::vector<ITEM *> pattern_vector;
	size_t pattern_index = 0;

	// input char
	int c;

	// checker for double g
	bool g = false;

	while ((c = wgetch(menu_win)) != 'q') {
		switch (c) {
		// called when the window is resized
		case KEY_RESIZE:
			mvaddch(50, 100, 'a');
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
			pattern_vector = start_pattern_matching(menu, menu_win, vault->api_entry);
			pattern_index = 0;
			curs_set(0);
			break;
		}

		// reset 'g' input
		if (c != 'g')
			g = false;

		// if the cursor is on a directory, change the color and remove the mark
		set_mark_fore(menu);

		// refresh window
		wrefresh(menu_win);
		refresh();
	}
}
