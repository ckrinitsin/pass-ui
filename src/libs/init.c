#include "init.h"
#include "shell-scripts.h"

void load_vault_path(char *path) {
	const char *vault_name = "vault-dir.txt";
	// try opening file
	FILE *dir_ptr = fopen(vault_name, "r");

	if (dir_ptr == NULL) {
		// set up a string for default path
		path = getenv("HOME");
		strcat(path, "/.password-store/");

		// check if that default path exists
		DIR *dir = opendir(path);
		if (dir) {
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
			fprintf(stderr, "Could not check for directory");
		}
	}
	else {
		if (fgets(path, max_path_size, dir_ptr) == NULL) {
			fclose(dir_ptr);
			fprintf(stderr, "Couldn't read file");
			exit(-1);
		}

		fclose(dir_ptr);
	}
}

int heyo(struct vault *vault) {
    size_t number_files = get_number_files(vault->vault_dir);
	// open a pipe to the command
	FILE *pipe = findscript(vault->vault_dir);

	// read the output of the command into a string
	char buffer[max_path_size];
	char *directories[number_files];

    for (size_t i = 0; i < number_files; i++) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL) break;

        directories[i] = strdup(buffer);
    }

	// close the pipe
	if (pclose(pipe) == -1) {
		perror("pclose failed");
		return 1;
	}

	for (size_t i = 0; i < number_files; i++) {
		printf("Directory %zu: %s\n", i + 1, directories[i]);
		free(directories[i]);
	}

	return 0;
}
