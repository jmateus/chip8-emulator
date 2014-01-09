#ifndef __SIMPLE_INI_H__
#define __SIMPLE_INI_H__

#include <stdbool.h>


#define SIMPLE_INI_DELIMETER "=#\n"
#define SIMPLE_INI_COMMENT_CHAR '#'
#define SIMPLE_INI_MAX_STRING_SIZE 256


typedef struct ini_node_t{
	char key[SIMPLE_INI_MAX_STRING_SIZE];
	char value[SIMPLE_INI_MAX_STRING_SIZE];
	struct ini_node_t *next;
} INI_NODE;


INI_NODE* ini_initSimpleIni(char* filename);

bool ini_hasKey(INI_NODE* head, char* key);

char* ini_getValue(INI_NODE* head, char* key);
char* ini_getValueOrDefault(INI_NODE* head, char* key, char* defaultValue);
int ini_getInt(INI_NODE* head, char* key, int base);

unsigned int ini_size(INI_NODE* head);

#endif