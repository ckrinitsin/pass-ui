#include <stdlib.h>

typedef struct ll {
    char *value;
    struct ll *next;
} linked_list;


void *value_at(linked_list *head, size_t index);

linked_list *node_at(linked_list *head, size_t index);

linked_list *push_back(linked_list *head, void *value);

linked_list *pop_back(linked_list *head);

linked_list *back(linked_list *head);

linked_list *erase_list(linked_list *head);

linked_list *erase_heap(linked_list *head);

size_t size(linked_list *head);

void print_string_list(linked_list *head);

struct vault {
	size_t count_entries;     // Number entries
    char *vault_dir;          // Directory of the stored vaults
    linked_list *api_entry;   // Array of api version of entry, is synced with entry
    linked_list *entry;       // Array of all entries
};

