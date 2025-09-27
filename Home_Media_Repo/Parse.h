#pragma once
#ifndef PARSE_H
#define PARSE_H

#include <ctype.h>

typedef struct parse_node {
	parse_node map_array[26];
	int value;
} parse_node;

void Add_Parse_Node(parse_node* head, const char* str, int val);

#endif