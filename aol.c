#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define MAX_ITEMS 100
#define STACK_SIZE 50
#define QUEUE_SIZE 50

int i;
// Membuat struct untuk item yang akan digunakan untuk menyimpan item
typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    int quantity;
} Item;

// Struct Array digunakan untuk Create (case 1)
typedef struct {
    Item items[MAX_ITEMS];
    int size;
} StructArray;

// Menginisialisasi Structue Array
void sa_init(StructArray *sa) {
    sa->size = 0;
}

// Mencari dimana item yang ingin dicari
int sa_find(StructArray *sa, int id) {
    for (i = 0; i < sa->size; i++)
        if (sa->items[i].id == id)
            return i;
    return -1;
}

// Membuat item baru menggunakan Structue Array
void sa_add(StructArray *sa, Item item) {
    if (sa->size >= MAX_ITEMS) {
        printf("Storage full! Cannot add item.\n");
        return;
    }
    if (sa_find(sa, item.id) != -1) {
        printf("Item with ID %d already exists in Struct Array.\n", item.id); // Mencegah untuk IDnya tidak boleh sama
        return;
    }
    sa->items[sa->size++] = item; //Menyimpan item pada size selanjutnya di Structure Array
    printf("Item added to Struct Array.\n");
}

// Menghapus item dari Struct Array berdasarkan ID
void sa_delete(StructArray *sa, int id) {
    int index = sa_find(sa, id); // Mencari index item berdasarkan ID
    if (index == -1) {
        printf("Item dengan ID %d tidak ditemukan di Struct Array.\n", id);
        return;
    }
    // Menggeser item setelah index yang dihapus ke kiri
    for (i = index; i < sa->size - 1; i++) {
        sa->items[i] = sa->items[i + 1];
    }
    sa->size--; // Mengurangi ukuran Struct Array
    printf("Item dengan ID %d dihapus dari Struct Array.\n", id);
}

// Linked List digunakan untuk Delete (case 4)
// Membuat nodenya
typedef struct Node {
    Item item;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
} LinkedList;

// Menginisialisasi Linked List dengan membuat headnya jadi NULL dslu
void ll_init(LinkedList *ll) {
    ll->head = NULL;
}

// Menghapus node dari memory agar tidak ada resource leaks
void ll_clear(LinkedList *ll) {
    Node *curr = ll->head;
    while (curr) {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }
    ll->head = NULL;
}

// Sync data dari Structure Array
void ll_sync_from_array(LinkedList *ll, StructArray *sa) {
    ll_clear(ll); //menghapus nodes yang ada di linked list jika ada yang tersisa dari operasi sebelumnya
    for (i = sa->size - 1; i >= 0; i--) { //mengambil data dari SA reversed untuk menjaga original ordernya karena pakai insert from head.
        Node *newNode = malloc(sizeof(Node));
        if (newNode == NULL) continue;
        newNode->item = sa->items[i];
        newNode->next = ll->head;
        ll->head = newNode;
    }
}

// Display biasa
void ll_display(LinkedList *ll) {
    Node *cur;
    if (!ll->head) {
        printf("Linked List is empty.\n");
        return;
    }
    printf("Linked List Items:\n");
    printf("ID\tName\t\tQuantity\n");
    for (cur = ll->head; cur != NULL; cur = cur->next) {
        printf("%d\t%-15s\t%d\n", cur->item.id, cur->item.name, cur->item.quantity);
    }
}

// Hapus item berdasarkan ID di Linked List
void ll_delete_item(LinkedList* ll, int id) {
    Node* curr = ll->head;
    Node* prev = NULL;
    while (curr && curr->item.id != id) {
        prev = curr;
        curr = curr->next;
    }
    if (!curr) {
        printf("Item dengan ID %d tidak ditemukan di Linked List.\n", id);
        return;
    }
    if (!prev) {
        ll->head = curr->next;
    } else {
        prev->next = curr->next;
    }
    free(curr);
    printf("Item dihapus dari Linked List.\n");
}

