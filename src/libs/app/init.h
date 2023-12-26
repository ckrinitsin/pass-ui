#include "../utils/shell-scripts.h"
#include "../ui/init-ui.h"

#include <string.h>
#include <errno.h>
#include <dirent.h>

int init();

// Returns the path to the password vault
char *load_vault_path();

// Saves password entries from the vault into struct
linked_list *get_all_entries(char *vault_dir, size_t count, bool api);
