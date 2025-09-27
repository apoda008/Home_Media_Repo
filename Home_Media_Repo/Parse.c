#include "Parse.h"

void Add_Parse_Word(parse_node* head, const char* str, int val) {
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


//TEST REGION

parse_node head = { -1 };