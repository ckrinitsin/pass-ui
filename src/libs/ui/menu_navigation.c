#include "menu_navigation.h"

char *help_msg = "e               edit your password using nano\n"
					   "c               copy your password in clipboard\n"
					   "R               refresh the list\n"
					   "r               rename the password or directory\n"
					   "d               delete the password or the directory\n"
					   "ENTER           show your password information";


int navigation(MENU *menu, WINDOW *windows[], struct vault *vault) {
	// vector which stores all items, which are matched with the pattern, index for navigating through them
	linked_list *pattern_vector = NULL;
	size_t pattern_index = 0;

	// saves the output of various commands
	linked_list *output = NULL;

	// input char
	int c;

	// checker for double g
	bool g = false;

	char *prompt;

	while ((c = wgetch(windows[MENU_WINDOW])) != 'q') {
		switch (c) {
		// called when the window is resized
		case KEY_RESIZE:
			// unpost menu and post it to a new window with new specs
			set_size_menu_window(menu, windows, current_item(menu));

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
			if (size(pattern_vector) > 1) { /// TODO: dont need that?
				pattern_index = (pattern_index == size(pattern_vector) - 1) ? 0 : pattern_index + 1;
				set_current_item(menu, value_at(pattern_vector, pattern_index));
			}
			break;

		// get previous entry of matched pattern
		case 'N':
			if (size(pattern_vector) > 1) { /// TODO: dont need that?
				pattern_index = (pattern_index == 0) ? size(pattern_vector) - 1 : pattern_index - 1;
				set_current_item(menu, value_at(pattern_vector, pattern_index));
			}
			break;

		// start pattern matching
		case '/':
			pattern_vector = start_pattern_matching(menu, windows, vault->api_entry);
			pattern_index = 0;
			curs_set(0);
			break;

		// show entry information
		case '\n':
			// remove message at bottom bar
			delete_message(windows);

			// if the item is not a directory, but a password file, get the information into a vector
			if (strstr(value_at(vault->entry, item_index(current_item(menu))), "/") != NULL)
				break;
			output = get_pass_information(output, value_at(vault->api_entry, item_index(current_item(menu))));

            if (size(output) == 0) break;

			// create a username_password pop up
			username_password_display(windows, output, value_at(vault->api_entry, item_index(current_item(menu))), menu);

			// clear the vector
			output = erase_heap(output);
			break;

		// copy password into clipboard
		case 'c':
			// check if item is directory, if not copy to clipboard and confirm with a message
			if (strstr(value_at(vault->entry, item_index(current_item(menu))), "/") != NULL)
				break;
			if (copy_to_clipboard(value_at(vault->api_entry, item_index(current_item(menu))))){
                print_message(windows, "Password copied to clipboard for 45 seconds");
            } else {
                print_message(windows, "Password is not copied to clipboard");
            }
			break;

		// delete an entry
		case 'd':
			// Text is different whether it's a directory or not
			prompt = "Sure you want to delete the entry?\nYou cannot undo that (y/n)";
			if (strstr(value_at(vault->entry, item_index(current_item(menu))), "/") != NULL)
				prompt = "Sure you want to delete the directory?\nDeletes all entries inside this directory.\nYou cannot undo that (y/n)";

			// start confirmation prompt and execute command, if confirmed
			if (centered_confirm_prompt(menu, windows, CONFIRM_PROMPT_WINDOW, prompt, BORDER_COLOR, true)) {
				delete_entry(value_at(vault->api_entry, item_index(current_item(menu))));
				return item_index(current_item(menu));
			}
			break;

		// edit a password using nano
		case 'e':
			if (strstr(value_at(vault->entry, item_index(current_item(menu))), "/") != NULL)
				break;
			endwin();
			edit_password(value_at(vault->api_entry, item_index(current_item(menu))));
			initscr();
			return item_index(current_item(menu));

		// reload list
		case 'R':
			return item_index(current_item(menu));

		// rename an entry
		case 'r':
			push_back(output, rename_form(windows, menu, value_at(vault->api_entry, item_index(current_item(menu)))));
			if (*(char*)value_at(output, 0) == '/') {
                erase_list(output);
				print_message(windows, "New name mustn't end with '/'");
				break;
			}
			if (*(char*)value_at(output, 0) == '\0') 
				break;
			rename_password(value_at(vault->api_entry, item_index(current_item(menu))), value_at(output, 0));
            erase_list(output);
			return item_index(current_item(menu));

		// show help message
		case '?':
			centered_confirm_prompt(menu, windows, CONFIRM_PROMPT_WINDOW, help_msg, -1, false);
			break;
		}

		// reset 'g' input
		if (c != 'g')
			g = false;


        // if a window is overlapping with the menu, the items are not printed again, this trick enforces this
        if (0 == item_index(current_item(menu))) {
            menu_driver(menu, REQ_DOWN_ITEM);
            menu_driver(menu, REQ_UP_ITEM);
        } else {
            menu_driver(menu, REQ_UP_ITEM);
            menu_driver(menu, REQ_DOWN_ITEM);
        }

		// if the cursor is on a directory, change the color
		set_color(menu);

		// refresh window
		wrefresh(windows[MENU_WINDOW]);
		wrefresh(windows[PATTERN_WINDOW]);
		refresh();
	}

	return -1;
}
