#include "shell-scripts.h"

// Gets the whole shell script and opens a pipe for that
FILE *get_command_pipe(char *arg) {
	FILE *pipe = popen(arg, "r");
	if (pipe == NULL) {
		perror("popen failed");
		exit(-1);
	}
	return pipe;
}

// Util function for setting up a string which represents a shell script
// for getting all password entries
FILE *findscript(char *arg) {
	size_t arg_size = strlen(arg);

	char *result = malloc(100 + arg_size);
	if (result == NULL) {
		fprintf(stderr, "findscript fail\n");
        exit(-1);
	}
	result[0] = '\0';

	strcat(result, "cd ");
	strcat(result, arg);
	strcat(result, "\nfind . -type d -name '.git' -prune -o -name '*.gpg' -type f -print | sed 's/.gpg//' | cut -c 3-");
	FILE *pipe = get_command_pipe(result);
	free(result);
	return pipe;
}

// Wrapper function to get the number of password entries
size_t get_number_files(char *vault_dir) {
	size_t max_number_digits = 20;
	FILE *pipe = countscript(vault_dir);
	char output[max_number_digits];
	fgets(output, max_number_digits, pipe);
	size_t result = strtol(output, NULL, 10);
    pclose(pipe);
	return result;
}

// Util function for setting up a string which represents a shell script
// for getting the number password entries
FILE *countscript(char *arg) {
	size_t arg_size = strlen(arg);

	char *result = malloc(79 + arg_size);
	if (result == NULL) {
		fprintf(stderr, "findscript fail\n");
        exit(-1);
	}
	result[0] = '\0';

	strcat(result, "cd ");
	strcat(result, arg);
	strcat(result, "\nfind . -type d -name '.git' -prune -o -name '*.gpg' -type f -print | wc -l");
	FILE *pipe = get_command_pipe(result);
	free(result);
	return pipe;
}
