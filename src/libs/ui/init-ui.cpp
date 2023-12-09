#include "init-ui.h"
#include <menu.h>

int init_ui(struct vault *vault, int index) {
	ITEM **items;
	MENU *menu;
	std::vector<WINDOW *> windows(20, NULL); //CHANGE THAT

	// initializes items
	items = (ITEM **)malloc((vault->count_entries + 1) * sizeof(ITEM *));
	if (items == NULL) {
		fprintf(stderr, "could not malloc items\n");
		exit(-1);
	}

	// create items
	for (size_t i = 0; i < vault->count_entries; i++) {
		if (i < vault->count_entries - 1 && vault->api_entry[i + 1].find(vault->api_entry[i]) != std::string::npos && (vault->api_entry[i + 1].find('/') != std::string::npos)) {
			// if this item is a directory
			vault->entry[i] += "/";
		}
		items[i] = new_item(vault->entry[i].c_str(), NULL);
	}

	// set last entry to NULL, to indicate the end of the menus
	items[vault->count_entries] = NULL;

	// create menu with created items
	menu = new_menu((ITEM **)items);

	// sets border and size of windows
	set_size_menu_window(menu, &windows, NULL);
	windows.at(PATTERN_WINDOW) = newwin(1, COLS, LINES - 1, 0);

	// goes 'back' to current item
	set_current_item(menu, items[index]);

	// sets color and mark of current item
	set_color(menu);
	refresh();

	// navigation
	int result = navigation(menu, &windows, vault);

	// unpost and free all the memory taken up
	unpost_menu(menu);
	free_menu(menu);
	for (size_t i = 0; i < vault->count_entries; i++)
		free_item(items[i]);
	free(items);

	delwin(windows.at(MENU_WINDOW));
	delwin(windows.at(PATTERN_WINDOW));
	delwin(windows.at(MENU_SUB_WINDOW));

	return result;
}
