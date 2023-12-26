#include "display.h"

#define XPADDING 10

void create_pop_up(WINDOW *windows[], linked_list *information, char *entry) {
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
	if (size(information) == 1) {
        // if not, make the width dependent of password size and add a padding
		width = strlen(value_at(information, 0)) + XPADDING;

        // set important coordinates
		height_border = 6;
		y -= 4;
		y_border = y + 2;
	}
	else {
        // if yes, check which string is bigger and make the width the bigger size + padding
		width = (strlen(value_at(information, 0)) > strlen(value_at(information, 1))) ? strlen(value_at(information, 0)) + XPADDING : strlen(value_at(information, 1)) + XPADDING;

        // set important coordinates
		height_border = 10;
		y -= 2;
		y_border = y - 2;
	}

    // set the rest of the coordinates
	x -= width / 2;
	width_border = (strlen(entry) > width + 4) ? strlen(entry) + 8 : width + 8;
	x_border -= width_border / 2;

    // print the three boxes
	print_box_with_title(windows, BORDER_WINDOW, y_border, x_border, width_border, height_border, entry, "", false, BORDER_COLOR);
	if (size(information) > 1)
		print_box_with_title(windows, USERNAME_WINDOW, y, x, width, height, "Username:", value_at(information, 1), false, -1);
	print_box_with_title(windows, PASSWORD_WINDOW, y + 4, x, width, height, "Password:", value_at(information, 0), false, -1);
}

void username_password_display(WINDOW *windows[], linked_list *information, char *entry, MENU *menu) {
	// input char
	int c;

    // create boxes
    create_pop_up(windows, information, entry);

	while ((c = wgetch(windows[MENU_WINDOW])) != 'q') {
		switch (c) {
		case KEY_RESIZE:
            // delete windows
            remove_window_index(windows, BORDER_WINDOW);
            remove_window_index(windows, PASSWORD_WINDOW);
            if (windows[USERNAME_WINDOW])
                remove_window_index(windows, USERNAME_WINDOW);

			// resize menu and preserve current item
			set_size_menu_window(menu, windows, current_item(menu));

            // create the boxes again, with new coordinates
            create_pop_up(windows, information, entry);
            break;
		}
	}

    // remove all windows and refresh
	remove_window_index(windows, PASSWORD_WINDOW);
	remove_window_index(windows, BORDER_WINDOW);
	if (windows[USERNAME_WINDOW])
		remove_window_index(windows, USERNAME_WINDOW);
}
