#include "init-ui.h"

void init_ui(struct vault *vault) {
	ITEM **items;
	MENU *menu;
	WINDOW *menu_win;

	// initialize
	initscr();

	// start color mode and preserve standard background
	start_color();
	use_default_colors();


	cbreak();
	keypad(stdscr, TRUE);

	// don't print inputs and make the cursor invisible
	noecho();
	curs_set(0);

	// initialize colors
	init_pair(1, COLOR_RED, -1);
	init_pair(2, COLOR_BLUE, -1);

	// set delay after escape to 0
	set_escdelay(0);

	// initializes items
	items = (ITEM **)malloc((vault->count_entries + 1) * sizeof(ITEM *));
	if (items == NULL) {
		fprintf(stderr, "could not malloc items\n");
		exit(-1);
	}

	// create items
	for (size_t i = 0; i < vault->count_entries; i++) {
		if (i < vault->count_entries - 1 && (vault->api_entry[i + 1]).find(vault->api_entry[i]) != std::string::npos) {
			// if this item is a directory
			vault->entry[i] += "/";
		}
		items[i] = new_item(vault->entry[i].c_str(), NULL);
	}

	// create menu with created items
	menu = new_menu((ITEM **)items);

	// create menu window
	menu_win = newwin(LINES - 1, COLS - 1, 0, 0);
	keypad(menu_win, TRUE);

	// set main and sub window
	set_menu_win(menu, menu_win);
	set_menu_sub(menu, derwin(menu_win, LINES - 3, COLS - 2, 1, 1));
	set_menu_format(menu, LINES - 3, 1);


	// print a border and set mark_length
	box(menu_win, 0, 0);
	set_menu_mark(menu, "   ");

	// post menu
	post_menu(menu);
	set_mark_fore(menu);
	refresh();

	wrefresh(menu_win);
	refresh();

	// navigation
	navigation(menu, menu_win, vault);

	// unpost and free all the memory taken up
	unpost_menu(menu);
	free_menu(menu);
	for (size_t i = 0; i < vault->count_entries; i++)
		free_item(items[i]);
	endwin();
}
