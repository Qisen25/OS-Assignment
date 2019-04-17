/**
 *Task Scheduler simulator OS assignment
 *AUTHOR: Kei Sum Wang, student id: 19126089
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "scheduler.h"
#include "fileIO.h"

double num_tasks, total_waiting_time, total_turnaround_time; 
int hrs, mins, secs;//global variables, time should be accessible throughout

time_t now;

time(&now);

struct tm *curTime = localtime(&now);

hrs = curTim->tm_hour;
mins = curTim->tm_min;
secs = curTim->tm_sec;

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Not enough arguments");
    }

    return 0;
}

/**
 *task method
 *PURPPOSE: used by the task thread
 *IMPORTS: m (capacity of tasks in ready queue)
 */
void* task(int m, void* arg)
{
    wait(empty);//semaphore empty show empty spaces in queue

    //TODO this is currently pseudocode, do real implement soon
    if(m == 1 || q.length == m - 1)
    {
        q.enqueue(the task);
    }
    else if(!q.full())
    {
        q.enqueue(task1);
        printf("task#: cpu_burst");
        printf("Arrival time: %d:%d:%d", hrs, mins, secs);
        q.enqueue(task2);
        printf("task#: cpu_burst");
        printf("Arrival time: %d:%d:%d", hrs, mins, secs);
    }
    signal(full);//use semaphore full to show number of items in queue
}

/**
 *cpu
 *PURPOSE: To perform CPU operations
 */
void* cpu(void *arg)
{
    
}
