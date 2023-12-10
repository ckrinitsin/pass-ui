#include "shell-scripts.h"

#define max_number_digits 20

// Gets the whole shell script and opens a pipe for that
FILE *get_command_pipe(std::string arg) {
	FILE *pipe = popen(arg.c_str(), "r");
	if (pipe == NULL) {
		perror("popen failed\n");
		exit(-1);
	}
	return pipe;
}

// Util function for setting up a string which represents a shell script
// for getting all password entries
FILE *findscript(std::string arg) {
	std::string command = "cd '" + arg + "'\nfind . -type f ! -name '*.gpg' | tree -S | sed -s 's/.gpg//' | head -n -2 | tail -n +2 | cut -c 5-";
	return get_command_pipe(command);
}

FILE *findscript_api(std::string arg) {
	std::string command = "cd '" + arg + "'\nfind . -type f ! -name '*.gpg' | tree -i -f | sed -s 's/.gpg//' | head -n -2 | tail -n +2  | cut -c 3-";
	return get_command_pipe(command);
}

// Wrapper function to get the number of password entries
size_t get_number_entries(std::string vault_dir) {
	FILE *pipe = countscript(vault_dir);
	char output[max_number_digits];
	fgets(output, max_number_digits, pipe);
	size_t result = strtol(output, NULL, 10);
	pclose(pipe);
	return result;
}

// Util function for setting up a string which represents a shell script
// for getting the number password entries
FILE *countscript(std::string arg) {
	std::string command = "cd '" + arg + "'\nfind . -type f ! -name '*.gpg' | tree -i -f | head -n -2 | tail -n +2 | wc -l";
	return get_command_pipe(command);
}

FILE *get_password_script(std::string arg) {
    // this command does only print the output, if succeded (gpg doesn't give an error)
    std::string command = "pass '" + arg + "' 2>&1 | grep -v 'gpg: '";
    return get_command_pipe(command);
}
