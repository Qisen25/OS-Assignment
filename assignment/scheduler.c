/**
 *Task Scheduler simulator OS assignment
 *AUTHOR: Kei Sum Wang, student id: 19126089
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "scheduler.h"

float num_tasks = 0.0, total_waiting_time = 0.0, total_turnaround_time = 0.0; 

int bufferLimit, fileSize;

pthread_cond_t full, empty;//conditions for wait and signaling
pthread_mutex_t mutex;//to help setup mutual exclusion when entering crit section

//shared buffers
Queue* readyQueue;//buffer to be accessed by threads
Queue* fromFile;//file content are stored in queue

pthread_t task_thread, cpu1, cpu2, cpu3;

char *name1 = "CPU-1", *name2 = "CPU-2", *name3 = "CPU-3"; //names of cpus
char *simLog = "simulation_log";//output file

/*
 *MAIN
 *IMPORT: take in a count for # args, filename and size for ready queue
 */
int main(int argc, char *argv[])
{
    bufferLimit = atoi(argv[2]);

    if(argc < 3 || (bufferLimit < 1 || bufferLimit > 10))
    {
        printf("Incorrect arguments\nCheck readme for argument details\n");
        return -1;
    }

    //initilization of containers
    fromFile = createQueue();
    readyQueue = createQueue();

    if(readFile(argv[1], fromFile) != 0)
    {
    	freeQueue(fromFile, 0);
    	freeQueue(readyQueue, 0);
    	return -1;//unsuccessful read should end
    }

    //total number of tasks globally known
    fileSize = fromFile->length;

    //wipe previous log to prevent appending
    wipeLog(simLog);

    //initialise mutex and condition vars
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&full, NULL);
    pthread_cond_init(&empty, NULL);

    //task thread creation
    pthread_create(&task_thread, NULL, &task, NULL);

    //cpu threads creation
    pthread_create( &cpu1, NULL, &cpu, (void*) name1);
    pthread_create(&cpu2, NULL, &cpu, (void*) name2);
    pthread_create(&cpu3, NULL, &cpu, (void*) name3);

    //start threads
    pthread_join(task_thread, NULL);
    pthread_join(cpu1, NULL);
    pthread_join(cpu2, NULL);
    pthread_join(cpu3, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&full);
    pthread_cond_destroy(&empty);

    printf("%d flength begin\n", fromFile->length);
    printf("%d rlength begin\n", readyQueue->length);

    //free containers from memory
    freeQueue(fromFile, 0);
    freeQueue(readyQueue, 0);

    //output end stats the num_task, waiting and turn around stats to screen and file
    taskTimeStats();

    return 0;
}

/**
 *task method
 *PURPPOSE: used by the task thread to add tasks to ready queue
 */
void* task(void *arg)
{
    int now_hr = 0, now_min = 0, now_sec = 0, i, spaceLeft, concat;
    char summary[200];

    process *task1, *task2 ;

    while(!queueEmpty(fromFile))
    {
		
        pthread_mutex_lock(&mutex);//lock thread to prevent to incorrect processing
        //task thread is the only thread able access ready queue until unlock

        task1 = dequeue(fromFile);
        printf("task inputting\n\n");

        //wait for cpu to consume if buffer limit reached
        while(readyQueue->length == bufferLimit)
        {
            //printf("waiting for cpu to consume %d length \n", readyQueue->length);
            pthread_cond_wait(&empty, &mutex);
        }

        //critical section for queueing, task can only access ready when lock cpus cannot
        getTime(&(task1->arrive_hr), &(task1->arrive_min), &(task1->arrive_sec));//get current time
        enqueue(readyQueue, task1);

        //output task statistics to screen and file
        taskThreadStats(task1);

        //calculate the space left in ready queue
        spaceLeft = bufferLimit - readyQueue->length;
        /*buffer size must be greater 1 in order to get 2 tasks
         *file queue cannot be empty after task1 dequeue
         *space left must be greater than 0 or otherwise ready queue 1 task above buffer limit 
         */
        if(bufferLimit > 1 && !queueEmpty(fromFile) && spaceLeft > 0)
        {
            printf("********adding second task*************\n");
            task2 = dequeue(fromFile);
            getTime(&(task2->arrive_hr), &(task2->arrive_min), &(task2->arrive_sec));//get current time
            enqueue(readyQueue, task2);

            //output task threads stat for queueing of 2nd task
            taskThreadStats(task2);
        }


        pthread_cond_signal(&full);//signal that processes are available in readyqueue
        pthread_mutex_unlock(&mutex);//unlock mutex after done enqueueing

        printf("%d curr ready queue size \n", readyQueue->length);

    }

    /*Since there are 3 cpus, 1 cpu may block forever if there less than 3 tasks from file
     *task thread cannot signal in critical section once done, so loop to release any cpu blocked
     *This is to prevent deadlock
     */
    if(fileSize < 3)
    {
    	for(i = 0; i < 3; i++)
    	{
    		pthread_cond_signal(&full);
    	}
    }

    //concat string ready to write task thread summary to file and print to screen
    concat = sprintf(summary, "\nNumber of tasks put into Ready-Queue: %d\n", fileSize);
    getTime(&now_hr, &now_min, &now_sec);
    concat += sprintf(summary + concat, "Terminate at time: %d:%d:%d\n\n",  now_hr, now_min, now_sec);
    printf("%s", summary);
    writeToFile(simLog, summary);
    return NULL;

}

