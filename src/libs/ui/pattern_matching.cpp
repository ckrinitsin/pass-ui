#include "pattern_matching.h"

std::vector<ITEM *> start_pattern_matching(MENU *menu, WINDOW *menu_win, std::vector<std::string> real_paths) {
	// result of method
	std::vector<ITEM *> matched_items;

	// saves current item, for the case that pattern matching is cancelled
	ITEM *cur_item = current_item(menu);

	// input char
	int c;

	// int buffer_index = 0;
	std::vector<std::string> pattern_matching_buffer(1, "");

	// frees last line, if it wasn't
	for (int i = COLS - 1; i >= 0; i--) {
		mvdelch(LINES - 1, i);
	}

	// moves to last line and prints a '/'
	move(LINES - 1, 0);
	addch('/');

	// set the cursor visible and start tracking the x position of cursor
	curs_set(1);
	int x_pos = 1;

	while (true) {
		c = getch();
		switch (c) {
		// delete a key
		case KEY_BACKSPACE:
			// if its the first position, exit pattern mode
			if (x_pos == 1) {
				mvdelch(LINES - 1, 0);
				return matched_items;
			}

			// if an additional pattern was deleted, delete the vector variable
			if (mvinch(LINES - 1, x_pos - 1) == ' ') {
				pattern_matching_buffer.pop_back();
			}

			// if not, just delete the last character of the buffer
			else {
				pattern_matching_buffer.back().pop_back();
			}

			// delete the character from the screen
			mvdelch(LINES - 1, --x_pos);
			break;

		// Escape
		case 27:
			// delete whole last line and go back to the selected item before the pattern_matching
			for (int i = x_pos - 1; i >= 0; i--) {
				mvdelch(LINES - 1, i);
			}
			set_current_item(menu, cur_item);
			return std::vector<ITEM *>();

		// Enter
		case '\n':
			return matched_items;

		default:
			// check if its a printable key
			if (c >= 32 && c <= 126) {
				// if it's a space, make memory for additional pattern
				if (c == ' ') {
					pattern_matching_buffer.push_back("");
				}

				// if not, add the character to the buffer
				else {
					pattern_matching_buffer.back() += c;
				}

				// print the character to the screen
				mvaddch(LINES - 1, x_pos++, c);
				break;
			}
		}

		// search for the pattern
		matched_items = search_menu_pattern(menu, pattern_matching_buffer, real_paths);

		// refresh the window
		wrefresh(menu_win);
		refresh();
	}
}

std::vector<ITEM *> search_menu_pattern(MENU *menu, std::vector<std::string> pattern_matching_buffer, std::vector<std::string> real_paths) {
	// get all items and initialize result vector
	ITEM **items = menu_items(menu);
	std::vector<ITEM *> vec;

	// iterate through all real_paths
	for (size_t i = 0; i < real_paths.size(); i++) {
		// check for that item, if all patterns in the vector can be found inside the item name
		bool match = true;
		for (size_t j = 0; j < pattern_matching_buffer.size() && pattern_matching_buffer.at(j) != ""; j++) {
			if (real_paths.at(i).find(pattern_matching_buffer.at(j)) == std::string::npos)
				match = false;
		}

		// if all patterns are in the name, add the item to the vector
		if (match)
			vec.push_back(items[i]);
	}

	// go to the first item of the vector
	if (vec.size() > 0) {
		set_current_item(menu, vec.at(0));
		set_mark_fore(menu);
	}

	return vec;
}

void set_mark_fore(MENU *menu) {
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
