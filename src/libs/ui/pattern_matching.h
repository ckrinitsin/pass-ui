#include "../utils/structs.h"

#include <menu.h>
#include <ncurses.h>
#include <string>
#include <vector>

std::vector<ITEM *> start_pattern_matching(MENU *menu, WINDOW *menu_win, std::vector<std::string> real_paths);

std::vector<ITEM *> search_menu_pattern(MENU *menu, std::vector<std::string> pattern_matching_buffer, std::vector<std::string> real_paths);

void set_mark_fore(MENU *menu);
