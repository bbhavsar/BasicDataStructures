#ifndef _QUEUE_H_
#define _QUEUE_H_

struct queueNode {
   struct queueNode *prev;
   struct queueNode *next;
   void *data;
};

typedef struct queueNode queueNode;

void
insertQueueNodeBack(queueNode **head,
                    void *data);
void
insertQueueNodeFront(queueNode **head,
                     void *data);
void *
removeQueueNode(queueNode **head);


#endif

