#include "pass_api.h"

void get_pass_information(std::vector<std::string> *information, std::string argument) {
	// open a pipe to the command and count the number of entries
	FILE *pipe = get_password_script("'" + argument + "'");

	char buffer[max_path_size];

	// tries to read the pipe, storing the filename into the vector
    for (int i = 0; i < 2; i++) {
        if (fgets(buffer, max_path_size, pipe) == NULL) break;

        *(buffer + strlen(buffer) - 1) = '\0';
        information->push_back(buffer);
    }

    // removes the "login: " substring if present
    if(information->size() > 1 && information->at(1).find("login: ") != std::string::npos) {
        std::string new_login("");
        for (auto i = information->at(1).begin() + 7; i <= information->at(1).end(); i++) {
            new_login += *i;
        }
        information->at(1) = new_login;
    }

	// close the pipe
	if (pclose(pipe) == -1) {
		perror("pclose failed\n");
		exit(-1);
	}
}

bool copy_to_clipboard(std::string argument) {
    // usage of system without shell-script.c because of simple shell command, return is not needed
    std::string command = "pass --clip '" + argument + "' > /dev/null 2>&1";
    return system(command.c_str()) > 0;
}

bool delete_entry(std::string argument) {
    // usage of system without shell-script.c because of simple shell command, return is not needed
    std::string command = "pass rm -f -r '" + argument + "' > /dev/null 2>&1";
    return system(command.c_str()) > 0;
}

bool edit_password(std::string argument) {
    // usage of system without shell-script.c because of simple shell command, return is not needed
    std::string command = "pass edit '" + argument + "'";
    return system(command.c_str()) > 0;
}

bool rename_password(std::string argument1, std::string argument2) {
    std::string command = "pass mv '" + argument1 + "' '" + argument2 + "' > /dev/null 2>&1";
    return system(command.c_str()) > 0;
}
