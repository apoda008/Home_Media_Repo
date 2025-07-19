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
MediaData** Resize_Hash_Table(MediaData** orginal_table, size_t size_array) {
	if (orginal_table == NULL) {
		printf("Invalid hash table or new size.\n");
		return NULL;
	}
	size_t new_size = size_array * 2; //double the size of the hash table

	MediaData** new_table = malloc(new_size * sizeof(MediaData*));
	if (new_table == NULL) {
		printf("Memory allocation failed for resized hash table.\n");
		return NULL;
	}
	for (int i = 0; i < new_size; i++) {
		if (i < size_array) {
			new_table[i] = orginal_table[i]; //copy the old data into the new table
		}
		else {
			new_table[i] = NULL; //initialize all slots to NULL
		}
	}
	//Rehash the existing data into the new table

	free(orginal_table); //free the old table
	return new_table;
}

MediaNode* Bin_Read(MediaData** hash_table, TCHAR* database_file, size_t size) {

	FILE* file = _tfopen(database_file, _T("rb"));
	if (file == NULL) {
		perror("Failed to open file");
		return 0;
	}
	else {
		printf("bin open successfully\n");
	}

	//MediaData buffer
	MediaData temp;

	while ((fread(&temp, sizeof(MediaData), 1, file)) == 1) {
		printf("Entered the while\n");
		MediaData* new_node = (MediaData*)malloc(sizeof(MediaData));
		if (new_node == NULL) {
			perror("Memory allocation failed for new node");
			fclose(file);
			return NULL;
		}

		memcpy(new_node, &temp, sizeof(MediaData));

		//This will be removed after testing
		//printf("\nTitle: %s\n", new_node->title);
		//printf("ID: %f\n", new_node->tmdb_id);
		//printf("Description: %s\n", new_node->description);
		//printf("Media Type: %d\n", new_node->media_type);
		//_tprintf(_T("Directory Position: %s\n\n"), new_node->dir_position_media);
		////////////////////////////////////

		Insert_Hash_Table(hash_table, new_node, size);

		//free(new_node);
		//new_node = NULL;

	}

	fclose(file);
	return;
}
//hash function 
size_t Hash_Function(const char* title, size_t array_size) {
	//simple hash function that sums the ASCII values of the characters in the title
	size_t hash = 0;
	while (*title) {
		hash += (unsigned char)(*title);
		if (hash > array_size) {
			hash %= array_size; //to ensure it fits in the array size
			printf("Beyond array size\n");
		}
		title++;
	}
	printf("hash: %zu\n", hash);
	return hash;
}

void Insert_Hash_Table(MediaData** hash_table, MediaData* data, size_t array_size) {
	if (hash_table == NULL) {
		printf("Hash table or data is NULL.\n");
		return;
	}

	size_t index = Hash_Function(data->title, array_size); 
	
	//if the index is already occupied, find the next available slot
	//spreading in both directions; inserting at a higher first then
	//reversing if the insert reaches the end of the array 
	if (hash_table[index] != NULL) {
		printf("=COLLISION=\n");
		size_t above = index - array_size;
		for (int i = index; i < array_size; i++) {
			if ((i < array_size) && (hash_table[i + 1] == NULL) ) {
				index = i + 1;
				//insert
				hash_table[index] = data;
				break;
			}
			if (i == array_size) {
				for (int j = index; j > 0; j--) {
					if ((j > 0) && (hash_table[j - 1] == NULL)) {
						index = j - 1;
						hash_table[index] = data;
						//insert
						break;
					}
					if (j == 0) {
						printf("Hash table is full, cannot insert %s\n", data->title);
						hash_table = Resize_Hash_Table(hash_table, array_size);
						Insert_Hash_Table(hash_table, data, (array_size * 2)); //resize and reinsert
						return; //if we reach the end of the array and nothing is found the table is full 
					}
				}

			}
		}
	}
	else { hash_table[index] = data; } //if the index is empty, insert it there

	printf("Inserted %s at index %zu\n", data->title, index);
}