/**
 *cpu
 *PURPOSE: To perform CPU operations
 */
void* cpu(void *arg)
{
    int now_hr = 0, now_min = 0, now_sec = 0;
    int task_counter;
    char summary[50];
    char *cpu_name = (char*)arg;//get cpu thread name
    process* task;

    task_counter = 0;

    //**check criticalnotEmpty() to see the mutex lock**
    //Run if either ready is not empty or file queue still contains tasks.
    while(criticalNotEmpty())
    {
        /*if criticalNotEmpty() is true allow 1 cpu that is waiting to 
         *enter crit section of accessing ready queue
         */
        printf("%s begin\n", cpu_name);
        printf("%d flength begin %s\n", fromFile->length, cpu_name);
        printf("%d rlength begin %s\n", readyQueue->length, cpu_name);
        getTime(&now_hr, &now_min, &now_sec);
        printf("begin time: %d:%d:%d\n", now_hr, now_min, now_sec);


        printf("\n********ready queue remove %s********\n", cpu_name);
        //at least 1 item should be available to dequeue other this loop should've not execute
        task = dequeue(readyQueue);

        printf("*******%d %s curr ready queue size after deq*********\n", readyQueue->length, cpu_name);
        //calculate the wait and turn around time
        getTime(&(task->serv_hr), &(task->serv_min), &(task->serv_sec));
        calcWaitTime(task);
        calcTurnAround(task);

        //output cpu serving task stats
        cpuThreadStats(task, cpu_name, SERVING);

        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);//release lock now, cpu will now consume
                                     //no longer need lock since task out of ready queue		

        //remainder section aka consumin
        printf("********CONSUMING %s********\n", cpu_name);
        sleep(task->cpu_burst);
        getTime(&(task->comp_hr), &(task->comp_min), &(task->comp_sec));
        task_counter++;//count task for current thread
        num_tasks++;//count task for global

        //output cpu completion task stats
        cpuThreadStats(task, cpu_name, DONE);


        free(task);
    }

    printf("***CPU Termination*****\n");
    sprintf(summary,"\n%s terminates after servicing %d tasks\n", cpu_name, task_counter);
    printf("%s", summary);
    writeToFile(simLog, summary);
    return NULL;
}

/*
 *criticalNotEmpty function checks if cpu can enter the critical section
 *only if ready queue is not empty and file queue still has task
 *EXPORT: integer flag indicating ready and file queue is empty(0) or not(1)
 */
int criticalNotEmpty()
{
    int notEmpty = 0;

    pthread_mutex_lock(&mutex);	//one cpu hold lock at a time, enforce mutual exclusion
    /*this is critical section for cpu thread, must ensure that each cpu (one at a time)
     *checks the empty status of ready queue and file queue before consuming
     *this prevent cpu from entering loop and dequeing when previously another 
     *cpu task has just dequeued (RACE CONDITION)
     */

    //ready queue is empty but file queue still has tasks wait for task thread
    while(queueEmpty(readyQueue) && !queueEmpty(fromFile))
    {
        pthread_cond_wait(&full, &mutex);
    }

    //if this is true(1) then task can continue to critcal section to consume
    //we don't need to check if file queue is empty due to wait above
    //task thread would queue more tasks if ready queue is empty but there are still more tasks
    notEmpty = (!queueEmpty(readyQueue));

    //if ready queue and no more tasks to get, lock is no longer needed
    //nothing to consume thus not enter crit section
    if(queueEmpty(readyQueue) && queueEmpty(fromFile))
    {
        pthread_mutex_unlock(&mutex);	
    }

    return notEmpty;
}

