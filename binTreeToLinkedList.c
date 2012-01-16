#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define ARRAY_SIZE(a) (sizeof a / sizeof a[0])
struct  node {
    int data;
    union {
        struct node *left;
        struct node *prev;
    };

    union {
        struct node *right;
        struct node *next;
    };
};


typedef struct node node;

node *addNode(node *n, int elem)
{
    if (n == NULL) {
        node *leaf = malloc(sizeof *leaf);
        assert(leaf != NULL);
        leaf->data = elem;
        leaf->left = leaf->right = NULL;
        return leaf;
    }

    if (elem <= n->data) {
        n->left = addNode(n->left, elem);
    } else {
        n->right = addNode(n->right, elem);
    }

    return n;
}

void inorder(node *n)
{
    if (n == NULL) {
        return;
    }

    inorder(n->left);
    printf("%d ", n->data);
    inorder(n->right);
}

// Convert binary search tree to a sorted circular doubly linked list
// in place.
void convertBinTreeToLinkedList(node *root, node **head)
{
    if (root == NULL) {
        return;
    }

    // Necessary to save the right child else it can
    // be modified by the code below.
    node *right = root->right;
    convertBinTreeToLinkedList(root->left, head);

    if (*head == NULL) {
        *head = root;
        (*head)->next = *head;
        (*head)->prev = *head;
    } else {
        // Insert node at the tail.
        node *oldTail = (*head)->prev;
        root->prev = oldTail;
        root->next = *head;

        oldTail->next = root;
        (*head)->prev = root;
    }

    convertBinTreeToLinkedList(right, head);
}


void iterateList(node *head)
{
    node *it = head;
    
    if (it == NULL)  {
        printf("Empty list\n");
        return;
    }

    do {
        printf("%d, ", it->data);
        it = it->next;
    } while (it != head);

    printf("\n");
}


int main()
{
    int arr[] = {25, 11, -50, 500, -2, 10};
    int i;
    node *root = NULL;

    for (i = 0; i < ARRAY_SIZE(arr); i++) 
        printf("%d ", arr[i]);

    printf("\n");



    for (i = 0; i < ARRAY_SIZE(arr); i++) {
        root = addNode(root, arr[i]);
    }

    inorder(root);
    printf("\n");

    node *head = NULL;
    convertBinTreeToLinkedList(root, &head);
    iterateList(head);
}
