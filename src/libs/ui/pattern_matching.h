#include "../utils/structs.h"
#include "design.h"

std::vector<ITEM *> start_pattern_matching(MENU *menu, std::vector<WINDOW*> *windows, std::vector<std::string> real_paths);

std::vector<ITEM *> search_menu_pattern(MENU *menu, std::vector<std::string> pattern_matching_buffer, std::vector<std::string> real_paths);