/*
 *function to get time on system. Uses C time standard library
 *IMPORTS: takes in pointers to integer for hours, minutess and seconds
 */
void getTime(int *hr, int *min, int *sec)
{
    time_t now;

    time(&now);

    struct tm *curTime = localtime(&now);

    *hr = curTime->tm_hour;
    *min = curTime->tm_min;
    *sec = curTime->tm_sec;
}

/*
 *function to output arrival from task thread
 *IMPORT: a task
 */
void taskThreadStats(process *task)
{
    int j = 0;
    char str[200];

    //string concatanation
    j = sprintf(str, "#: %d | Burst: %d\n", task->task_id, task->cpu_burst);
    j += sprintf(str + j, "Arrival time: %d:%d:%d\n\n", task->arrive_hr, task->arrive_min, task->arrive_sec);

    //print to terminal
    printf("%s", str);

    //write to file using func from fileIO.c
    writeToFile(simLog, str);

}

/*
 *function to output service and completion stats of tasks of cpu thread
 *IMPORT: a task, name of cpu and status(this is a preprocessor directive SERVING or DONE)
 *        find the preprocessor directive in scheduler.h
 */
void cpuThreadStats(process *task, char *cpu_name, int status)
{
    int j = 0;
    char str[200];

    //string concatanation
    j = sprintf(str, "Statistics for %s:\n", cpu_name);
    j += sprintf(str + j, "#: %d | Burst: %d\n", task->task_id, task->cpu_burst);
    j += sprintf(str + j, "Arrival time: %d:%d:%d\n", task->arrive_hr, task->arrive_min, task->arrive_sec);

    if(status == SERVING)
    {
    	sprintf(str + j, "Service time: %d:%d:%d\n\n", task->serv_hr, task->serv_min, task->serv_sec);
    }
    else if(status == DONE)
    {
    	sprintf(str + j, "Completion time: %d:%d:%d\n\n", task->comp_hr, task->comp_min, task->comp_sec);
    }

    //print to screen
    printf("%s", str);

    //write to log using func from fileIO.c
    writeToFile(simLog, str);

}

/*
 *function to calculate total wait time in seconds
 *IMPORTS: task, hour, minutes and seconds
 */
void calcWaitTime(process *inTask)
{
    float arrival = 0.0, serv_time = 0.0;

    //convert all parts of time to seconds for wait time calculations
    serv_time = (float)((inTask->serv_hr * 3600) + (inTask->serv_min * 60) + inTask->serv_sec);
    arrival = (float)((inTask->arrive_hr * 3600) + (inTask->arrive_min * 60) + inTask->arrive_sec);

    total_waiting_time += serv_time - arrival; //global var total waiting time in seconds
}

/*
 *function to calculate total turn around time in seconds
 *IMPORTS: task, hour, minutes and seconds
 */
void calcTurnAround(process *inTask)
{
    float arrival = 0.0, comp_time = 0.0, result = 0.0;

    //All time measurements in seconds for turn around time calculations
    comp_time = (float)((inTask->serv_hr * 3600) + (inTask->serv_min * 60) + inTask->serv_sec) + (float)(inTask->cpu_burst);
    arrival = (float)((inTask->arrive_hr * 3600) + (inTask->arrive_min * 60) + inTask->arrive_sec);

    result = comp_time - arrival;
    total_turnaround_time += result; //global var total waiting time in seconds
}

/*
 *function to print out overall task time statistics after scheduler done
 */
void taskTimeStats()
{
    float avg_wait = 0.0, avg_TAT = 0.0;

    avg_wait = total_waiting_time / num_tasks;
    avg_TAT = total_turnaround_time / num_tasks;

    printf("\nNumber of tasks: %d\n", (int)num_tasks);
    printf("Average waiting time: %.2f seconds\n", avg_wait);
    printf("Average turn around time: %.2f seconds\n", avg_TAT);

    //fileIO.c write stats to file
    writeTaskTimeStats(simLog, num_tasks, avg_wait, avg_TAT);
}