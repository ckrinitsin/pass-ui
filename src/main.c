#include "libs/init.h"
#include "libs/shell-scripts.h"
#include <stdlib.h>

int main() {
    // Test main function to see the flow of program
    char* vault_dir = malloc(max_path_size);
    if (vault_dir == NULL) {
        fprintf(stderr, "malloc fail");
    }

    // Loads the password-store path
    load_vault_path(vault_dir);

    // Initializes a struct with number entries and directory to the vault
    struct vault vault = { .count = get_number_files(vault_dir),  .vault_dir = vault_dir };

    // Gets a string array, with all entries
    char **entries = get_all_entries(&vault);

    // Prints entries
    for (size_t i = 0; i < vault.count; i++) {
        printf("Directory %zu: %s\n", i+1, entries[i]);
    }

    // Frees everything
    for (size_t i = 0; i < vault.count; i++) {
        free(entries[i]);
    }
    free(entries);
    free(vault_dir);

    return 0;
}
