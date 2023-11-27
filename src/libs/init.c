#include "init.h"
#include "shell-scripts.h"

void load_vault_path(char *path) {
	const char *vault_name = "vault-dir.txt";
	// try opening file
	FILE *dir_ptr = fopen(vault_name, "r");

	if (dir_ptr == NULL) {
        // save file doesn't exist

		// set up a string for default path
		path = getenv("HOME");
		strcat(path, "/.password-store/");


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
		}
		else if (ENOENT == errno) {
			// TODO: read for alternative path
			printf("give me location of your password-store");
		}
		else {
            // error
			fprintf(stderr, "Could not check for directory");
            exit(-1);
		}
	}
	else {
        // save file exists, try to read it
		if (fgets(path, max_path_size, dir_ptr) == NULL) {
			fclose(dir_ptr);
			fprintf(stderr, "Couldn't read file");
			exit(-1);
		}

		fclose(dir_ptr);
	}
}

char **get_all_entries(struct vault *vault) {
	// open a pipe to the command
	FILE *pipe = findscript(vault->vault_dir);

	// allocate entries array
	char **directories = malloc(vault->count * sizeof(char *));
	if (directories == NULL) {
		fprintf(stderr, "malloc failed");
		exit(-1);
	}

    // allocates memory for every entry and tries to read the pipe,
    // storing the file into the array
	for (size_t i = 0; i < vault->count; i++) {
		directories[i] = malloc(max_path_size);
		if (directories[i] == NULL) {
			fprintf(stderr, "malloc failed");
			exit(-1);
		}

		if (fgets(directories[i], max_path_size, pipe) == NULL) {
			fprintf(stderr, "couldn't read all passwords");
			exit(-1);
		}
	}

	// close the pipe
	if (pclose(pipe) == -1) {
		perror("pclose failed");
		exit(-1);
	}

	return directories;
}
