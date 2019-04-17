
typedef struct node
{
    void* data;
    struct node* next;
}Qnode;

typedef struct
{
    Qnode* head;
    Qnode* tail;
    int length;
}Queue;


Queue* createQueue();
void enqueue(Queue* q, void* data);
void* dequeue(Queue* q);
int queueEmpty(Queue* q);
void* peekFirst(Queue *q);
void* peekLast(Queue *q);
void freeQueue(Queue* q, int freeOrNot);
