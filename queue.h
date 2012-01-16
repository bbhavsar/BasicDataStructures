// Trivial implementation of a queue using circular doubly linked list.

#ifndef _QUEUE_H_
#define _QUEUE_H_


// Forward declaration of queueNode to hide private members of queue implementation.
struct queueNode;
typedef struct queueNode queueNode;

void
insertQueueNodeBack(queueNode **head,
                    void *data);
void
insertQueueNodeFront(queueNode **head,
                     void *data);
void *
removeQueueNode(queueNode **head);


#endif // _QUEUE_H_

