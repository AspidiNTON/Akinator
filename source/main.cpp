#include "tree.h"
#include "akinator.h"

void printNodeData(FILE* file, TreeElem elem){
    fprintf(file, "%s", elem);
}

int main(){

    Node* tree = readTree("xdd.txt");
    initializeLogger();
    
    gameLoop(tree);

    
    writeNode("xdddddddd.txt", tree);

    closeLogger();
    return 0;
}