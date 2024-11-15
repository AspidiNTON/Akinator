#include "akinator.h"

void flush(){
    char c;
    while ((c = getchar()) != '\n');
}

bool gameLoop(Node* node){
    if (node == NULL) {
        printErr("Nullptr tree recieved\n");
        return false;
    }
    dumpTree(node);
    while (true) {
        char gameMode;
        printf("Enter what do you want:\n[G]uess, [P]rint object's definition by name, [S]how the tree, [E]xit game with or [W]ithout saving the tree?\n");
        gameMode = getchar();
        flush();
        switch (tolower(gameMode)){
            case 'g':
                if (!playGame(node)) return false;
                dumpTree(node);
                break;
            case 'p':
                printElementDefinition(node);
                break;
            case 's':
                if (!showTree()) return false;
                break;
            case 'e':
                writeNode("tree.txt", node);
                return true;
            case 'w':
                return true;
            default:
                printf("Please enter one letter, either 'g', 'p', 's', 'e' or 'w'.\n");
                break;
        }
    }
    return true;
}

bool showTree(){
    char* filename = getImageFilename();
    if (filename == NULL) return false;
    if (system(filename) != 0) {
        printErr("Failed to open image\n");
        return false;
    }
    free(filename);
    return true;
}

char getYesOrNoAnswer(){
    char playerAnswer;
    while (true) {
        playerAnswer = tolower(getchar());
        flush();
        if (playerAnswer == 'y' || playerAnswer == 'n') break;
        printf("Please enter 'y' or 'n':\n");
    }
    return playerAnswer;
}

Node* traverseTree(Node* node){
    while (node->left != NULL && node->right != NULL) {
        printf("%s? [y/n]:\n", node->data);
        char playerAnswer = getYesOrNoAnswer();
        if (playerAnswer == 'y') node = node->left;
        else node = node->right;
    }
    ASSERT(node->left == NULL && node->right == NULL, "Wtf it's not supposed to happen...\n");
    return node;
}

char* getAllocatedString(){
    char buff[200];
    fgets(buff, 200, stdin);
    int i = 0;
    while (*(buff + i) != '\n' && i < 199) ++i;
    if (*(buff + i) != '\n') flush();
    *(buff + i) = '\0';
    char* str = (char*)calloc(i, sizeof(char));
    if (str == NULL) {
        printErr("Unable to calloc for a string\n");
        return NULL;
    }
    strncpy(str, buff, i);
    return str;
}

bool addObject(Node* node){
    ASSERT(node, "Nullptr node recieved\n");
    printf("What was the correct answer?\n");
    char* answer = getAllocatedString();
    if (answer == NULL) return false;

    printf("Please write a question/property that \"%s\" does not possess but the correct answer \"%s\" does:\n", node->data, answer);
    char* question = getAllocatedString();
    if (question == NULL) return false;
    
    node->left = createNode(answer);
    node->right = createNode(node->data);
    node->data = question;
    return true;
}

bool checkAnswer(Node* node){
    ASSERT(node, "Nullptr node recieved\n");
    char playerAnswer;
    printf("Is it %s? [y/n]:\n", node->data);
    playerAnswer = getYesOrNoAnswer();
    if (playerAnswer == 'y') printf("Yippie, I won!\n");
    else return addObject(node);
    return true;
}

bool playGame(Node* node){
    ASSERT(node, "Nullptr node recieved\n");

    node = traverseTree(node);

    return checkAnswer(node);
}

Node* findElement(Node* node, const char* str){
    ASSERT(node, "Nullptr node recieved\n");
    if (stricmp(node->data, str) == 0 && node->left == NULL && node->right == NULL) return node;
    Node* ans = NULL;
    if (node->left != NULL) {
        ans = findElement(node->left, str);
        if (ans != NULL) {
            printf("YES: %s\n", node->data);
            return ans;
        }
    }
    if (node->right != NULL) {
        ans = findElement(node->right, str);
        if (ans != NULL) {
            printf("NO: %s\n", node->data);
            return ans;
        }
    }
    return ans;
}

bool printElementDefinition(Node* node){
    ASSERT(node, "Nullptr node recieved\n");
    printf("Please enter element name you want to find:\n");
    char* str = getAllocatedString();
    if (str == NULL) return false;
    if (findElement(node, str) == NULL) printf("Unable to find element with this name\n");
    free(str);
    return true;
}