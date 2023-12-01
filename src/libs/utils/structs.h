#include <stdlib.h>
#include <string>
#include <vector>

struct vault {
	size_t count_entries;                 // Number entries
    std::string vault_dir;                // Directory of the stored vaults
    std::vector<std::string> api_entry;   // Array of api version of entry, is synced with entry
    std::vector<std::string> entry;       // Array of all entries
};

