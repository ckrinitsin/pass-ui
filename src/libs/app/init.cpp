#include "init.h"
#include <iostream>

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

    while (result >= 0) {

        // Loads the password-store path
        std::string vault_dir = load_vault_path();

        size_t count = get_number_entries(vault_dir);

        // Gets a string array, with all entries
        std::vector<std::string> entries = get_all_entries(vault_dir, count, false);
        std::vector<std::string> entries_api = get_all_entries(vault_dir, count, true);

        struct vault vault = {.count_entries = count, .vault_dir = vault_dir, .api_entry = entries_api, .entry = entries};
        result = init_ui(&vault, result);
    }

	endwin();
	return 0;
}

std::string load_vault_path() {
	std::string vault_name{"vault-dir.txt"};

	if (!std::filesystem::exists((std::filesystem::path)vault_name)) {
		// save file doesn't exist

		// set up a string for default path
		std::string env = std::getenv("HOME");
		std::string dir = "/.password-store/";

		if (std::filesystem::exists((std::filesystem::path)(env + dir))) {
			// create save file
			std::ofstream save_file;
			save_file.open(vault_name);

			// write default path in file and close
			save_file << env + dir;
			save_file.close();
			return env + dir;
		}
		else {
			// TODO: read for alternative path
			printf("Give me location of your password-store\n");
			exit(1);
		}
	}
	else {
		// save file exists, try to read it
		std::ifstream save_file;
		save_file.open(vault_name);

		// read file into char* and convert to string
		std::string result((std::istreambuf_iterator<char>(save_file)), std::istreambuf_iterator<char>());

		// close file and return string
		save_file.close();
		return result;
	}
}

std::vector<std::string> get_all_entries(std::string vault_dir, size_t count, bool api) {
	// open a pipe to the command and count the number of entries
	FILE *pipe;
	if (api) {
		pipe = findscript_api(vault_dir);
	}
	else {
		pipe = findscript(vault_dir);
	}

	// allocate entries vector and declare a buffer
	std::vector<std::string> entries(count, "");
	char buffer[max_path_size];

	// tries to read the pipe, storing the filename into the vector
	for (size_t i = 0; i < count; i++) {
		if (fgets(buffer, max_path_size, pipe) == NULL) {
			fprintf(stderr, "Couldn't read all passwords\n");
			exit(-1);
		}

		*(buffer + strlen(buffer) - 1) = '\0';
		entries[i] = buffer;
	}

	// close the pipe
	if (pclose(pipe) == -1) {
		perror("pclose failed\n");
		exit(-1);
	}

	return entries;
}
