#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

Queue* createQueue()
{
    Queue* q;

    q = (Queue*)malloc(sizeof(Queue));
    q->head = NULL;
    q->tail = NULL;
    q->length = 0;

    return q;
}

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

int queueEmpty(Queue* q)
{
    int isEmpty = 0;

    if(q->head == NULL)
    {
        isEmpty = 1;
    }

    return isEmpty;
}

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


void freeQueue(Queue* q, int freeOrNot)
{
    Qnode* node, *nextNode;
    
    node = q->head;/*start at front of q*/
    while (node != NULL)
    {
        nextNode = node->next;/*get next node*/
        if (freeOrNot == 1)/*if dataFree is 1
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

