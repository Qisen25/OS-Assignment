/**
 *Task Scheduler simulator OS assignment
 *AUTHOR: Kei Sum Wang, student id: 19126089
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "scheduler.h"
#include "fileIO.h"

int main(int argc, char *argv[])
{
    return 0;
}

/**
 *task method
 *PURPPOSE: used by the task thread
 *IMPORTS: m (capacity of tasks in ready queue)
 */
void* task(int m, void* arg)
{

    //TODO this is currently pseudocode, do real implement soon
    if(m == 1 || q.length == m - 1)
    {
        q.enqueue(the task);
    }
    else if(!q.full())
    {
        q.enqueue(task1);
        printf("task#: cpu_burst");
        printf("Arrival time: %d:%d:%d");
        q.enqueue(task2);
        printf("task#: cpu_burst");
        printf("Arrival time: %d:%d:%d");
    }
}

/**
 *Cpu
 *PURPOSE: To perform CPU operations
 */
void* cpu(void *arg)
{
    
}