// Stack untuk Update (case 3)
// Membuat stack
typedef struct {
    Item items[STACK_SIZE];
    int top;
} Stack;

// Menginisialisasi stack
void stack_init(Stack *st) {
    st->top = -1;
}

// Mencari item yang ingin di update
int stack_find(Stack *st, int id) {
    for (i = 0; i <= st->top; i++)
        if (st->items[i].id == id)
            return i;
    return -1;
}

// Mengambil data dari SA
void stack_sync_from_array(Stack *st, StructArray *sa) {
    stack_init(st);
    for (i = 0; i < sa->size && i < STACK_SIZE; i++) { //mengambil data dari SA dari bawah agar sama ordernya
        st->items[++st->top] = sa->items[i];
    }
}

// Mengupdate item menggunakan stack
void stack_update_item(Stack *st, Item item) {
    int idx = stack_find(st, item.id);
    if (idx == -1) {
        printf("Item with ID %d not found in Stack.\n", item.id);
        return;
    }
    st->items[idx] = item;
    printf("Item updated in Stack.\n");
}

// Queue untuk VIEW (case 4, view only)
// Membuat queue
typedef struct {
    Item items[QUEUE_SIZE];
    int front;
    int rear;
    int size;
} Queue;

// Menginisialisasi Queue
void queue_init(Queue *q) {
    q->front = 0;
    q->rear = QUEUE_SIZE - 1;
    q->size = 0;
}

// Mengambil data dari SA untuk queue (sync untuk tampil)
void queue_sync_from_array(Queue *q, StructArray *sa) {
    queue_init(q);
    for (i = 0; i < sa->size && i < QUEUE_SIZE; i++) {
        q->rear = (q->rear + 1) % QUEUE_SIZE;
        q->items[q->rear] = sa->items[i];
        q->size++;
    }
}

// Tampilkan semua item dalam queue (view saja)
void queue_display(Queue *q) {
    if (q->size == 0) {
        printf("Queue empty.\n");
        return;
    }
    printf("Queue Items (front to rear):\n");
    printf("ID\tName\t\tQuantity\n");
    int count = q->size;
    int index = q->front;
    while (count--) {
        printf("%d\t%-15s\t%d\n", q->items[index].id, q->items[index].name, q->items[index].quantity);
        index = (index + 1) % QUEUE_SIZE;
    }
}

// Binary Search Tree untuk Search (case 5)
// Membuat struct bst
typedef struct BSTNode {
    Item item;
    struct BSTNode *left, *right;
} BSTNode;

// Membuat node 
BSTNode* bst_create_node(Item item) {
    BSTNode* node = malloc(sizeof(BSTNode));
    if (node == NULL) return NULL;
    node->item = item;
    node->left = node->right = NULL;
    return node;
}

// Insert BST berdasarkan ID
BSTNode* bst_insert(BSTNode* root, Item item) {
    if (!root) return bst_create_node(item);
    if (item.id < root->item.id) root->left = bst_insert(root->left, item);
    else if (item.id > root->item.id) root->right = bst_insert(root->right, item);
    else printf("Item with ID %d already in BST.\n", item.id);
    return root;
}

// Search BST dengan ID
BSTNode* bst_search(BSTNode* root, int id) {
    if (!root || root->item.id == id) return root;
    if (id < root->item.id) return bst_search(root->left, id);
    return bst_search(root->right, id);
}

// Menampilkan secara inorder
void bst_inorder(BSTNode* root) {
    if (!root) return;
    bst_inorder(root->left);
    printf("%d\t%-15s\t%d\n", root->item.id, root->item.name, root->item.quantity);
    bst_inorder(root->right);
}

// Melepaskan node yang ada di memori
void bst_free(BSTNode* root) {
    if (!root) return;
    bst_free(root->left);
    bst_free(root->right);
    free(root);
}

