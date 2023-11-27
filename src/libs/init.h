#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "shell-scripts.c"

#define max_path_size 512

struct vault {
	size_t count;
	char *vault_dir;
	char *entry[];
};

// Returns the path to the password vault
void load_vault_path(char *path);

int heyo(struct vault *vault);
