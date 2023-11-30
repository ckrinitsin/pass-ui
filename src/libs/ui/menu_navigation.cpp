#include "menu_navigation.h"
#include <menu.h>

void navigation(MENU* menu, WINDOW* menu_win) {
    int c;
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
        case 'n':
            if (menu_pattern(menu)) 
				menu_driver(menu, REQ_NEXT_MATCH);
            break;
        case 'N':
            if (menu_pattern(menu)) 
				menu_driver(menu, REQ_PREV_MATCH);
            break;
        case '/':
            start_pattern_matching(menu, menu_win);
            break;
		}

        // reset 'g' input
		if (c != 'g')
			g = false;

        // if the cursor is on a directory, change the color and remove the mark
        ITEM *cur_item = current_item(menu);
        std::string cur_item_name(item_name(cur_item));
        if (cur_item_name.find('/') != std::string::npos) {
	        set_menu_mark(menu, "   ");
            set_menu_fore(menu, COLOR_PAIR(2));
        } else {
	        set_menu_mark(menu, " * ");
            set_menu_fore(menu, COLOR_PAIR(1));
        }

		wrefresh(menu_win);
        refresh();
	}
}
