#include "pattern_matching.h"
#include <curses.h>
#include <string>
#include <string.h>

void start_pattern_matching(MENU* menu, WINDOW* menu_win) {
    int c;
    char pattern_matching_buffer[256];
    pattern_matching_buffer[0] = '\0';
    for (int i = COLS-1; i >= 0; i--) {
        mvdelch(LINES-1, i);
    }

    move(LINES-1,0);
    addch('/');
    int x_pos = 1;

    while (true) {
        c = getch();
        switch(c) {
            case KEY_BACKSPACE:
                if (x_pos == 1) {
                    mvdelch(LINES-1, 0);
                    return;
                }
                pattern_matching_buffer[strlen(pattern_matching_buffer)-1] = '\0';
                mvdelch(LINES-1, --x_pos);
                break;
            case 27:
                for (int i = COLS-1; i >= 0; i--) {
                    mvdelch(LINES-1, i);
                }
                set_menu_pattern(menu, "");
                return;
            case '\n':
                set_menu_pattern(menu, pattern_matching_buffer);
                return;
            default:
                pattern_matching_buffer[strlen(pattern_matching_buffer)] = c;
                pattern_matching_buffer[strlen(pattern_matching_buffer)+1] = '\0';
                mvaddch(LINES-1, x_pos++, c);
                break;
        }
        set_menu_pattern(menu, pattern_matching_buffer);

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
