#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defines the max length of a directory name
#define max_path_size 512

struct vault {
	size_t count;       // Number entries
	char *vault_dir;    // Directory of the stored vaults
	char **entry;       // Array of all entries
};

// Returns the path to the password vault
void load_vault_path(char *path);

// Returns a char array with all password entries from the vault
char **get_all_entries(struct vault *vault);
