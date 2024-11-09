#include "dump.h"

#define TREE_FUNCTIONS_DUMP

static FILE* htmlFilePtr;
static int htmlImageCount;
char dotFilename[80] = "logger/result.dot";
char imageFilename[80] = "logger/output000.svg";

bool createTreeDotFile(Node* node, const char* outFilename);

void printTreeInDotFormat(Node* node, FILE* file, int rank);

bool createSvgFromDot(const char* inFilename, const char* outFilename);

bool initializeLogger(){
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
    //fprintf(htmlFilePtr, " function executed, resulting node:\n");
    if (!createTreeDotFile(node, dotFilename)) return false;
    imageFilename[13] = htmlImageCount / 100 + '0';
    imageFilename[14] = htmlImageCount / 10 % 10 + '0';
    imageFilename[15] = htmlImageCount % 10 + '0';
    if (!createSvgFromDot(dotFilename, imageFilename)) return false;
    fprintf(htmlFilePtr, "<img src=\"%s\">\n\n\n\n", imageFilename);
    ++htmlImageCount;
    #endif
    return true;
}

bool createTreeDotFile(Node* node, const char* outFilename){
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

    //beginning and vertices declaration
    /*fprintf(filePtr, "digraph list{\nrankdir=LR\n\tFREE\n");
    for (int i = 0; i < LIST_MAX_SIZE; ++i) {
        fprintf(filePtr, "\tx%d [shape=record,style=rounded,label=\"id: %d | data: ", i, i);
        fprintElement(filePtr, list->elements[i]);
        fprintf(filePtr, " | next: %d | prev: %d\"]\n", list->rightPointers[i], list->leftPointers[i]);
    }
    //next arrows
    fprintf(filePtr, "\tedge[color=blue]\n");
    int index = 0;
    do {
        fprintf(filePtr, "\tx%d->x%d\n", index, list->rightPointers[index]);
        index = list->rightPointers[index];
    } while (index != 0);
    //prev arrows
    fprintf(filePtr, "\tedge[color=red]\n");
    index = 0;
    do {
        fprintf(filePtr, "\tx%d->x%d\n", index, list->leftPointers[index]);
        index = list->leftPointers[index];
    } while (index != 0);
    //free memory
    fprintf(filePtr, "\tedge[color=green]\n");
    index = list->freeElement;
    while (list->rightPointers[index] != 0) {
        fprintf(filePtr, "\tx%d->x%d\n", index, list->rightPointers[index]);
        index = list->rightPointers[index];
    }
    fprintf(filePtr, "\tFREE->x%d\n", list->freeElement);
    putc('}', filePtr);
    fclose(filePtr);*/
}

void printTreeInDotFormat(Node* node, FILE* file, int rank){
    //printf("%p %p %d\n", node, file, rank);
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

bool createSvgFromDot(const char* inFilename, const char* outFilename){
    if (inFilename == NULL || outFilename == NULL) {
        printErr("Filename nullptr recieved\n");
        return false;
    }
    char command[100] = {};
    sprintf(command, "dot -Tsvg %s -o %s", inFilename, outFilename);
    if (system(command) != 0) {
        printErr("Image creation command failed\n");
        return false;
    }
    return true;
}