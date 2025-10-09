#include "Parse.h"

int Does_Command_Exist(parse_node* head, const char* str) {
	parse_node* current = head;
	for (int i = 0; i < strlen(str); i++) {
		int index = toupper(str[i]); // Calculate index based on first character
		if (index < 'A' || index > 'Z') {
			printf("Char out of bounds error\n");
			return -1; // Invalid character
		}
		index -= 65; // Normalize to 0-25
		
		if (current->map_array[index] == NULL) {
			// If the node doesn't exist, the word is not in the tree
			return -1;
		}
		if (i == strlen(str) - 1) {
			// If it's the last character, return the value
			return current->value;
		}
		current = current->map_array[index]; // Move to the next node
	}
	return -1; // Word not found
}

void Add_Parse_Word(parse_node* head, const char* str, int val) {
	parse_node* current = head;
	
	for (int i = 0; i < strlen(str); i++) {

		int index = toupper(str[i]); // Calculate index based on first character

		if (index < 'A' || index > 'Z') {
			printf("Char out of bounds error\n");
			return; // Invalid character
		}
		
		index -= 65; // Normalize to 0-25

		if (current->map_array[index] == NULL) {
			// If the node doesn't exist, create it
			parse_node* new_node = calloc(1, sizeof(parse_node));
			if (new_node == NULL) {
				printf("Node creation error\n");
				return;
			}
			current->map_array[index] = new_node;
			current->value = -1; // Intermediate node
		}

		if (i == strlen(str) - 1) {
			// If it's the last character, set the value
			current->value = val;
			printf("Set value %d for word %s\n", val, str);
		}	

		current = current->map_array[index]; // Move to the next node
	}

}

parse_node* initialize_parse_tree() {
	parse_node* head = calloc(1, sizeof(parse_node));
	if (head == NULL) {
		printf("error\n");
		return NULL;
	}

	for (int i = 0; i < 26; i++) {
		//head->map_array[i].value = -1;
		head->map_array[i] = NULL;
	}

	head->value = -1; // Root node
	
	//head->map_array = (parse_node*)calloc(26, sizeof(parse_node)); // Allocate array for 26 letters
	if (head->map_array == NULL) {
		printf("error\n");
		free(head);
		return NULL;
	}

	Add_Parse_Word(head, "SELECT", SELECT);
	Add_Parse_Word(head, "CHANGE", CHANGE);
	Add_Parse_Word(head, "REMOVE", REMOVE);
	Add_Parse_Word(head, "SEARCH", SEARCH);
	Add_Parse_Word(head, "MOVIE", MOVIE);

	Add_Parse_Word(head, "TITLE", TITLE);
	Add_Parse_Word(head, "DESCRIPTION", DESCRIPTION);
	Add_Parse_Word(head, "GENRE", GENRE);
	Add_Parse_Word(head, "ALL", ALL);
	Add_Parse_Word(head, "ID", ID);
	
	Add_Parse_Word(head, "WHERE", WHERE);
	Add_Parse_Word(head, "FROM", FROM);
	
	Add_Parse_Word(head, "EQUALS", EQUALS);
	Add_Parse_Word(head, "LESSTHAN", LESSTHAN);
	Add_Parse_Word(head, "GREATERTHAN", GREATERTHAN);
	Add_Parse_Word(head, "LIKE", LIKE);
	Add_Parse_Word(head, "ORDERBY", ORDERBY);
	Add_Parse_Word(head, "GROUPBY", GROUPBY);
	Add_Parse_Word(head, "ASCENDING", ASCENDING);	
	Add_Parse_Word(head, "DESCENDING", DESCENDING);
	
	Add_Parse_Word(head, "MOVIES", MOVIES);
	Add_Parse_Word(head, "SERIES", SERIES);

	printf("Parse tree initialized successfully.\n");
	return head;
}

void Free_Parse_Tree(parse_node* head) {
	if (head == NULL) {
		return;
	}
	for (int i = 0; i < 26; i++) {
		if (head->map_array[i] != NULL) {
			Free_Parse_Tree(head->map_array[i]);
		}
	}
	free(head);
}

//TEST REGION
void test_unit() {
	parse_node* test = initialize_parse_tree();
	if(test == NULL) {
		printf("error\n");
		return;
	}
	
	int result = Does_Command_Exist(test, "from");
	printf("Result for SELECT: %d\n", result);

	

}