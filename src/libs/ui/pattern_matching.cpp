#include "pattern_matching.h"

std::vector<ITEM *> start_pattern_matching(MENU *menu, std::vector<WINDOW *> *windows, std::vector<std::string> real_paths) {
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
		mvwdelch(windows->at(PATTERN_WINDOW), 0, i);
	}

	// moves to last line and prints a '/'
	mvwaddch(windows->at(PATTERN_WINDOW), 0, 0, '/');
	wrefresh(windows->at(PATTERN_WINDOW));

	// set the cursor visible and start tracking the x position of cursor
	curs_set(1);
	int x_pos = 1;

	while (true) {
		c = getch();
		switch (c) {
		case KEY_RESIZE:
			// unpost menu and post it to a new window with new specs
			set_size_menu_window(menu, windows, NULL);

			// move the pattern window to right position
			wmove(windows->at(PATTERN_WINDOW), 0, x_pos);

			break;

		// delete a key
		case KEY_BACKSPACE:
			// if its the first position, exit pattern mode
			if (x_pos == 1) {
				mvwdelch(windows->at(PATTERN_WINDOW), 0, 0);
				return matched_items;
			}

			// if an additional pattern was deleted, delete the vector variable
			if (mvwinch(windows->at(PATTERN_WINDOW), 0, x_pos - 1) == ' ') {
				pattern_matching_buffer.pop_back();
			}

			// if not, just delete the last character of the buffer
			else {
				pattern_matching_buffer.back().pop_back();
			}

			// delete the character from the screen
			mvwdelch(windows->at(PATTERN_WINDOW), 0, --x_pos);
			break;

		// Escape
		case 27:
			// delete whole last line and go back to the selected item before the pattern_matching
			for (int i = x_pos - 1; i >= 0; i--) {
				mvwdelch(windows->at(PATTERN_WINDOW), 0, i);
			}
			set_current_item(menu, cur_item);
			return std::vector<ITEM *>();

		// Enter
		case '\n':
			wrefresh(windows->at(PATTERN_WINDOW));
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
				mvwaddch(windows->at(PATTERN_WINDOW), 0, x_pos++, c);
				break;
			}
		}

		// search for the pattern
		matched_items = search_menu_pattern(menu, pattern_matching_buffer, real_paths);

		// go to the first item of the vector
		if (matched_items.size() > 0) {
			set_current_item(menu, matched_items.at(0));
		}
		else {
			set_current_item(menu, cur_item);
		}
		set_color(menu);

		// refresh the windows
		wrefresh(windows->at(MENU_WINDOW));
		wrefresh(windows->at(PATTERN_WINDOW));
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

	return vec;
}
