/*
 * Unit test for linked q
 *
 * by Kei Sum Wang, student id: 19126089
 * REFERENCE: this is from the unit Unix and C programming
 */

#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

void printFormat(void* data);
void testLastOps(Queue *q);


int main()
{
    Queue *q = createQueue();

    printf("====TEST HARNESS FOR Queue=======\n");

    printf("\n====Test creation of Queue=======\n");
    if (q->head == NULL && q->tail == NULL && q->length == 0)
    {
        printf("q creation success\n");
    }
    else
    {
        printf("creation of q failed\n");
    }

    printf("\n==========TESTING LAST OPERATIONS=============\n");
    testLastOps(q);
    
    freeQueue(q, 0);

    return 0;
}

void testLastOps(Queue *q)
{
    int d = 4, e = 5, f = 6;
    int *p1, *p2, *p3, *top, *end;

    printf("\n====enqueue. values %d, %d, %d\n", d, e, f);
    enqueue(q, &d);
    enqueue(q, &e);
    enqueue(q, &f);

    printf("===check first and last values and length with qs peekFirst(), peekLast() and length field===\n");
    top = peekFirst(q);
    end = peekLast(q);


    printf("peekFirst %d\n", *top);
    printf("peekLast %d\n", *end);
    printf("insert length %d\n", q->length);
    
    printf("====dequeue. expected order of delete 4, 5, 6\n");
    p1 = dequeue(q);
    printf("delete %d\n", *p1);
    p2 = dequeue(q);
    printf("delete %d\n", *p2);
    p3 = dequeue(q);
    printf("delete %d\n", *p3);
    printf("%d was last deletion next delete will indicate nothing to delete\n", *p3);
    dequeue(q);

    printf("delete length %d\n", q->length);
}



/*print function for linkedq displayList method*/
void printFormat(void* data)
{
    int *num;

    num = (int*)data;

    printf("%d\n", *num);
}
