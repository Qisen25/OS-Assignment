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
#include "fileIO.h"
#include "Queue.h"

float num_tasks = 0.0, total_waiting_time = 0.0, total_turnaround_time = 0.0; 

int bufferSize, fileSize;

pthread_cond_t full, empty;//conditions for wait and signaling
pthread_mutex_t mutex;

//shared buffers
Queue* readyQueue;
Queue* fromFile;//file content are stored in queue

pthread_t task_thread, cpu1, cpu2, cpu3;
char *name1 = "CPU-1", *name2 = "CPU-2", *name3 = "CPU-3"; //names of cpus

/*
 *MAIN
 *IMPORT: take in a count for # args, filename and size for ready queue
 */
int main(int argc, char *argv[])
{
	bufferSize = atoi(argv[2]);

    if(argc < 3 || (bufferSize < 1 || bufferSize > 10))
    {
        printf("Incorrect arguments\nCheck readme for argument details\n");
        return -1;
    }

    //initilization of containers
    fromFile = createQueue();
    readyQueue = createQueue();

    if(readFile(argv[1], fromFile) != 0)
   	{
   		return -1;//unsuccessful read should end
   	}

    fileSize = fromFile->length;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&full, NULL);
    pthread_cond_init(&empty, NULL);

    //task thread
    pthread_create(&task_thread, NULL, &task, NULL);

    //cpu threads
    pthread_create( &cpu1, NULL, &cpu, (void*) name1);
    pthread_create(&cpu2, NULL, &cpu, (void*) name2);
    pthread_create(&cpu3, NULL, &cpu, (void*) name3);

    pthread_join(task_thread, NULL);
    pthread_join(cpu1, NULL);
    pthread_join(cpu2, NULL);
    pthread_join(cpu3, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&full);
    pthread_cond_destroy(&empty);

    freeQueue(fromFile, 0);
    freeQueue(readyQueue, 0);

    taskTimeStats();

    return 0;
}

/**
 *task method
 *PURPPOSE: used by the task thread to add tasks to ready queue
 */
