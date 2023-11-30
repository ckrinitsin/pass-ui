#include <dirent.h>
#include <errno.h>
#include <memory>
#include <string>
#include <stdio.h>
#include "../utils/shell-scripts.h"
#include "../utils/structs.h"

// Defines the max length of a directory name
#define max_path_size 512

// Returns the path to the password vault
std::string load_vault_path();

// Saves password entries from the vault into struct
std::vector<std::string> get_all_entries(std::string vault_dir, size_t count, bool api);
