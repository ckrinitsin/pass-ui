#include "pop_ups.h"

#define XPADDING 10

void create_pop_up(std::vector<WINDOW *> *windows, std::vector<std::string> information, std::string entry) {
	// coordinates for username/password window(s)
	size_t y = LINES / 2;
	size_t x = COLS / 2;
	size_t width;
	size_t height = 3;

	// coordinates for border window
	size_t y_border = LINES / 2;
	size_t x_border = COLS / 2;
	size_t width_border;
	size_t height_border;

    // check if we have given a username
	if (information.size() == 1) {
        // if not, make the width dependent of password size and add a padding
		width = information.at(0).size() + XPADDING;

        // set important coordinates
		height_border = 6;
		y -= 4;
		y_border = y + 2;
	}
	else {
        // if yes, check which string is bigger and make the width the bigger size + padding
		width = (information.at(0).size() > information.at(1).size()) ? information.at(0).size() + XPADDING : information.at(1).size() + XPADDING;

        // set important coordinates
		height_border = 10;
		y -= 2;
		y_border = y - 2;
	}

    // set the rest of the coordinates
	x -= width / 2;
	width_border = (entry.size() > width + 4) ? entry.size() + 8 : width + 8;
	x_border -= width_border / 2;

    // print the three boxes
	print_box_with_title(windows, BORDER_WINDOW, y_border, x_border, width_border, height_border, entry, "", BORDER_COLOR);
	if (information.size() > 1)
		print_box_with_title(windows, USERNAME_WINDOW, y, x, width, height, "Username:", information.at(1), -1);
	print_box_with_title(windows, PASSWORD_WINDOW, y + 4, x, width, height, "Password:", information.at(0), -1);
}

void username_password_pop_up(std::vector<WINDOW *> *windows, std::vector<std::string> information, std::string entry, MENU *menu) {
	// input char
	int c;

    // create boxes
    create_pop_up(windows, information, entry);

	while ((c = wgetch(windows->at(MENU_WINDOW))) != 'q') {
		switch (c) {
		case KEY_RESIZE:
            // delete windows
            remove_window(windows, BORDER_WINDOW);
            remove_window(windows, PASSWORD_WINDOW);
            if (windows->at(USERNAME_WINDOW))
                remove_window(windows, USERNAME_WINDOW);

			// resize menu and preserve current item
			set_size_menu_window(menu, windows, current_item(menu));

            // create the boxes again, with new coordinates
            create_pop_up(windows, information, entry);
            break;
		}
	}

    // remove all windows and refresh
	remove_window(windows, PASSWORD_WINDOW);
	remove_window(windows, BORDER_WINDOW);
	if (windows->at(USERNAME_WINDOW))
		remove_window(windows, USERNAME_WINDOW);

    wrefresh(windows->at(MENU_WINDOW));
    wrefresh(windows->at(PATTERN_WINDOW));
    refresh();
}
