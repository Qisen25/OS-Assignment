#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

/*
 * Queue implementation
 *AUTHOR: Kei Sum Wang    student id: 19126089
 *REFERENCE: this is based off linked list from the unit Unix and C programming
 */

/*
 *Initialisation of queue
 *EXPORT: an initialised queue ready to use
 */
Queue* createQueue()
{
    Queue* q;

    q = (Queue*)malloc(sizeof(Queue));
    q->head = NULL;
    q->tail = NULL;
    q->length = 0;

    return q;
}

/*
 * enqueue 
 *PURPOSE: inserts data into queue. each data inserted at end of queue
 *IMPORT: queue and a data of type void pointer
 */
void enqueue(Queue* q, void* data)
{
    Qnode* newNode;

    newNode = (Qnode*)malloc(sizeof(Qnode));
    newNode->next = NULL;

    newNode->data = data;

    if(q->head == NULL)//if queue empty
    {
        q->head = newNode;
    }
    else
    {
        q->tail->next = newNode;
    }

    q->tail = newNode;

    q->length++;
}

/*
 * dequeue 
 *PURPOSE: remove data from queue. each data remove from front of queue
 *IMPORT: queue
 *EXPORTS: void pointer
 */
void* dequeue(Queue* q)
{
    Qnode* currHead;
    void* value;

    currHead = q->head;

    if(q->length == 0)
    {
        value = NULL;
        printf("List is Empty: nothing to dequeue\n");
    }
    else
    {
        value = currHead->data;
        if(currHead->next == NULL)
        {
            q->tail = NULL;
        }
     
        q->head = currHead->next;

        q->length--;
    }
    free(currHead);

    return value;
}

/*
 *Check if queue is empty
 *IMPORT: queue
 *EXPORT: integer indicating empty or nor
 */
int queueEmpty(Queue* q)
{
    int isEmpty = 0;

    //nothing at head then queue is empty
    if(q->head == NULL)
    {
        isEmpty = 1;
    }

    return isEmpty;
}

/*
 * peek at front of queue
 *IMPORT: queue
 *EXPORT: data of type void pointer
 */
void* peekFirst(Queue *q)
{
    void* value;

    if (q->length == 0)
    {
        value = NULL;
    }
    else
    {
        value = q->head->data;
    }

    return value;
}

/*
 * peek at end of queue
 *IMPORT: queue
 *EXPORT: data of type void pointer
 */
void* peekLast(Queue *q)
{
    void* value;

    if (q->length == 0)
    {
        value = NULL;
    }
    else
    {
        value = q->tail->data;
    }

    return value;
}

/*
 * free queue and its content from memory if needed
 *IMPORT: queue, integer indicating to free data or not
 */
void freeQueue(Queue* q, int freeOrNot)
{
    Qnode* node, *nextNode;
    
    node = q->head;/*start at front of q*/
    while (node != NULL)
    {
        nextNode = node->next;/*get next node*/
        if (freeOrNot == 1)/*if freeOrNot is 1
                                    then free the data. 
                                    Otherwise don't free data*/
        {
            free(node->data);/*free malloced data*/
        }
        free(node);
        node = nextNode;/*make sure current is the next node*/
    }

    free(q);
}

