#include "akinator.h"

void getline(char* buff){
    fgets(buff, 199, stdin);
    int i = 0;
    while (*(buff + i) != '\n') ++i; // strspn
    *(buff + i) = '\0';
}

bool gameLoop(Node* node){
    if (node == NULL) {
        printErr("Nullptr tree recieved\n");
        return false;
    }
    dumpTree(node);
    while (true) {
        playGame(node);
        dumpTree(node);
        printf("Wanna play another game? [y/n]:\n");
        char str[100] = "";
        scanf("%99s", str);
        if (stricmp(str, "y")) break;
    }
    return true;
}

bool playGame(Node* node){ // разбей на функции (неуд2)
    char str[100] = "";
    while (node->left != NULL && node->right != NULL) {
        printf("%s [y/n]:\n", node->data);
        while (scanf("%99s", str)) {
            if (stricmp(str, "y") == 0 || stricmp(str, "n") == 0) break;
            printf("Please enter \"y\" or \"n\":\n");
        }
        if (stricmp(str, "y") == 0) node = node->left;
        else                        node = node->right;
    }

    if ((node->left != NULL && node->left == NULL) || (node->left == NULL && node->left != NULL)) {
        printErr("Wtf it's not supposed to happen...\n");
        return false;
    }

    printf("Is it %s? [y/n]:\n", node->data);
    scanf("%99s", str);
    if (stricmp(str, "y") == 0) printf("Yippie, I won!\n");
    else {
        char ans[200] = "";
        printf("What was the correct answer?\n");
        fgets(ans, 200, stdin); //flush
        getline(ans);
        char* answer = (char*)calloc(strlen(ans) + 1, sizeof(char));
        strcpy(answer, ans);
        char quest[200] = "";
        printf("Please write a question/property that \"%s\" does not possess but the correct answer \"%s\" does:\n", node->data, ans);
        getline(quest);
        char* question = (char*)calloc(strlen(quest) + 1, sizeof(char));
        strcpy(question, quest);
        node->left = createNode(answer);
        node->right = createNode(node->data);
        node->data = question;
    }
    return true;
}