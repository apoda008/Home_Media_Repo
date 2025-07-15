#include "data_structures.h"




//*********************LIST/NODE MANAGEMENT****************************
//*********************************************************************
void free_linked_list(MediaNode* header) {
	MediaNode* current = header;

	while (current != NULL) {
		MediaNode* to_delete = current;
		current = current->next;
		free(to_delete);
		to_delete = NULL;
	}
}

void print_list(MediaNode* head, char* choice) {
	//all data in the node
	while (head != NULL) {
		if (strcmp(choice, "all") == 0) {

			printf("================DATA ALL: ================\n");
			printf("db_pos: %d\n", head->data.db_position);
			printf("Title: %s\n", head->data.title);
			printf("tmdb_id: %f\n", head->data.tmdb_id);
			printf("media: %d\n", head->data.media_type);
			for (int i = 0; i < 19; i++) {
				printf("genre: %d\n", head->data.genre_types[i]);
			}
			printf("description: %s\n", head->data.description);
			printf("dir position: %s\n", head->data.dir_position_media);
			printf("===============DATA ALL: ================\n");

		}
		else if (strcmp(choice, "db_pos") == 0) {
			printf("db_pos: %d\n", head->data.db_position);
		}
		else if (strcmp(choice, "title") == 0) {
			printf("Title: %s\n", head->data.title);
		}
		else if (strcmp(choice, "tmdb_id") == 0) {
			printf("tmdb_id: %f\n", head->data.tmdb_id);
		}
		else if (strcmp(choice, "media_type") == 0) {
			printf("media: %d\n", head->data.media_type);
		}
		else if (strcmp(choice, "genre") == 0) {
			for (int i = 0; i < 19; i++) {
				printf("genre: %d\n", head->data.genre_types[i]);
			}
		}
		else if (strcmp(choice, "descr") == 0) {
			printf("description: %s\n", head->data.description);
		}
		else if (strcmp(choice, "dir") == 0) {
			printf("dir position: %s\n", head->data.dir_position_media);
		}
		else {
			printf("FAILED: improper second argument");
		}
		head = head->next;
	}
}

void insert_node_LL(MediaNode* node, MediaNode* header) {
	MediaNode* start = header;
	
	while (header != NULL) {
		if (strcmp(node->data.title, node->data.title) > 0) {
			node->next = start->next;
			start->next = node;
			break;
		}
		start = start->next;
	}
}

void split_list(MediaNode* source, MediaNode** front_ref, MediaNode** back_ref) {
	MediaNode* slow = source;
	MediaNode* fast = source->next;

	while (fast != NULL) {
		fast = fast->next;
	}
	if (fast != NULL) {
		slow = slow->next;
		fast = fast->next;
	}

	*front_ref = source;
	*back_ref = slow->next;
	slow->next = NULL;
}

MediaNode* merge_list(MediaNode* a, MediaNode* b) {
	if (a == NULL) { return b; }
	if (b == NULL) { return a; }

	MediaNode* result = NULL;

	//this part will need adjustment if comparison parameters chage
	if (strcmp(a->data.title, b->data.title) <= 0) {
		result = a;
		result->next = merge_list(a->next, b);
	}
	else {
		result = b;
		result->next = merge_list(a, b->next);
	}

	return result;
}

void merge_sort(MediaNode** headRef) {
	MediaNode* head = *headRef;

	if (head == NULL || head->next == NULL) {
		return; //0 or 1 node
	}

	MediaNode* front;
	MediaNode* back;

	split_list(head, &front, &back);

	merge_sort(&front);
	merge_sort(&back);

	*headRef = merge_list(front, back);

}


//---------------------------TREES--------------------------------------
TreeNode* create_tree_node(MediaData item) {
	TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
	new_node->data = item;
	new_node->left = NULL;
	new_node->right = NULL;
	return new_node;
}

TreeNode* sorted_to_bst(MediaNode** source, int start, int end) {
	if (start > end) {
		return NULL;
	}
	MediaNode* current = source;
	int mid = start + (end - start) / 2;
	TreeNode* left = sorted_to_bst(source, start, mid - 1);
	TreeNode* root = create_tree_node((*source)->data);
	root->left = left;
	*source = (*source)->next;
	root->right = sorted_to_bst(source, mid + 1, end);

	return root;

}

void inorder_traversal_print(TreeNode* root) {
	if (root == NULL) {
		return;
	}
	printf("ROOT: Title: %s\n", root->data.title);
	inorder_traversal_print(root->left);

	printf("Title: %s\n", root->data.title);

	inorder_traversal_print(root->right);
}

TreeNode* free_binary_tree(TreeNode* root) {
	if (root == NULL) {
		return NULL;
	}
	
	root->left = free_binary_tree(root->left);
	root->right = free_binary_tree(root->right);

	printf("Freeing node with value %s\n", root->data.title);
	free(root);
	
	return NULL;
}

//*********************************************************************
//*********************************************************************


//&&&&&&&&&&&&&&&&&&&&---HASH MAP------&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

//needs to be the size of total movies + 1/2 the size 
//
//int hashfuction(char* title_to_parse);

//



//&&&&&&&&&&&&&&&&&&&&&&&&-------&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
