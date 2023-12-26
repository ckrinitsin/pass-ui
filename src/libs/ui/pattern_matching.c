#include "pattern_matching.h"

linked_list *start_pattern_matching(MENU *menu, WINDOW *windows[], linked_list *real_paths) {
	// result of method
	linked_list *matched_items = NULL;

	// saves current item, for the case that pattern matching is cancelled
	ITEM *cur_item = current_item(menu);

	// input char
	int c;

    linked_list *pattern_matching_buffer = NULL;
    push_back(pattern_matching_buffer, allocate_string(""));

	// frees last line, if it wasn't
	for (int i = COLS - 1; i >= 0; i--) {
		mvwdelch(windows[PATTERN_WINDOW], 0, i);
	}

	// moves to last line and prints a '/'
	mvwaddch(windows[PATTERN_WINDOW], 0, 0, '/');
	wrefresh(windows[PATTERN_WINDOW]);

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
			wmove(windows[PATTERN_WINDOW], 0, x_pos);

			break;

		// delete a key
		case KEY_BACKSPACE:
			// if its the first position, exit pattern mode
			if (x_pos == 1) {
				mvwdelch(windows[PATTERN_WINDOW], 0, 0);
				return matched_items;
			}

			// if an additional pattern was deleted, delete the vector variable
			if (mvwinch(windows[PATTERN_WINDOW], 0, x_pos - 1) == ' ') {
                pop_back(pattern_matching_buffer);
			}

			// if not, just delete the last character of the buffer
			else {
				// pattern_matching_buffer.back().pop_back();
                char *buf = back(pattern_matching_buffer)->value;
                buf[strlen(buf) - 1] = '\0';
			}

			// delete the character from the screen
			mvwdelch(windows[PATTERN_WINDOW], 0, --x_pos);
			break;

		// Escape
		case 27:
			// delete whole last line and go back to the selected item before the pattern_matching
			for (int i = x_pos - 1; i >= 0; i--) {
				mvwdelch(windows[PATTERN_WINDOW], 0, i);
			}
			set_current_item(menu, cur_item);
			return NULL;

		// Enter
		case '\n':
			wrefresh(windows[PATTERN_WINDOW]);
			return matched_items;

		default:
			// check if its a printable key
			if (c >= 32 && c <= 126) {
				// if it's a space, make memory for additional pattern
				if (c == ' ') {
                    push_back(pattern_matching_buffer, allocate_string(""));
				}

				// if not, add the character to the buffer
				else {
                    char *tmp = concat_strings(2, back(pattern_matching_buffer)->value, c);
                    free(back(pattern_matching_buffer));
                    back(pattern_matching_buffer)->value = tmp;
				}

				// print the character to the screen
				mvwaddch(windows[PATTERN_WINDOW], 0, x_pos++, c);
				break;
			}
		}

		// search for the pattern
		matched_items = search_menu_pattern(menu, pattern_matching_buffer, real_paths);

		// go to the first item of the vector
		if (size(matched_items) > 0) {
			set_current_item(menu, value_at(matched_items, 0));
		}
		else {
			set_current_item(menu, cur_item);
		}
		set_color(menu);

		// refresh the windows
		wrefresh(windows[MENU_WINDOW]);
		wrefresh(windows[PATTERN_WINDOW]);
	}
}

linked_list *search_menu_pattern(MENU *menu, linked_list *pattern_matching_buffer, linked_list *real_paths) {
	// get all items and initialize result vector
	ITEM **items = menu_items(menu);
    linked_list *vec = NULL;

	// iterate through all real_paths
	for (size_t i = 0; i < size(real_paths); i++) {
		// check for that item, if all patterns in the vector can be found inside the item name
		bool match = true;
		for (size_t j = 0; j < size(pattern_matching_buffer) && *(char*)value_at(pattern_matching_buffer, j) != '\0'; j++) {
			// if (real_paths.at(i).find(pattern_matching_buffer.at(j)) == std::string::npos)
			if (strstr(value_at(real_paths, i), value_at(pattern_matching_buffer, j)) == NULL)
				match = false;
		}

		// if all patterns are in the name, add the item to the vector
		if (match)
            push_back(vec, items[i]);
	}

	return vec;
}
