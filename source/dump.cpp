#include "dump.h"

#define TREE_FUNCTIONS_DUMP

static FILE* htmlFilePtr;
static int htmlImageCount;
char dotFilename[80] = "logger/result.dot";
char imageFilename[80] = "logger/output%d.svg";

bool createTreeDotFile(const Node* node, const char* outFilename);

void printTreeInDotFormat(const Node* node, FILE* file, int rank);

bool createSvgFromDot(const char* inFilename, const char* outFilename);

bool initializeLogger(){
    createSvgFromDot("123.txt", "234.txt & ");
    htmlImageCount = 0;
    htmlFilePtr = fopen("listLog.html", "w");
    if (htmlFilePtr == NULL) {
        printErr("Unable to open listLog.html...\n");
        return false;
    }
    fprintf(htmlFilePtr, "<pre style=\"font-size: 20px\">\n");
    return true;
}

void closeLogger(){
    fprintf(htmlFilePtr, "</pre>\n");
    fclose(htmlFilePtr);
}

bool dumpTree(Node* node){
    #ifdef TREE_FUNCTIONS_DUMP
    if (htmlFilePtr == NULL) {
        printErr("htmlFilePtr is uninitialized\n");
        return false;
    }
    if (!createTreeDotFile(node, dotFilename)) return false;
    char currentImageFilename[200] = {};
    snprintf(currentImageFilename, 199, imageFilename, htmlImageCount);
    if (!createSvgFromDot(dotFilename, currentImageFilename)) return false;
    fprintf(htmlFilePtr, "<img src=\"%s\">\n\n\n\n", currentImageFilename);
    ++htmlImageCount;
    #endif
    return true;
}

bool createTreeDotFile(const Node* node, const char* outFilename){
    if (outFilename == NULL) {
        printErr("Filename nullptr recieved\n");
        return false;
    }
    FILE* filePtr = fopen(outFilename, "w");
    if (filePtr == NULL) {
        printErr("Unable to open file: %s\n", outFilename);
        return false;
    }
    fprintf(filePtr, "digraph list{\n");
    fprintf(filePtr, "\tx%p [shape=record,style=rounded,label=\"{ data: %s | { left: 0x%p | right: 0x%p } }\"]\n", node, node->data, node->left, node->right);
    printTreeInDotFormat(node, filePtr, 1);
    putc('}', filePtr);
    fclose(filePtr);
    return true;
}

void printTreeInDotFormat(const Node* node, FILE* file, int rank){
    if (node == NULL) {
        printErr("Nullptr node recieved\n");
        return;
    }
    if (node->left != NULL) {
        printTreeInDotFormat(node->left, file, rank + 1);
        fprintf(file, "\tx%p [shape=record,style=rounded,label=\"{ data: %s | { left: 0x%p | right: 0x%p } }\"]\n", node->left, node->left->data, node->left->left, node->left->right);
        fprintf(file, "\tx%p->x%p\n", node, node->left);
    }
    if (node->right != NULL) {
        fprintf(file, "\tx%p [shape=record,style=rounded,label=\"{ data: %s | { left: 0x%p | right: 0x%p } }\"]\n", node->right, node->right->data, node->right->left, node->right->right);
        fprintf(file, "\tx%p->x%p\n", node, node->right);
        printTreeInDotFormat(node->right, file, rank + 1);
    }
}

bool isFilenamaValid(const char* str){
    const char* tmp = str;
    while (*str != '\0') {
        if (isalpha(*str) || isdigit(*str) || *str == '/' || *str == ' ' || (*str == '.' && *(str + 1) != '.')) ++str;
        else{
            printErr("Invalid filename \"%s\", character: %c (char code: %d)\n", tmp, *str, (int)*str);
            return false;
        }
    }
    return true;
}

bool createSvgFromDot(const char* inFilename, const char* outFilename){
    if (inFilename == NULL || outFilename == NULL) {
        printErr("Filename nullptr recieved\n");
        return false;
    }
    char command[200] = {};
    if (!isFilenamaValid(inFilename) || !isFilenamaValid(outFilename)) return false;

    snprintf(command, 199, "dot -Tsvg %s -o %s", inFilename, outFilename); // snprintf FIXME:
    if (system(command) != 0) {
        printErr("Image creation command failed\n");
        return false;
    }
    return true;
}