// Circular Linked List untuk VIEW Filter jika quantity > 10 (case 6)
// Membuat struct ciruclar
typedef struct CNode {
    Item item;
    struct CNode *next;
} CNode;

typedef struct {
    CNode *tail;
} CircularList;

// Menginisialisasi cirucular
void cl_init(CircularList *cl) {
    cl->tail = NULL;
}

// Melepas node pada memori
void cl_clear(CircularList *cl) {
    if (!cl->tail) return;
    CNode *cur = cl->tail->next;
    cl->tail->next = NULL; // Break cycle agar bisa di free tempnya
    while (cur) {
        CNode *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    cl->tail = NULL;
}

// Insert menggunakan Circular
void cl_insert(CircularList *cl, Item item) {
    CNode *node = malloc(sizeof(CNode));
    if (!node) return;
    node->item = item;
    if (cl->tail == NULL) {
        cl->tail = node;
        node->next = node; // jika belum ada tailnya newNode tadi akan menjadi tail
    } else {
        node->next = cl->tail->next;
        cl->tail->next = node;
        cl->tail = node; //Menyambungkan juga agar jadi ciruclar 
    }
}

// Mengambil data yang hanya quantity > 10
void cl_sync_filtered(CircularList *cl, StructArray *sa) {
    cl_clear(cl);
    for (i = 0; i < sa->size; i++) {
        if (sa->items[i].quantity > 10) {
            cl_insert(cl, sa->items[i]);
        }
    }
}

// Display circular
void cl_display(CircularList *cl) {
    if (!cl->tail) {
        printf("No items found in Circular Linked List.\n");
        return;
    }
    printf("Circular Linked List Items (quantity > 10):\n");
    printf("ID\tName\t\tQuantity\n");
    CNode *head = cl->tail->next;
    CNode *cur = head;
    do {
        printf("%d\t%-15s\t%d\n", cur->item.id, cur->item.name, cur->item.quantity);
        cur = cur->next;
    } while (cur != head);
}

// Input Item
void input_item(Item *item) {
    printf("Enter ID: ");
    while (scanf("%d", &item->id) != 1) { // Validasi input ID
        printf("Invalid input. Please enter a numeric ID: ");
        while(getchar() != '\n'); // Menghapus input yang tidak valid
    }
    getchar(); // Menghapus newline dari input buffer
    printf("Enter name: ");
    fgets(item->name, MAX_NAME_LEN, stdin);
    item->name[strcspn(item->name, "\n")] = 0; // Menghapus newline dari nama
    printf("Enter quantity: ");
    scanf("%d", &item->quantity);
    getchar(); // Menghapus newline dari input buffer
}

// Menu
void menu() {
    printf("\n=== Warehouse Management ===\n");
    printf("1. Add item (Struct Array)\n");
    printf("2. View all items (Queue - view only)\n");
    printf("3. Update item (Stack)\n");
    printf("4. Delete item (Linked List)\n");
    printf("5. Search item (BST)\n");
    printf("6. View items with quantity > 10 (Circular Linked List)\n");
    printf("0. Exit\n");
    printf("Choose case: ");
}

int main() {
    // Inisialisasi Struct Array, Linked List, Stack, Queue, Binary Search Tree, Circular Linked List.
    StructArray sa;
    sa_init(&sa);

    LinkedList ll;
    ll_init(&ll);

    Stack st;
    stack_init(&st);

    Queue q;
    queue_init(&q);

    BSTNode* bst_root = NULL;

    CircularList cl;
    cl_init(&cl);

    int choice;
    Item temp;
    int idx;
    BSTNode *found;

    while (1) { // Infinite Loop
        menu(); // Menampilkan menu
        scanf("%d", &choice); // Mengambil input pilihan dari pengguna
        getchar(); // Menghapus newline dari input buffer

        switch (choice) {
            case 0: // Exit
                bst_free(bst_root); // Melepaskan memori BST
                cl_clear(&cl); // Melepaskan memori Circular Linked List
                ll_clear(&ll); // Melepaskan memori Linked List
                printf("Exiting...\n");
                return 0; // Keluar dari program

            case 1: // Add item (Struct Array)
                input_item(&temp); // Mengambil input item dari pengguna
                sa_add(&sa, temp); // Menambahkan item ke Struct Array
                // Sinkronisasi data ke struktur lainnya
                ll_sync_from_array(&ll, &sa);
                stack_sync_from_array(&st, &sa);
                queue_sync_from_array(&q, &sa);
                bst_free(bst_root); // Melepaskan memori BST sebelumnya
                bst_root = NULL; // Reset root BST
                for (i = 0; i < sa.size; i++)
                    bst_root = bst_insert(bst_root, sa.items[i]); // Menyisipkan item ke BST
                cl_sync_filtered(&cl, &sa); // Sinkronisasi Circular Linked List
                break;

            case 2: // View all items (Queue - view only)
                queue_display(&q); // Menampilkan semua item dalam queue
                break;

            case 3: // Update item (Stack)
                printf("Enter ID to update: ");
                scanf("%d", &temp.id);
                getchar();
                idx = stack_find(&st, temp.id); // Mencari item di stack
                if (idx == -1) {
                    printf("Item ID %d not found in Stack.\n", temp.id);
                    break;
                }
                input_item(&temp); // Mengambil input item baru
                st.items[idx] = temp; // Memperbarui item di stack
                // Memperbarui Struct Array dan sinkronisasi
                idx = sa_find(&sa, temp.id);
                if (idx != -1) {
                    sa.items[idx] = temp; // Memperbarui item di Struct Array
                    ll_sync_from_array(&ll, &sa); // Sinkronisasi Linked List
                    queue_sync_from_array(&q, &sa); // Sinkronisasi Queue
                    bst_free(bst_root); // Melepaskan memori BST
                    bst_root = NULL; // Reset root BST
                    for (i = 0; i < sa.size; i++)
                        bst_root = bst_insert(bst_root, sa.items[i]); // Menyisipkan item ke BST
                    cl_sync_filtered(&cl, &sa); // Sinkronisasi Circular Linked List
                }
                printf("Item updated via Stack.\n");
                break;

            case 4: // Delete item (Linked List)
                printf("Enter ID to delete: ");
                scanf("%d", &temp.id);
                getchar();
                idx = sa_find(&sa, temp.id); // Mencari item di Struct Array
                if (idx == -1) {
                    printf("Item ID %d tidak ditemukan di Struct Array.\n", temp.id);
                    break;
                }
                sa_delete(&sa, temp.id); // Menghapus item dari Struct Array
                ll_delete_item(&ll, temp.id); // Menghapus item dari Linked List
                // Sinkronisasi Queue
                queue_sync_from_array(&q, &sa);
                // Sinkronisasi Stack dan BST
                stack_sync_from_array(&st, &sa);
                bst_free(bst_root); // Melepaskan memori BST
                bst_root = NULL; // Reset root BST
                for (i = 0; i < sa.size; i++)
                    bst_root = bst_insert(bst_root, sa.items[i]); // Menyisipkan item ke BST
                cl_sync_filtered(&cl, &sa); // Sinkronisasi Circular Linked List
                break;

            case 5: // Search item (BST)
                printf("Enter ID to search: ");
                scanf("%d", &temp.id);
                getchar();
                found = bst_search(bst_root, temp.id); // Mencari item di BST
                if (found) {
                    printf("Item found:\nID: %d\nName: %s\nQuantity: %d\n",
                           found->item.id, found->item.name, found->item.quantity);
                } else {
                    printf("Item not found in BST.\n");
                }
                break;

            case 6: // View items quantity > 10 (Circular Linked List)
                cl_display(&cl); // Menampilkan item di Circular Linked List
                break;

            default:
                printf("Invalid option. Please choose 0-6.\n"); // Menangani input yang tidak valid
        }
    }

    return 0;

}