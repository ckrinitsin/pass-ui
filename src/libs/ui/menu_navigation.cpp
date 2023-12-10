#include "menu_navigation.h"

std::string help_msg = "e               edit your password using nano\n"
					   "c               copy your password in clipboard\n"
					   "R               refresh the list\n"
					   "r               rename the password or directory\n"
					   "d               delete the password or the directory\n"
					   "ENTER           show your password information";


int navigation(MENU *menu, std::vector<WINDOW *> *windows, struct vault *vault) {
	// vector which stores all items, which are matched with the pattern, index for navigating through them
	std::vector<ITEM *> pattern_vector;
	size_t pattern_index = 0;

	// saves the output of various commands
	std::vector<std::string> output;

	// input char
	int c;

	// checker for double g
	bool g = false;

	std::string prompt;

	while ((c = wgetch(windows->at(MENU_WINDOW))) != 'q') {
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

		// show entry information
		case '\n':
			// remove message at bottom bar
			delete_message(windows);

			// if the item is not a directory, but a password file, get the information into a vector
			if (vault->entry.at(item_index(current_item(menu))).find("/") != std::string::npos)
				break;
			get_pass_information(&output, vault->api_entry.at(item_index(current_item(menu))));

            if (output.size() == 0) break;

			// create a username_password pop up
			username_password_display(windows, output, vault->api_entry.at(item_index(current_item(menu))), menu);

			// clear the vector
			output.clear();
			break;

		// copy password into clipboard
		case 'c':
			// check if item is directory, if not copy to clipboard and confirm with a message
			if (vault->entry.at(item_index(current_item(menu))).find("/") != std::string::npos)
				break;
			if (copy_to_clipboard(vault->api_entry.at(item_index(current_item(menu))))){
                print_message(windows, "Password copied to clipboard for 45 seconds");
            } else {
                print_message(windows, "Password is not copied to clipboard");
            }
			break;

		// delete an entry
		case 'd':
			// Text is different whether it's a directory or not
			prompt = "Sure you want to delete the entry?\nYou cannot undo that (y/n)";
			if (vault->entry.at(item_index(current_item(menu))).find("/") != std::string::npos)
				prompt = "Sure you want to delete the directory?\nDeletes all entries inside this directory.\nYou cannot undo that (y/n)";

			// start confirmation prompt and execute command, if confirmed
			if (centered_confirm_prompt(menu, windows, CONFIRM_PROMPT_WINDOW, prompt, BORDER_COLOR, true)) {
				delete_entry(vault->api_entry.at(item_index(current_item(menu))));
				return item_index(current_item(menu));
			}
			break;

		// edit a password using nano
		case 'e':
			if (vault->entry.at(item_index(current_item(menu))).find("/") != std::string::npos)
				break;
			endwin();
			edit_password(vault->api_entry.at(item_index(current_item(menu))));
			initscr();
			return item_index(current_item(menu));

		// reload list
		case 'R':
			return item_index(current_item(menu));

		// rename an entry
		case 'r':
			output.push_back(rename(windows, menu, vault->api_entry.at(item_index(current_item(menu)))));
			if (output.at(0) == "/") {
				output.clear();
				print_message(windows, "New name mustn't end with '/'");
				break;
			}
			if (output.at(0) == "")
				break;
			rename_password(vault->api_entry.at(item_index(current_item(menu))), output[0]);
			output.clear();
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
		wrefresh(windows->at(MENU_WINDOW));
		wrefresh(windows->at(PATTERN_WINDOW));
		refresh();
	}

	return -1;
}
