#include "../utils/shell-scripts.h"

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <string.h>
#include <vector>

// Returns the path to the password vault
std::string load_vault_path();

// Saves password entries from the vault into struct
std::vector<std::string> get_all_entries(std::string vault_dir, size_t count, bool api);
