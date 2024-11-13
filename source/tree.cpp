#include "tree.h"

Node* createNode(TreeElem data){
    Node* node = (Node*)calloc(1, sizeof(Node));
    node->data = data;
    return node;
}

bool destroyNode(Node* node){
    if (node == NULL) {
        printErr("Node nullptr recieved aaa\n");
        return false;
    }
    if (node->left  != NULL) destroyNode(node->left);
    if (node->right != NULL) destroyNode(node->right);
    free(node);
    return true;
}

void printNode(FILE* file, Node* node){
    if (node == NULL) return;
    fprintf(file, "( ");
    printNodeData(file, node->data);
    putc(' ', file);

    if (node->left == NULL) putc('*', file);
    else printNode(file, node->left);
    putc(' ', file);

    if (node->right == NULL) putc('*', file);
    else printNode(file, node->right);
    fprintf(file, " )");
}

bool writeNode(const char* filename, Node* node){
    if (filename == NULL) {
        printErr("Filename is nullptr\n");
        return false;
    }
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printErr("Unable to open file\n");
        return false;
    }
    printNode(file, node);
    fclose(file);
    return true;
}

Node* readTree(const char* filename){
    if (filename == NULL) {
        printErr("Filename is nullptr\n");
        return NULL;
    }
    struct stat sb;
    if (stat(filename, &sb) == -1) {
        printErr("Failed to read file stats\n");
        return NULL;
    }
    char* buff = (char*)calloc(sb.st_size + 1, sizeof(char));
    if (buff == NULL) {
        printErr("Failed to calloc buffer for tree read\n");
        return NULL;
    }

    FILE *ptr = fopen(filename, "r");
    if (ptr == NULL) {
        free(buff);
        printErr("Unable to open file\n");
        return NULL;
    }
    fread(buff, sizeof(char), sb.st_size, ptr);
    fclose(ptr);

    Node* node = readNode(buff + 2, buff + sb.st_size - 3);
    return node;
}

Node* readNode(const char* begin, const char* end){
    int firstElement = 0;
    while (*(begin + firstElement) != '(' && *(begin + firstElement) != '*') ++firstElement;
    char* str = (char*)calloc(firstElement, sizeof(char));
    strncpy(str, begin, firstElement - 1);
    Node* node = createNode(str);

    if (*(begin + firstElement) == '*') return node;
    int secondElement = firstElement, bracketCount = 0;
    do{
        if      (*(begin + secondElement) == '(') ++bracketCount;
        else if (*(begin + secondElement) == ')') --bracketCount;
        ++secondElement;
    } while (bracketCount > 0);
    node->left = readNode(begin + firstElement + 2, begin + secondElement - 3);

    ++secondElement;
    if (*(begin + secondElement) == '*') return node;
    node->right = readNode(begin + secondElement + 2, end - 2);
    return node;
}


//Node* createBranch(TreeElem data, Node* left, Node* right);

//Node* insertNode(Tree* tree, Node* node, treeCmpFunc cmp);