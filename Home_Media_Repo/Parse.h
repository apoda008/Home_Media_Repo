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

void test_unit_print(parse_node* test);

void Add_Parse_Node(parse_node* head, const char* str, int val);

parse_node* initialize_parse_tree();

void test_unit_initalize(parse_node* test);

#endif