#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "simpleini.h"


static bool isComment(char *line) {
	return line[0] == SIMPLE_INI_COMMENT_CHAR;
}


static bool isLineEmpty(char *line) {
	if(strlen(line) == 0) {
		return true;
	}
	else if(line[0] == '\n') {
		return true;
	}
	else {
		return false;
	}
}


static INI_NODE* addNode(INI_NODE *head, char* key, char* value) {
	INI_NODE *newNode = (INI_NODE*) malloc(sizeof(INI_NODE));

	strcpy(newNode->key, key);
	strcpy(newNode->value, value);
	newNode->next = head;

	return newNode;
}


static INI_NODE* readFile(INI_NODE *head, FILE *file) {
	char line[SIMPLE_INI_MAX_STRING_SIZE];

	if(file == NULL) {
		return NULL;
	}

	int i = 0;

	while( (fgets(line, SIMPLE_INI_MAX_STRING_SIZE, file)) != NULL) {
		//TODO: trim line

		if(isLineEmpty(line)) {
			continue;
		}
		else if(isComment(line)) {
			continue;
		}
		else {
			char *key, *value;

			key = strtok(line, SIMPLE_INI_DELIMETER);
			value = strtok(NULL, SIMPLE_INI_DELIMETER);

			head = addNode(head, key, value);
		}
	}

	return head;
}


char* ini_getValue(INI_NODE* head, char* key) {
	INI_NODE *current = head;

	while(current != NULL) {
		if(strcmp(current->key, key) == 0) { //TODO: ignore case
			return current->value;
		}
		else {
			current = current->next;
		}
	}

	return NULL;
}


char* ini_getValueOrDefault(INI_NODE* head, char* key, char* defaultValue) {
	char *value = ini_getValue(head, key);

	if(value == NULL) {
		return defaultValue;
	}
	else {
		return value;
	}
}


bool ini_hasKey(INI_NODE* head, char* key) {
	INI_NODE *current = head;

	while(current != NULL) {
		if(strcmp(current->key, key) == 0) { //TODO: ignore case
			return true;
		}
		else {
			current = current->next;
		}
	}

	return false;
}


unsigned int ini_size(INI_NODE* head) {
	INI_NODE *current = head;
	int size = 0;

	while(current != NULL) {
		size++;
		current = current->next;
	}

	return size;
}


int ini_getInt(INI_NODE* head, char* key, int base) {
	char* value = ini_getValue(head, key);
	return (int) strtol(value, NULL, base);
}


INI_NODE* ini_initSimpleIni(char *filename) {
	INI_NODE *head = NULL;
	FILE *iniFile = fopen(filename, "r");
	head = readFile(head, iniFile);

	return head;
}

