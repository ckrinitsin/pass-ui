#include "structs.h"
#include <stdio.h>

void *value_at(linked_list *head, size_t index) {
	for (int i = 0; i < index; i++) {
		if (head == NULL)
			return NULL;
		head = head->next;
	}
	return head->value;
}

linked_list *node_at(linked_list *head, size_t index) {
	for (int i = 0; i < index; i++) {
		if (head == NULL)
			return NULL;
		head = head->next;
	}
	return head;
}

linked_list *push_back(linked_list *head, void *value) {
	linked_list *save = head;
	if (head == NULL) {
		while (head == NULL)
			head = malloc(sizeof(linked_list));
		head->value = value;
		head->next = NULL;
		return head;
	}
	while (head->next != NULL)
		head = head->next;
	while (head->next == NULL)
		head->next = malloc(sizeof(linked_list));
	head->next->value = value;
	head->next->next = NULL;
	return save;
}

linked_list *pop_back(linked_list *head) {
	linked_list *save = head;
	if (head == NULL)
		return NULL;
	if (head->next == NULL) {
		free(head);
		return NULL;
	}
	while (head->next->next != NULL)
		head = head->next;
	free(head->next);
	head->next = NULL;
	return save;
}

linked_list *back(linked_list *head) {
	if (head == NULL)
		return NULL;
	while (head->next != NULL)
		head = head->next;
	return head;
}

linked_list *erase_list(linked_list *head) {
    linked_list *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
	return NULL;
}

linked_list *erase_heap(linked_list *head) {
    linked_list *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp->value);
        free(tmp);
    }
	return NULL;
}

size_t size(linked_list *head) {
	int result = 0;
	while (head != NULL) {
		result++;
		head = head->next;
	}
	return result;
}

//debug method
void print_string_list(linked_list *head) {
    while (head != NULL) {
        printf("%s\n", head->value);
        head = head->next;
    }
}
