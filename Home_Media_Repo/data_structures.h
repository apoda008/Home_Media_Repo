#pragma once
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H
//#include "database_commands.h"
#include "master.h"
#include "program_functions.h"


typedef struct MediaData {
    int db_position;
    //watch this; changed from 260 to 256
    char title[256];
    double tmdb_id;
    bool media_type; // (0 or 1) 1 for movie, 0 for show
    int genre_types[19];
    char description[2000];
    TCHAR dir_position_media[MAX_PATH];
} MediaData;


typedef struct MediaNode {
    MediaData data;
    struct MediaNode* next;
} MediaNode;

typedef struct TreeNode {
    MediaData data;

    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct Genre_Dict_Node {
	char genre_name[50];
	MediaNode* media_list; //linked list of media nodes
	struct Genre_Dict_Node* next;
} Genre_Dict_Node;

typedef struct Genre_Dict_Header {
    //heads of the linked lists to the respective genres
	Genre_Dict_Node* head_action; 
    Genre_Dict_Node* head_adventure;
    Genre_Dict_Node* head_animation;
    Genre_Dict_Node* head_comedy;
    Genre_Dict_Node* head_crime;
    Genre_Dict_Node* head_documentary;
    Genre_Dict_Node* head_drama;
    Genre_Dict_Node* head_family;
    Genre_Dict_Node* head_fantasy;
    Genre_Dict_Node* head_history;
    Genre_Dict_Node* head_horror;
    Genre_Dict_Node* head_music;
    Genre_Dict_Node* head_mystery;
    Genre_Dict_Node* head_romance;
    Genre_Dict_Node* head_science_fiction;
    Genre_Dict_Node* head_tv_movie;
    Genre_Dict_Node* head_thriller;
    Genre_Dict_Node* head_war;
    Genre_Dict_Node* head_western;
    
    
    size_t size; //size of the linked list
	TCHAR genre_path[MAX_PATH]; //path to the genre bin files
} Genre_Dict_Header;


//Forward
MediaNode* Bin_Read(MediaData** hash_table, TCHAR* database_file, size_t size);

/*
===========LINKED LIST==================
*/
//inserts a single node into list
void insert_node(MediaNode* node);

void split_list(MediaNode* source, MediaNode** front_ref, MediaNode** back_ref);
//function for merge_sort
MediaNode* merge_list(MediaNode* a, MediaNode* b);
//merge sorts the bin file
void merge_sort(MediaNode** headRef);

//MAYBE??
void insert_node_LL(MediaNode* source);
//Prints list
void print_list(MediaNode*, char* choice);
//frees linked list
void free_linked_list(MediaNode* header);
//frees individual node (todo) 
void free_individual_node(MediaNode* pointer, void* ref_obj);


/*
===========BINARY TREE===================
*/

//creates and mallocs a single tree node
TreeNode* create_tree_node(MediaData item);

//takes a sorted linked list and builds bottom up into a binary tree
TreeNode* sorted_to_bst(MediaNode** source, int start, int end);

//prints out a sorted binary tree
void inorder_traversal_print(TreeNode* root);

//frees mem from a binary tree
TreeNode* free_binary_tree(TreeNode* root);

/////TIME TO MAKE A FUCKING HASH TABLE!!!///////
/////LETS FUUCKING GOOOOOO//////////////////////


MediaData** Hash_Initialization(size_t amount_of_files, Master_Directory* global_ptr);

size_t Hash_Function(const char* title, size_t array_size);

void Insert_Hash_Table(MediaData**, MediaData* data, size_t size_of_array);

void Delete_From_Hash_Table(MediaData** hash_table, char* title);

MediaData* Search_Hash_Table(MediaData** hash_table, char* title, size_t size_array);

MediaData** Resize_Hash_Table(MediaData** orginal_table);



#endif // !DATA_STRUCTURES_H
