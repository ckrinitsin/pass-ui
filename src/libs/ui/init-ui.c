#include "init-ui.h"

int init_ui(struct vault *vault, int index) {
	ITEM **items;
	MENU *menu;

    WINDOW **windows;
    windows = malloc(15 * sizeof(WINDOW*));

	// initializes items
	items = (ITEM **)malloc((vault->count_entries + 1) * sizeof(ITEM *));
	if (items == NULL) {
		fprintf(stderr, "could not malloc items\n");
		exit(-1);
	}

	// create items
	for (size_t i = 0; i < vault->count_entries; i++) {
		if (i < vault->count_entries - 1 && strstr(value_at(vault->api_entry, i + 1), value_at(vault->api_entry, i)) != NULL 
                && strstr(value_at(vault->api_entry, i + 1), "/") != NULL) {
			// if this item is a directory, add a '/' to the entry name
            char *tmp = concat_strings(2, value_at(vault->entry, i), "/");
            free(value_at(vault->entry, i));
            node_at(vault->entry, i)->value = tmp;
		}
		items[i] = new_item(value_at(vault->entry, i), NULL);
	}

	// set last entry to NULL, to indicate the end of the menus
	items[vault->count_entries] = NULL;

	// create menu with created items
	menu = new_menu((ITEM **)items);

	// sets border and size of windows
	set_size_menu_window(menu, windows, NULL);
    windows[PATTERN_WINDOW] = newwin(1, COLS, LINES - 1, 0);

	// goes 'back' to current item
	set_current_item(menu, items[index]);

	// sets color and mark of current item
	set_color(menu);
	refresh();

	// navigation
	int result = navigation(menu, windows, vault);

	// unpost and free all the memory taken up
	unpost_menu(menu);
	free_menu(menu);
	for (size_t i = 0; i < vault->count_entries; i++)
		free_item(items[i]);
	free(items);

	delwin(windows[MENU_WINDOW]);
	delwin(windows[PATTERN_WINDOW]);
	delwin(windows[MENU_SUB_WINDOW]);
    free(windows);

	return result;
}
