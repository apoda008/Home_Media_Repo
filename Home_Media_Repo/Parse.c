#include "Parse.h"

void test_unit_initalize() {

}

parse_node* Add_Parse_Word(parse_node* head, const char* str, int val) {
	parse_node* current = head;
	
	for (int i = 0; i < strlen(str); i++) {
		int index = toupper(str[i]) - 65; // Calculate index based on character
		if (current->map_array[index].value == 0 && current->map_array[index].map_array[0].value == 0) {
			
			// If the node doesn't exist, create it
			parse_node new_node = { 0 };
			current->map_array[index] = new_node;

			current->value = -1; // Intermediate node
		}
		if (i == strlen(str) - 1) {
			// If it's the last character, set the value
			current->value = val;
		}
		current = &current->map_array[index]; // Move to the next node
	}

}

parse_node* initialize_parse_tree() {
	parse_node* head = calloc(1, sizeof(parse_node));
	if (head == NULL) {
		printf("error\n");
		return NULL;
	}
	head->map_array = calloc(26, sizeof(parse_node)); // Allocate array for 26 letters
	if (head->map_array == NULL) {
		printf("error\n");
		free(head);
		return NULL;
	}

	Add_Parse_Node(head, "SELECT", 0);
	Add_Parse_Node(head, "CHANGE", 1);
	Add_Parse_Node(head, "REMOVE", 2);
	Add_Parse_Node(head, "SEARCH", 3);
	Add_Parse_Node(head, "TITLE", 4);
	Add_Parse_Node(head, "DESCRIPTION", 5);
	Add_Parse_Node(head, "GENRE", 6);
	Add_Parse_Node(head, "ALL", 7);
	Add_Parse_Node(head, "WHERE", 8);
	Add_Parse_Node(head, "FROM", 9);
	Add_Parse_Node(head, "EQUALS", 10);

}


//TEST REGION
void test_unit() {
	initialize_parse_tree();
	test_unit_initalize();
}