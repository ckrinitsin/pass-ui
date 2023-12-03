#include <string>

// Defines the max length of a directory name
#define max_path_size 512

FILE *get_command_pipe(std::string arg);

FILE *findscript(std::string arg);

FILE *findscript_api(std::string arg);

size_t get_number_entries(std::string vault_dir);

FILE *countscript(std::string arg);

FILE *get_password_script(std::string arg);
