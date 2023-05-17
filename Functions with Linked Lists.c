#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

#define STRING_MAX 1024
typedef struct node {
char *name;
int lines;
int runtime;
int memory_usage;
struct node *next;
} Node;
typedef struct linked_list {
Node *head;
} LinkedList;
void readInputString(char *, int);
void readInputNumber(int *);
void readInputChar(char *);
LinkedList * newList();
char getCommand();
void handleInsert(LinkedList *);
void handleDelete(LinkedList *);
void handleSearch(LinkedList *);
void handlePrint(LinkedList *);
void handleQuit(LinkedList *);
int main() {
LinkedList *list = newList();
char command = '\0';
printf("Experimental database entry\n");
while (command != 'Q') {
command = getCommand();
switch (command) {
case 'I':
handleInsert(list);
break;
case 'D':
handleDelete(list);
break;
case 'S':
handleSearch(list);
break;
case 'P':
handlePrint(list);
break;
case 'Q':
handleQuit(list);
break;
}
}
free(list);
return 0;
}
void handleInsert(LinkedList *list) {
	int letters = 0, lines = 0, runtime = 0, memory = 0;
	printf("\nNumber of characters in file name: ");
	readInputNumber(&letters);

	char name[letters+1];
	printf("File name: ");
	readInputString(name, letters+1);

	printf("Number of lines in CSV file: ");
	readInputNumber(&lines);

	printf("Experiment runtime (ms): ");
	readInputNumber(&runtime);

	printf("Experiment memory usage (B): ");
	readInputNumber(&memory);

	Node* new_node = (Node*) malloc(sizeof(Node));
	new_node ->name = strdup(name);
	new_node ->lines = lines;
	new_node ->runtime = runtime;
	new_node ->memory_usage = memory;

	Node *current = list->head;
	Node *before = NULL;

		while (current != NULL && strcmp(current ->name, name) <= 0) {
		if (strcmp(current ->name, name) == 0) {
			printf("\nAn entry with that file name already exists.\n");
			free(new_node ->name);
			free(new_node);
			return;
		}
		before = current;
		current = current ->next;

	}

	if (before == NULL) {
		new_node ->next = list ->head;
		list ->head = new_node;
	}

	else {
		new_node ->next = current;
		before ->next = new_node;
	
	}
	
}

void handleDelete(LinkedList *list) {
	char delete[STRING_MAX+1];
	printf("\nEnter file name to delete: ");
	readInputString(&delete,STRING_MAX+1);
	Node *current = list->head;
	Node *before = NULL;
	while (current != NULL) {
		if (strcmp(current ->name, delete) == 0) {
			printf("Deleting entry for CSV file <%s>\n", current ->name);
			break;
		}
	
		before = current;
		current = current ->next;
	}

	if (current == NULL) {
		printf("An entry for file <%s> does not exist.\n", delete);
		return;
	}

	if (before == NULL) {
		list ->head = current ->next;
	}

	else {
		before ->next = current ->next; 
	}

	free(current->name);
    free(current);
	

}

void handleSearch(LinkedList *list) {
	char find[STRING_MAX+1];
	printf("\nEnter file name to find: ");
	readInputString(&find, STRING_MAX+1);
	Node *current = list ->head;
	Node *before = NULL;

	while (current != NULL) {
		if (strcmp(current ->name, find) == 0) {
			printf("File <%s>\n",current ->name);
			printf("Lines: %d\n", current ->lines);
			printf("Runtime (ms): %d\n", current ->runtime);
			printf("Memory usage (B): %d\n", current ->memory_usage);
			double th;
			double x = (current ->lines);
			double y = (current ->runtime)*0.001;
			th = x/y;
			th = round(th*100.0)/100.0;
			printf("Throughput: %.2f\n", th);
			return;
		}

		before = current;
		current = current ->next;
	}

	printf("An entry for file <%s> does not exist.\n", find);
}

void handlePrint(LinkedList *list) {
	Node *current = list ->head;
	Node *before = NULL;

	printf("\nData entries:\n");

	if (current == NULL) {
		printf("There are no data entries.\n");
		return;
	}

	while (current != NULL) {
		printf("\nFile <%s>\n",current ->name);
		printf("Lines: %d\n", current ->lines);
		printf("Runtime (ms): %d\n", current ->runtime);
		printf("Memory usage (B): %d\n", current ->memory_usage);
		double th;
		double x = (current ->lines);
		double y = (current ->runtime)*0.001;
		th = x/y;
		th = round(th*100.0)/100.0;
		printf("Throughput: %.2f\n", th);

		before = current;
		current = current ->next;
	}

}

void handleQuit(LinkedList *list) {
	Node *current = list->head;
	Node *temp = current;
	while (current != NULL) {
		printf("Deleting entry for CSV file <%s>\n", current ->name);
		temp = current;
		current = current ->next;
		free(temp ->name);
		
		free(temp);

	
	}
	list->head = NULL;

	
}
void readInputString(char *str, int length) {
int i = 0;
char c;
while (i < length && (c = getchar()) != '\n') {
str[i++] = c;
}
str[i] = '\0';
}
void readInputNumber(int *number) {
char buf[STRING_MAX];
readInputString(buf, STRING_MAX);
*number = (int)strtol(buf, (char **)NULL, 10);
}
void readInputChar(char *character) {
char buf[2];
readInputString(buf, 2);
*character = buf[0];
}
LinkedList * newList() {
LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
list->head = NULL;
return list;
}
char getCommand() {
char command;
printf("\nSupported commands are:\n");
printf(" I - Insert\n");
printf(" D - Delete\n");
printf(" S - Search\n");
printf(" P - Print\n");
printf(" Q - Quit\n");
printf("\nPlease select a command: ");
readInputChar(&command);
while (command != 'I' && command != 'D' && command != 'S' && command != 'P'
&& command != 'Q') {
printf("Invalid command <%c>, please select a command: ", command);
readInputChar(&command);
}
return command;
}