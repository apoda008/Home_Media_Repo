#pragma once
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H
//#include "database_commands.h"
#include "master.h"


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
char* Hash_Initialization(int amount_of_files);

int Hash_Function(const char* title);

void Insert_Hash_Table(char* hash_table, MediaData* data);

void Delete_From_Hash_Table(char* hash_table, const char* title);

MediaData* Search_Hash_Table(char* hash_table, const char* title);


#endif // !DATA_STRUCTURES_H
