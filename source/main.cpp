#include "tree.h"
#include "akinator.h"

void printNodeData(FILE* file, TreeElem elem){
    fprintf(file, "%s", elem);
}

int main(){
    Node* tree = readTree("tree.txt");
    initializeLogger();

    gameLoop(tree);

    //writeNode("xdddddddd.txt", tree);

    closeLogger();
    destroyNode(tree);
    return 0;
}