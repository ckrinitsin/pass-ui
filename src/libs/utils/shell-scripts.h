#include "string-helper.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Defines the max length of a directory name
#define max_path_size 512

FILE *get_command_pipe(char *arg);

FILE *findscript(char *arg);

FILE *findscript_api(char *arg);

size_t get_number_entries(char *vault_dir);

FILE *countscript(char *arg);

FILE *get_password_script(char *arg);
