#include "form.h"
#include <string.h>

char *trim_whitespaces(char *str) {
	char *end;

	// trim leading space
	while (isspace(*str))
		str++;

	if (*str == 0) // all spaces?
		return str;

	// trim trailing space
	end = str + strnlen(str, 128) - 1;

	while (end > str && isspace(*end))
		end--;

	// write new null terminator
	*(end + 1) = '\0';

	return str;
}

char *rename_form(WINDOW *windows[], MENU* menu, char *value) {
    // make cursor visible
    curs_set(1);

    // declare the field, and properties of the field
	int width = (36 < strlen(value)) ? strlen(value)+4 : 40;
	int height = 1;
	FIELD *field[2] = { new_field(height, width, 0, 0, 0, 0), NULL };

    // remove the '/' from the string and set opts and buffer of the field
    if (value[strlen(value) - 1] == '/') value[strlen(value) - 1] = '\0';
    set_field_buffer(field[0], 0, value);
	set_field_opts(field[0], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);

    // update the width and height
	width += 2;
	height += 2;

    //  create the form out of the field and draw the box
	FORM *form = new_form(field);
	print_box_with_title(windows, TITLE_WINDOW, (LINES / 2) - (height / 2), (COLS / 2) - (width / 2), width, height, "Rename to:", "", false, -1);

    // set the windows and post form
	set_form_win(form, windows[TITLE_WINDOW]);
	set_form_sub(form, derwin(windows[TITLE_WINDOW], height - 2, width - 2, 1, 1));
    post_form(form);

    // put your cursor to the last char and refresh the windows
    form_driver(form, REQ_END_LINE);
    refresh();
    wrefresh(windows[TITLE_WINDOW]);

    int c;

    char *result;

    while ((c = wgetch(windows[MENU_WINDOW])) != 27) {
        switch(c) {
        case KEY_RESIZE:
            // move the input window and update everything else
            mvwin(windows[TITLE_WINDOW], (LINES / 2) - (height / 2), (COLS / 2) - (width / 2));
			set_size_menu_window(menu, windows, current_item(menu));
            wrefresh(windows[TITLE_WINDOW]);
            break;
        case '\n':
			// or the current field buffer won't be sync with what is displayed
			form_driver(form, REQ_NEXT_FIELD);
			form_driver(form, REQ_PREV_FIELD);
			move(LINES-3, 2);

            // get the (trimmed) result
            result = trim_whitespaces(field_buffer(field[0], 0));

            // reset everything to go back to menu navigation
            curs_set(0);
            unpost_form(form);
            free_form(form);
            free_field(field[0]);
            remove_window_index(windows, TITLE_WINDOW);

            // if the end char is a '/', the string is invalid
            if (result[strlen(result) - 1] == '/') 
                return "/";

            return result;

        // go one left
		case KEY_LEFT:
			form_driver(form, REQ_PREV_CHAR);
			break;

        // go one right
		case KEY_RIGHT:
			form_driver(form, REQ_NEXT_CHAR);
			break;

		// delete the char before cursor
		case KEY_BACKSPACE:
		case 127:
			form_driver(form, REQ_DEL_PREV);
			break;

		// delete the char under the cursor
		case KEY_DC:
			form_driver(form, REQ_DEL_CHAR);
			break;

        // 'print' the char c
		default:
			form_driver(form, c);
			break;
        }

        // refresh the window to display the change
        wrefresh(windows[TITLE_WINDOW]);
    }

    // reset everything to go back to menu navigation
    curs_set(0);
    unpost_form(form);
    free_form(form);
    free_field(field[0]);
    remove_window_index(windows, TITLE_WINDOW);
    return "";
}
