#pragma once
#ifndef PARSE_H
#define PARSE_H

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct parse_node {
	parse_node* map_array;
	int value;
} parse_node;

void Add_Parse_Node(parse_node* head, const char* str, int val);

#endif