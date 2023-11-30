#include "init-ui.h"
#include <algorithm>
#include <curses.h>
#include <iostream>
#include <menu.h>
#include <sstream>
#include <vector>

void init_ui(struct vault *vault) {
	ITEM **items;
	int c;
	MENU *menu;
	WINDOW *menu_win;

	// initialize
	initscr();
	start_color();
	use_default_colors();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, -1);
	init_pair(2, COLOR_CYAN, -1);


	// create items and menu
	items = (ITEM **)malloc((vault->count_entries + 1 + vault->count_directories) * sizeof(ITEM *));
	for (size_t i = 0; i < vault->count_entries + vault->count_directories; i++) {
		if (i < vault->count_entries + vault->count_directories - 1 && (vault->api_entry[i + 1]).find(vault->api_entry[i]) != std::string::npos) {
            // if directory
			// item_opts_off(items[i], O_SELECTABLE);
            vault->entry[i] += "/";
		} else {

        }
        items[i] = new_item(vault->entry[i].c_str(), NULL);
	}
	menu = new_menu((ITEM **)items);
    set_menu_fore(menu, COLOR_PAIR(1));

	// create menu window
	menu_win = newwin(LINES, COLS, 0, 0);
	keypad(menu_win, TRUE);

	// set main and sub window
	set_menu_win(menu, menu_win);
	set_menu_sub(menu, derwin(menu_win, LINES - 2, COLS - 2, 1, 1));
	set_menu_format(menu, LINES - 2, 1);


	// print a border and set mark
	box(menu_win, 0, 0);
	set_menu_mark(menu, " * ");
	refresh();

	// post menu
	post_menu(menu);
	wrefresh(menu_win);

	refresh();

	// navigation (own file)
	bool g = false;
	while ((c = wgetch(menu_win)) != 'q') {
		switch (c) {
		case KEY_DOWN:
		case 'j':
			menu_driver(menu, REQ_DOWN_ITEM);
			break;
		case KEY_UP:
		case 'k':
			menu_driver(menu, REQ_UP_ITEM);
			break;
		case KEY_NPAGE:
			menu_driver(menu, REQ_SCR_DPAGE);
			break;
		case KEY_PPAGE:
			menu_driver(menu, REQ_SCR_UPAGE);
			break;
		case 'G':
			menu_driver(menu, REQ_LAST_ITEM);
			break;
		case 'g':
			if (g) {
				menu_driver(menu, REQ_FIRST_ITEM);
				g = false;
			}
			else {
				g = true;
			}
			break;
        case 'p':
            set_menu_pattern(menu, "");
            break;
        case ':':
            // char p = getch();
            set_menu_pattern(menu, "gra");
            break;
		}
		if (c != 'g')
			g = false;

        ITEM *cur_item = current_item(menu);
        std::string cur_item_name(item_name(cur_item));
        if (cur_item_name.find('/') != std::string::npos) {
            set_menu_fore(menu, COLOR_PAIR(2));
        } else {
            set_menu_fore(menu, COLOR_PAIR(1));
        }
		wrefresh(menu_win);
	}

	/* Unpost and free all the memory taken up */
	unpost_menu(menu);
	free_menu(menu);
	for (size_t i = 0; i < vault->count_entries; i++)
		free_item(items[i]);
	endwin();
}
