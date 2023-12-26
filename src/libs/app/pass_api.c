#include "pass_api.h"

linked_list *get_pass_information(linked_list *information, char *argument) {
	// open a pipe to the command and count the number of entries
	FILE *pipe = get_password_script(argument);

	char buffer[max_path_size];

	// tries to read the pipe, storing the filename into the vector
    if (fgets(buffer, max_path_size, pipe) == NULL) return NULL;

    *(buffer + strlen(buffer) - 1) = '\0';
    information = push_back(information, allocate_string(buffer));

    if (fgets(buffer, max_path_size, pipe) == NULL) return NULL;

    *(buffer + strlen(buffer) - 1) = '\0';
    // removes the "login: " substring if present
    if (strstr(buffer, "login: ") != NULL) {
        char new_login[max_path_size];
        for (int i = 7; i <= strlen(buffer); i++) {
            new_login[i-7] = buffer[i];
        }
        information = push_back(information, allocate_string(new_login));
    }

	// close the pipe
	if (pclose(pipe) == -1) {
		perror("pclose failed\n");
		exit(-1);
	}

    return information;
}

bool copy_to_clipboard(char *argument) {
    // usage of system without shell-script.c because of simple shell command, return is not needed
    char *command = concat_strings(3, "pass --clip '", argument, "' 2>&1 | grep -q -v 'gpg: ' && echo 1");

    FILE *pipe = get_command_pipe(command);

    char buffer[max_path_size];
    if (fgets(buffer, max_path_size, pipe) == NULL) return false;

	// close the pipe
	if (pclose(pipe) == -1) {
		perror("pclose failed\n");
		exit(-1);
	}

    free(command);
    if (*buffer == '1') return true;

    return false;
}

bool delete_entry(char *argument) {
    // usage of system without shell-script.c because of simple shell command, return is not needed
    char *command = concat_strings(3, "pass rm -f -r '", argument, "' > /dev/null 2>&1");
    bool res = system(command) > 0;
    free(command);
    return res;
}

bool edit_password(char *argument) {
    // usage of system without shell-script.c because of simple shell command, return is not needed
    char *command = concat_strings(3, "pass edit '", argument, "'");
    bool res = system(command) > 0;
    free(command);
    return res;
}

bool rename_password(char *argument1, char *argument2) {
    char *command = concat_strings(5, "pass mv '", argument1, "' '", argument2, "' > /dev/null 2>&1");
    bool res = system(command) > 0;
    free(command);
    return res;
}
