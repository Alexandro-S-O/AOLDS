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

int getBalance(Node* root){
    if(root == NULL){
        return 0;
    }
    return height(root->left) - height(root->right);
}

Node* insertNode(Node* node, Item item) {
    if (node == NULL)
        return newNode(item);

    if (item.id < node->item.id)
        node->left = insertNode(node->left, item);
    else if (item.id > node->item.id)
        node->right = insertNode(node->right, item);
    else // Tidak boleh ada duplicate
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && item.id < node->left->item.id)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && item.id > node->right->item.id)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && item.id > node->left->item.id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && item.id < node->right->item.id) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

void addItem() {
    system("cls");
    Item newItem;
    printf("Add New Item\n");
    printf("Enter ID: ");
    scanf("%d", &newItem.id);
    if (searchNode(root, newItem.id) != NULL) {
        printf("Item with this ID already exists!\n");
        system("pause");
        return;
    }
    printf("Enter Name: ");
    scanf(" %[^\n]s", newItem.name);
    printf("Enter Quantity: ");
    scanf("%d", &newItem.quantity);
    printf("Enter Price: ");
    scanf("%f", &newItem.price);
    root = insertNode(root, newItem);
    printf("Item added successfully.\n");
    system("pause");
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
            case 2: ;
            case 0: printf("Exiting program.\n"); break;
            default: printf("Invalid choice. Try again.\n"); system("pause");
        }
    } while(choice != 0);
}