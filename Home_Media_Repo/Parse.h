#pragma once
#ifndef PARSE_H
#define PARSE_H

#include "master.h"

typedef struct parse_node {
	struct parse_node* map_array[26];
	int value;
} parse_node;

//Command -> what you want to do
enum Command {
	SELECT = 0,
	CHANGE = 1,
	REMOVE = 2,
	SEARCH = 3,
	MOVIE = 4
};

enum Target {
	TITLE = 5,
	DESCRIPTION = 6,
	GENRE = 7,
	ALL = 8
};

enum Source {
	WHERE = 9,
	//HAVING = 10, //maybe later
	FROM = 11, //maybe later
	//JOIN = 12, //maybe later
};

enum Options {
	EQUALS = 13,
	LESSTHAN = 14,
	GREATERTHAN = 15,
	LIKE = 16,
	//BETWEEN = 17, //maybe later
	//IN = 18, //maybe later
	//ISNULL = 19, //maybe later
	//ISNOTNULL = 20, //maybe later
	ORDERBY = 21,
	GROUPBY = 22,
	ASCENDING = 23,
	DESCENDING = 24
};

//Target -> what objects/tables/columns you want to affect
//Source/Conditions -> Where the data comes from or rules applied
//Options/Clauses -> Extra modifiers
/*
*
* Example SQL query:
* SELECT column1, column2
* FROM table_name
* WHERE condition
* GROUP BY column1
* HAVING COUNT(*) > 1
* ORDER BY column2 DESC;
*
*/

int Does_Command_Exist(parse_node* head, const char* str);

void Add_Parse_Word(parse_node* head, const char* str, int val);

parse_node* initialize_parse_tree();

void test_unit();

void Free_Parse_Tree(parse_node* head);

#endif