#include "init.h"

int init() {
	int result = 0;

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
	init_color_pairs();

	// set delay after escape to 0
	set_escdelay(0);

	// while (result >= 0) {

		// Loads the password-store path
		char *vault_dir = load_vault_path();

		size_t count = get_number_entries(vault_dir);

		// Gets a string array, with all entries
		linked_list *entries = get_all_entries(vault_dir, count, false);
		linked_list *entries_api = get_all_entries(vault_dir, count, true);

		struct vault vault = {.count_entries = count, .vault_dir = vault_dir, .api_entry = entries_api, .entry = entries};
		result = init_ui(&vault, result);
        erase_heap(entries);
        erase_heap(entries_api);
        free(vault_dir);
	// }

	// endwin();
	return 0;
}

char *load_vault_path() {
	const char *vault_name = "vault-dir.txt";
	// try opening file
	FILE *dir_ptr = fopen(vault_name, "r");

	if (dir_ptr == NULL) {
		// save file doesn't exist

		// set up a string for default path
        char *path = concat_strings(2, getenv("HOME"), "/.password-store/");


		DIR *dir = opendir(path);
		if (dir) {
			// directory exists
			closedir(dir);

			// create file if doesn't exist
			dir_ptr = fopen(vault_name, "w");

			// write default path in file
			if (fputs(path, dir_ptr) < 0) {
				fprintf(stderr, "Could not write txt file");
			}

			fclose(dir_ptr);
            return path;
		}
		else if (ENOENT == errno) {
			// TODO: read for alternative path
			printf("give me location of your password-store");
            exit(-1);
		}
		else {
			// error
			fprintf(stderr, "Could not check for directory");
			exit(-1);
		}
	}
	else {
        char buffer[max_path_size];
		// save file exists, try to read it
		if (fgets(buffer, max_path_size, dir_ptr) == NULL) {
			fclose(dir_ptr);
			fprintf(stderr, "Couldn't read file");
			exit(-1);
		}
		fclose(dir_ptr);
        return allocate_string(buffer);
	}
}

linked_list *get_all_entries(char *vault_dir, size_t count, bool api) {
	// open a pipe to the command and count the number of entries
	FILE *pipe;
	if (api) {
		pipe = findscript_api(vault_dir);
	}
	else {
		pipe = findscript(vault_dir);
	}

	// allocate entries vector and declare a buffer
	linked_list *entries = NULL;
	char buffer[max_path_size];

	// tries to read the pipe, storing the filename into the vector
	for (size_t i = 0; i < count; i++) {
		if (fgets(buffer, max_path_size, pipe) == NULL) {
			fprintf(stderr, "Couldn't read all passwords\n");
			exit(-1);
		}

		*(buffer + strlen(buffer) - 1) = '\0';
        entries = push_back(entries, allocate_string(buffer));
	}

	// close the pipe
	if (pclose(pipe) == -1) {
		perror("pclose failed\n");
		exit(-1);
	}

	return entries;
}
