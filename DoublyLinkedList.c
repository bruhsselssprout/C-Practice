/* A doubly-linked list whose node values are independently heap-allocated. */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StringNode {
	char *value;
	struct StringNode *previous;
	struct StringNode *next;
} StringNode;

typedef struct {
	StringNode *head;
	StringNode *tail;
	size_t size;
} DoublyLinkedList;

static char *copy_string(const char *value)
{
	size_t length;
	char *copy;

	if (value == NULL) {
		return NULL;
	}

	length = strlen(value) + 1;
	copy = malloc(length);
	if (copy != NULL) {
		memcpy(copy, value, length);
	}
	return copy;
}

void list_init(DoublyLinkedList *list)
{
	if (list != NULL) {
		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
	}
}

/* Appends a copy of value. Returns 1 on success and 0 on failure. */
int list_insert(DoublyLinkedList *list, const char *value)
{
	StringNode *node;

	if (list == NULL || value == NULL) {
		return 0;
	}

	node = malloc(sizeof(*node));
	if (node == NULL) {
		return 0;
	}

	node->value = copy_string(value);
	if (node->value == NULL) {
		free(node);
		return 0;
	}

	node->previous = list->tail;
	node->next = NULL;
	if (list->tail != NULL) {
		list->tail->next = node;
	} else {
		list->head = node;
	}
	list->tail = node;
	list->size++;
	return 1;
}

StringNode *list_find(const DoublyLinkedList *list, const char *value)
{
	StringNode *current;

	if (list == NULL || value == NULL) {
		return NULL;
	}

	for (current = list->head; current != NULL; current = current->next) {
		if (strcmp(current->value, value) == 0) {
			return current;
		}
	}
	return NULL;
}

/* Deletes the first item equal to value. Returns 1 if an item was deleted. */
int list_delete(DoublyLinkedList *list, const char *value)
{
	StringNode *node = list_find(list, value);

	if (node == NULL) {
		return 0;
	}

	if (node->previous != NULL) {
		node->previous->next = node->next;
	} else {
		list->head = node->next;
	}
	if (node->next != NULL) {
		node->next->previous = node->previous;
	} else {
		list->tail = node->previous;
	}

	free(node->value);
	free(node);
	list->size--;
	return 1;
}

void list_destroy(DoublyLinkedList *list)
{
	StringNode *current;

	if (list == NULL) {
		return;
	}

	current = list->head;
	while (current != NULL) {
		StringNode *next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	list_init(list);
}

static void trim_newline(char *text)
{
	size_t length;

	if (text == NULL) {
		return;
	}

	length = strlen(text);
	while (length > 0 && (text[length - 1] == '\n' || text[length - 1] == '\r')) {
		text[length - 1] = '\0';
		length--;
	}
}

static void print_list(const DoublyLinkedList *list)
{
	StringNode *current;

	if (list == NULL || list->head == NULL) {
		puts("List is empty.");
		return;
	}

	printf("List contents: ");
	for (current = list->head; current != NULL; current = current->next) {
		printf("%s%s", current->value, (current->next != NULL) ? " -> " : "");
	}
	putchar('\n');
}

int main(void)
{
	DoublyLinkedList list;
	char input[256];
	int choice;

	list_init(&list);
	puts("Doubly Linked List Demo");

	while (1) {
		puts("\n1. Insert a value");
		puts("2. Delete a value");
		puts("3. Search for a value");
		puts("4. Display the list");
		puts("5. Exit");
		printf("Choose an option: ");

		if (fgets(input, sizeof(input), stdin) == NULL) {
			break;
		}

		trim_newline(input);
		choice = atoi(input);

		switch (choice) {
		case 1:
			printf("Enter a value to insert: ");
			if (fgets(input, sizeof(input), stdin) == NULL) {
				puts("Input failed. Exiting.");
				list_destroy(&list);
				return 0;
			}
			trim_newline(input);
			if (input[0] == '\0') {
				puts("Value cannot be empty.");
				break;
			}
			if (list_insert(&list, input)) {
				printf("Inserted: %s\n", input);
			} else {
				puts("Failed to insert value.");
			}
			break;

		case 2:
			printf("Enter a value to delete: ");
			if (fgets(input, sizeof(input), stdin) == NULL) {
				puts("Input failed. Exiting.");
				list_destroy(&list);
				return 0;
			}
			trim_newline(input);
			if (list_delete(&list, input)) {
				printf("Deleted: %s\n", input);
			} else {
				printf("Value not found: %s\n", input);
			}
			break;

		case 3:
			printf("Enter a value to search: ");
			if (fgets(input, sizeof(input), stdin) == NULL) {
				puts("Input failed. Exiting.");
				list_destroy(&list);
				return 0;
			}
			trim_newline(input);
			if (list_find(&list, input) != NULL) {
				printf("Found: %s\n", input);
			} else {
				printf("Not found: %s\n", input);
			}
			break;

		case 4:
			print_list(&list);
			break;

		case 5:
			puts("Goodbye!");
			list_destroy(&list);
			return 0;

		default:
			puts("Invalid option. Please choose 1-5.");
			break;
		}
	}

	return 0;
}
