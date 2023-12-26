#include "string-helper.h"
#include <stdarg.h>
#include <string.h>

char *allocate_string(char *string) {
    // get a string from stack and allocate it on heap
	size_t length = strlen(string);
	char *result = malloc(length + 1);
	if (result == NULL) {
		fprintf(stderr, "could not malloc\n");
		exit(-1);
	}
	strcpy(result, string);
	return result;
}

char *concat_strings(int argc, ...) {
    // get the length of result string
    size_t length = 1;
    va_list args;
    va_start(args, argc);
    for (int i = 0; i < argc; i++) {
        char *string = va_arg(args, char *);
        length += strlen(string);
    }
    va_end(args);

    // allocate 
	char *result = malloc(length);
	if (result == NULL) {
		fprintf(stderr, "could not malloc\n");
		exit(-1);
	}
    strcpy(result, "");

    va_start(args, argc);
    for (int i = 0; i < argc; i++) {
        char *string = va_arg(args, char *);
        strcat(result, string);
    }
    va_end(args);
    return result;
}
