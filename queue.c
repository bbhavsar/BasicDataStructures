#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "queue.h"
#define VERBOSE 1
#if VERBOSE
   #define LOG(str)  printf str
#else
   #define LOG(str)
#endif

void
insertQueueNodeBack(queueNode **head,  // IN/OUT
                    void *data)        // IN
{
   queueNode *node = (queueNode *)malloc(sizeof *node);

   assert(node);
   node->data = data;

   if (*head == NULL) {
      node->prev = node->next = node;
      *head = node;
   } else {
      queueNode *tail = (*head)->prev;

      node->prev = tail;
      node->next = *head;

      tail->next = node;
      (*head)->prev = node;
   }
}


void
insertQueueNodeFront(queueNode **head,    // IN/OUT
                     void *data)          // IN
{
   queueNode *node = (queueNode *)malloc(sizeof *node);

   assert(node);
   node->data = data;

   if (*head == NULL) {
      node->prev = node->next = node;
   } else {
      queueNode *tail = (*head)->prev;

      node->next = *head;
      node->prev = tail;

      tail->next = node;
      (*head)->prev = node;
   }

   *head = node;
}


void *
removeQueueNode(queueNode **head)
{
   void *retVal;
   queueNode *newHead = *head;
   queueNode *tail;

   if (!(*head)) {
      return NULL;
   }

   retVal = (*head)->data;
   tail = (*head)->prev;
   if (*head == tail) {
      free(*head);
      *head = NULL;
      return retVal;
   }

   newHead = (*head)->next;
   tail->next = newHead;
   newHead->prev = tail;

   free(*head);
   *head = newHead;

   return retVal;
}


void
iterateList(queueNode *queue)
{
   queueNode * const head = queue;
   queueNode *curr;

   if (!head) {
      return;
   }

   curr = head;
   do {
      LOG(("%d\t", (int) curr->data));
      curr = curr->next;
   } while (curr != head);
   LOG(("\n"));
}


/* Unit test */
#if 1
int main()
{
   int arr[] = {-2, 20, 10, 15};
   int i;
   queueNode *list = NULL;

   for (i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
      insertQueueNodeBack(&list, (void *)arr[i]);
   }

   iterateList(list);

   printf("Removed %d\n", (int)removeQueueNode(&list));
   iterateList(list);

   printf("Adding -1 to head.\n");
   insertQueueNodeFront(&list, (void *)-1);
   iterateList(list);

   return 0;
}
#endif
