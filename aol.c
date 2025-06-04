#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Item {
    int id;
    char name[50];
    int quantity;
    float price;
} Item;

typedef struct Node {
    Item item;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

Node* root = NULL;

int max(int a, int b){
    return (a > b)? a : b;
}

int height(Node* root){
    if(root == NULL){
        return 0;
    }
    return root->height;
}

Node* newNode(Item item){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->item = item;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

Node* rightRotate(Node* y){
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;
    return x;
}

Node* leftRotate(Node* x){
    Node* y = x->left;
    Node* T2 = y->right;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
    return y;
}

void menu() {
    int choice;
    do {
        system("cls");
        printf("Warehouse Management\n");
        printf("1. Add Item\n");
        printf("2. Update Item\n");
        printf("3. Delete Item\n");
        printf("4. View All Items\n");
        printf("5. Search Item\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1: addItem(); break;
            case 0: printf("Exiting program.\n"); break;
            default: printf("Invalid choice. Try again.\n"); system("pause");
        }
    } while(choice != 0);
}