void* task(void *arg)
{
	int now_hr = 0, now_min = 0, now_sec = 0;

	process *task1, *task2 ;

	while(!queueEmpty(fromFile))
	{
		task1 = dequeue(fromFile);

	    pthread_mutex_lock(&mutex);//lock thread to prevent to incorrect processing

	    while(readyQueue->length == bufferSize)
		{
			//printf("waiting for cpu to consume %d length \n", readyQueue->length);
			pthread_cond_wait(&empty, &mutex);
		}

		//critical section for queueing
	    	//TODO this is currently pseudocode, do real implement soon
		//if(readyQueue->length == bufferSize - 1)
	    //{
	    	//printf("***Queueing task*****\n");
	    	getTime(&(task1->arrive_hr), &(task1->arrive_min), &(task1->arrive_sec));//get current time
	        enqueue(readyQueue, task1);
 
			printf("\n#: %d | Burst: %d\n", task1->task_id, task1->cpu_burst);
    		printf("Arrival time: %d:%d:%d\n", task1->arrive_hr, task1->arrive_min, task1->arrive_sec);

	
	    
	    //}
/*	    else if(readyQueue->length < bufferSize)
	    {
	    	printf("f2nd if");

	    	//get data from file queue
	    	task1 = dequeue(fromFile);
	    	task2 = dequeue(fromFile);

	    	//add to ready queue
	        enqueue(readyQueue, task1);
	        printf("#: %d | Burst: %d\n", task1->task_id, task1->cpu_burst);
	        printf("Arrival time: %d:%d:%d\n", hr, min, sec);

	        enqueue(readyQueue, task2);
	        printf("#: %d | Burst: %d\n", task2->task_id, task2->cpu_burst);
	        printf("Arrival time: %d:%d:%d\n", hr, min, sec);
	    }*/



	    pthread_mutex_unlock(&mutex);//unlock mutex after done enqueueing
	    //todo create if cond here

	    //if(readyQueue->length == bufferSize)
	    //{
	    	pthread_cond_signal(&full);//signal that processes are available in readyqueue
   		//}
	        

	}

	//pthread_cond_broadcast(&full);
	printf("\nNumber of tasks put into Ready-Queue: %d\n", fileSize);
	getTime(&now_hr, &now_min, &now_sec);
	printf("Terminate at time: %d:%d:%d\n",  now_hr, now_min, now_sec);
	//pthread_exit(NULL);
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
	char *cpu_name = (char*)arg;//get cpu thread name
	process* task;

	task_counter = 0;

	//while(readyQueue->length > 0 || fromFile->length > 0)
	//run if either ready is not empty or file queue still contains tasks
	while(!queueEmpty(readyQueue) || !queueEmpty(fromFile))
	{
		//printf("%s begin\n", cpu_name);
		//getTime(&now_hr, &now_min, &now_sec);
		//printf("begin time: %d:%d:%d\n", now_hr, now_min, now_sec);
		//unlock if condition above is true
		pthread_mutex_lock(&mutex);
		//readyQueue is empty wait for tasks to be queued
		//if tasks from file are still available
		
		while(queueEmpty(readyQueue) && !queueEmpty(fromFile))
		{
			//printf("%s waiting for readyqueue\n", cpu_name);
			//getTime(&now_hr, &now_min, &now_sec);
			//printf("begin time: %d:%d:%d\n", now_hr, now_min, now_sec);
			pthread_cond_wait(&full, &mutex);
		}

		//critical section for getting task

		task = dequeue(readyQueue);

		if(task != NULL)
		{
			getTime(&now_hr, &now_min, &now_sec);
			calcWaitTime(task, now_hr, now_min, now_sec);
			calcTurnAround(task, now_hr, now_min, now_sec);
			printf("\nStatistics for %s:\n", cpu_name);
			printf("Task %d\n", task->task_id);
			printf("Arrival time: %d:%d:%d\n", task->arrive_hr, task->arrive_min, task->arrive_sec);
			printf("Service time: %d:%d:%d\n", now_hr, now_min, now_sec);

		}


		pthread_cond_signal(&empty);
		//}

		pthread_mutex_unlock(&mutex);


		//remainder section aka consumin

		//pthread_mutex_lock(&mutex);
		if(task != NULL)
		{
			sleep(task->cpu_burst);
			getTime(&now_hr, &now_min, &now_sec);
			task_counter++;
			num_tasks++;
			//printf("in %d******************", readyQueue->length);//test
			printf("\nStatistics for %s:\n", cpu_name);
			printf("Task %d\n", task->task_id);
			printf("Arrival time: %d:%d:%d\n", task->arrive_hr, task->arrive_min, task->arrive_sec);
			printf("Completion time: %d:%d:%d\n", now_hr, now_min, now_sec);
			
			free(task);
		}

	}	


	//printf("file %d************\n", fromFile->length);
	//printf("ready %d************\n", readyQueue->length);

	printf("***CPU Termination*****\n");
	printf("%s terminates after servicing %d tasks\n", cpu_name, task_counter);//todo make this proper
    //pthread_exit(NULL);// exit for consumer makes main hang do not use
    return NULL;
}

/*
 *function to get time on system
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
 *function to calculate total wait time
 *IMPORTS: task, hour, minutes and seconds
 */
void calcWaitTime(process *inTask, int hr, int min, int sec)
{
	float arrival = 0.0, serv_time = 0.0;

	//convert all parts of time to seconds for wait time calculations
	serv_time = (float)((hr * 3600) + (min * 60) + sec);
	arrival = (float)((inTask->arrive_hr * 3600) + (inTask->arrive_min * 60) + inTask->arrive_sec);

	total_waiting_time += serv_time - arrival; //global var waiting time in seconds
}

/*
 *function to calculate total turn around time
 *IMPORTS: task, hour, minutes and seconds
 */
void calcTurnAround(process *inTask, int hr, int min, int sec)
{
	float arrival = 0.0, comp_time = 0.0, result = 0.0;

	//All time measurements in seconds for turn around time calculations
	comp_time = (float)((hr * 3600) + (min * 60) + sec) + (float)(inTask->cpu_burst);
	arrival = (float)((inTask->arrive_hr * 3600) + (inTask->arrive_min * 60) + inTask->arrive_sec);

	result = comp_time - arrival;
	total_turnaround_time += result; //global var waiting time in seconds

	//printf("turn around time %.2f\n", result); remove soon
	//printf("comp time %.2f\n", comp_time); remove soon
}

/*
 *function to print out overall task time statistics
 */
void taskTimeStats()
{
	float avg_wait = 0.0, avg_TAT = 0.0;
	
	avg_wait = total_waiting_time / num_tasks;
	avg_TAT = total_turnaround_time/ num_tasks;

	printf("\nNumber of tasks: %d\n", (int)num_tasks);
	printf("Average waiting time: %.2f seconds\n", avg_wait);
	printf("Average turn around time: %.2f seconds\n", avg_TAT);
}