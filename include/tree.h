#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "string.h"

#define printErr(s, ...) fprintf(stderr, "\033[0;31m" s "\033[1;0m", ## __VA_ARGS__)

typedef char* TreeElem;

struct Node{
    TreeElem data;
    Node* left;
    Node* right;
};

typedef int (*treeCmpFunc) (const TreeElem, const TreeElem);

bool destroyNode(Node*);

Node* createNode(TreeElem data);

void printNodeData(FILE* file, TreeElem elem);

void printNode(FILE* file, Node* node);

bool writeNode(const char* filename, Node* node);

Node* readTree(const char* filename);

Node* readNode(const char* begin, const char* end);

//Node* createBranch(TreeElem data, Node* left, Node* right);

//Node* insertNode(Tree* tree, Node* node, treeCmpFunc cmp);

#endif