MediaData** Hash_Initialization(size_t amount_of_files, Master_Directory* global_ptr) {
	printf("HASHING BABY\n");
	//starts a hash table of MediaData pointers
	size_t adjusted_size = amount_of_files * 2;
	MediaData** hash_table = malloc(adjusted_size * sizeof(MediaData*));
	if (hash_table == NULL) {
		printf("Memory allocation failed for hash table.\n");
		return NULL;
	}
	for (size_t i = 0; i < adjusted_size; i++) {
		hash_table[i] = NULL; //initialize all slots to NULL
	}

	global_ptr->size_of_hash = adjusted_size; //set the size of the hash table in the global struct

	//TEST===========================================
	MediaData temp = { 1, "JACKASS", 123456789, {0}, "This is a test description.", "C:\\test\\movie.bin" };
	size_t pos = Hash_Function(temp.title, adjusted_size);
	hash_table[pos] = &temp; //insert a test value to see if it works
	_tprintf(_T("Hash table initialized with size: %zu\n"), adjusted_size);
	printf("Title of test: %s\n", hash_table[pos]->title);
	//==================================================

	TCHAR dir_iteration[MAX_PATH];
	_tcscpy_s(dir_iteration, MAX_PATH, global_ptr->movie_bin_path);
	_tcscat_s(dir_iteration, MAX_PATH, _T("\\*"));

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile(dir_iteration, &findFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		_tprintf(_T("Error handle value. Error: %lu \n"), GetLastError());
		free(hash_table);
		hash_table = NULL; //free the hash table if it was allocated
		return NULL;
	}
	else {
		do {
			if ((strcmp(findFileData.cFileName, ".") == 0) || (strcmp(findFileData.cFileName, "..") == 0)) {
				continue;
			}

			//check if it's a directory or a file
			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				_tprintf(_T("[DIR]: %s\n"), findFileData.cFileName);
				
				TCHAR dir_for_reading[MAX_PATH];
				_tcsnccpy_s(dir_for_reading, MAX_PATH, global_ptr->movie_bin_path, MAX_PATH);
				_tcscat_s(dir_for_reading, MAX_PATH, _T("\\"));
				_tcscat_s(dir_for_reading, MAX_PATH, findFileData.cFileName);
				_tprintf(_T("String: %s\n"), dir_for_reading);
				Bin_Read(hash_table, dir_for_reading, adjusted_size);
			}
		} while (FindNextFile(hFind, &findFileData) != 0);

		if (GetLastError() != ERROR_NO_MORE_FILES) {
			printf("FindNextFile Failed (%d)\n", GetLastError());
			free(hash_table);
			hash_table = NULL; //free the hash table if it was allocated
			return NULL;
		}

	}
	
	FindClose(hFind); //close the find handle
	
	size_t hash_size = 0;
	//ITERATION THROUGH HASH TO MAKE SURE
	for (int i = 0; i < adjusted_size; i++) {
		if (hash_table[i] != NULL) {
			printf("Index %d: %s\n", i, hash_table[i]->title);
			hash_size += sizeof(MediaData); //calculate the size of each MediaData		
		}
		else {
			//printf("Index %d: NULL\n", i);
			hash_size += sizeof(MediaData*); //calculate the size of each pointer
		}
	}

	printf("Created Hash Table with size: %zu bytes\n", hash_size);

	return hash_table;
}

void Delete_From_Hash_Table(MediaData** hash_table, const char* title) {
	if (hash_table == NULL || title == NULL) {
		printf("Hash table or title is NULL.\n");
		return;
	}
	size_t index = Hash_Function(title, sizeof(hash_table) / sizeof(MediaData*));
	if (hash_table[index] != NULL && strcmp(hash_table[index]->title, title) == 0) {
		free(hash_table[index]);
		hash_table[index] = NULL; //set the pointer to NULL after freeing
		printf("Deleted %s from hash table at index %zu\n", title, index);
	}
	else {
		printf("Title %s not found in hash table.\n", title);
	}
}

MediaData* Search_Hash_Table(MediaData** hash_table, const char* title, size_t size_array) {
	if (hash_table == NULL || title == NULL) {
		printf("Hash table or title is NULL.\n");
		return NULL;
	}
	size_t index = Hash_Function(title, size_array);
	if (hash_table[index] != NULL && strcmp(hash_table[index]->title, title) == 0) {
		printf("Found %s at index %zu\n", title, index);
		return hash_table[index];
	}
	else if (hash_table[index] != NULL) {
		for (int i = index + 1; i < size_array; i++) {
			if (hash_table[i] != NULL && strcmp(hash_table[i]->title, title) == 0) {
				printf("Found %s at index %zu\n", title, i);
				return hash_table[i];
			}
			if (i == size_array - 1) {
				for (int j = index - 1; j < index; j--) {
					if (hash_table[j] != NULL && strcmp(hash_table[j]->title, title) == 0) {
						printf("Found %s at index %zu\n", title, j);
						return hash_table[j];
					}
					if (j == 0) {
						printf("Title %s not found in hash table.\n", title);
						return NULL; //if we reach the end of the array and nothing is found
					}
				}
			}
		}
	}
	else {
		printf("Title %s not found in hash table.\n", title);
		return NULL;
	}
}



//&&&&&&&&&&&&&&&&&&&&&&&&-------&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
