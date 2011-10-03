#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

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
    printf("%d, ", n->data);
    inorder(n->right);
}

// Convert binary search tree to circular doubly linked list
// in place.
void convertBinTreeToLinkedList(node *root, node **head)
{
    if (root == NULL) {
        return;
    }

    convertBinTreeToLinkedList(root->left, head);

    // Necessary to save the right child else it can
    // be modified by the code below.
    node *right = root->right;

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
    int arr[] = {0, 11, -50, -2, 10};
    int i;
    node *root = NULL;

    for (i = 0; i < sizeof arr / sizeof arr[0]; i++) {
        root = addNode(root, arr[i]);
    }

    inorder(root);
    printf("\n");

    node *head = NULL;
    convertBinTreeToLinkedList(root, &head);
    iterateList(head);
}
