#include "libs/init.h"
#include "libs/shell-scripts.h"
#include <stdlib.h>

int main() {
    char* vault_dir = malloc(max_path_size);
    if (vault_dir == NULL) {
        fprintf(stderr, "alloc fail");
    }

    load_vault_path(vault_dir);

    struct vault vault = { .count = 0,  .vault_dir = vault_dir };

    heyo(&vault);

    free(vault_dir);

    return 0;
}
