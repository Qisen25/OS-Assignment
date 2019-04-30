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

int bufferLimit, fileSize, taskThread_ran = 0;

pthread_cond_t full, empty;//conditions for wait and signaling
pthread_mutex_t mutex, queue_mutex;

//shared buffers
Queue* readyQueue;
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

    fileSize = fromFile->length;

    wipeLog(simLog);

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&queue_mutex, NULL);
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

    printf("%d flength begin\n", fromFile->length);
    printf("%d rlength begin\n", readyQueue->length);

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
	int now_hr = 0, now_min = 0, now_sec = 0, i, spaceLeft, concat;
	char summary[200];

	process *task1, *task2 ;

	while(!queueEmpty(fromFile))
	{
		

	    pthread_mutex_lock(&mutex);//lock thread to prevent to incorrect processing


	    task1 = dequeue(fromFile);
	    printf("task inputting\n\n");
	    taskThread_ran++;
	   

	    while(readyQueue->length == bufferLimit)
		{
			//printf("waiting for cpu to consume %d length \n", readyQueue->length);
			pthread_cond_wait(&empty, &mutex);
		}

		//critical section for queueing
    	getTime(&(task1->arrive_hr), &(task1->arrive_min), &(task1->arrive_sec));//get current time
        enqueue(readyQueue, task1);

		taskThreadStats(task1);

		//calculate the space left in ready queue
		spaceLeft = bufferLimit - readyQueue->length;
		/*buffer size must be greater 1 in order to get 2 tasks
		 *file cannot be empty after task1 dequeue
		 *space left must be greater than 0 or otherwise ready queue 1 task above buffer limit 
		 */
		if(bufferLimit > 1 && !queueEmpty(fromFile) && spaceLeft > 0)
		{
			printf("********adding second task*************\n");
			task2 = dequeue(fromFile);
			getTime(&(task2->arrive_hr), &(task2->arrive_min), &(task2->arrive_sec));//get current time
	        enqueue(readyQueue, task2);

			// printf("\n#: %d | Burst: %d\n", task2->task_id, task2->cpu_burst);
			// printf("Arrival time: %d:%d:%d\n", task2->arrive_hr, task2->arrive_min, task2->arrive_sec);
			taskThreadStats(task2);
		}

		
		pthread_cond_signal(&full);//signal that processes are available in readyqueue
	    pthread_mutex_unlock(&mutex);//unlock mutex after done enqueueing

	    printf("%d curr ready queue size \n", readyQueue->length);

	}

	/*Since there are 3 cpus, 1 cpu may block forever if there less than 3 tasks from file
	 *task thread cannot signal in critical once done, so loop to release any cpu blocked
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

	//**check critical_notEmpty to see the mutex lock**
	//Run if either ready is not empty or file queue still contains tasks.
	while(criticalNotEmpty())
	{
		// unlock if condition above is true
		//pthread_mutex_lock(&mutex);

		printf("%s begin\n", cpu_name);
		printf("%d flength begin %s\n", fromFile->length, cpu_name);
		printf("%d rlength begin %s\n", readyQueue->length, cpu_name);
		getTime(&now_hr, &now_min, &now_sec);
		printf("begin time: %d:%d:%d\n", now_hr, now_min, now_sec);


		printf("\n********ready queue remove %s********\n", cpu_name);
		task = dequeue(readyQueue);
		
		printf("*******%d %s curr ready queue size after deq*********\n", readyQueue->length, cpu_name);
		getTime(&(task->serv_hr), &(task->serv_min), &(task->serv_sec));
		calcWaitTime(task);
		calcTurnAround(task);
		cpuThreadStats(task, cpu_name, SERVING);


		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&mutex);		

		//remainder section aka consumin
printf("********CONSUMING %s********\n", cpu_name);
		sleep(task->cpu_burst);
		getTime(&(task->comp_hr), &(task->comp_min), &(task->comp_sec));
		task_counter++;
		num_tasks++;
		//printf("in %d******************", readyQueue->length);//test
		// printf("\nStatistics for %s:\n", cpu_name);
		// printf("Task %d\n", task->task_id);
		// printf("Arrival time: %d:%d:%d\n", task->arrive_hr, task->arrive_min, task->arrive_sec);
		// printf("Completion time: %d:%d:%d\n", task->comp_hr, task->comp_min, task->comp_sec);
		cpuThreadStats(task, cpu_name, DONE);

		
		free(task);


	}


	//if(task_counter == 0)
	//pthread_mutex_unlock(&mutex);

	//printf("file %d************\n", fromFile->length);
	//printf("ready %d************\n", readyQueue->length);

	printf("***CPU Termination*****\n");
	sprintf(summary,"\n%s terminates after servicing %d tasks\n", cpu_name, task_counter);//todo make this proper
    //pthread_exit(NULL);// exit for consumer makes main hang do not use
    printf("%s", summary);
    writeToFile(simLog, summary);
    return NULL;
}

/*
 *criticalNotEmpty function checks if cpu can enter the critical section
 *only if ready queue is not empty and file queue still has task
 *EXPORT: integer flag indicating ready and file queue is empty or not
 */
int criticalNotEmpty()
{
	int isEmpty = 0;

	pthread_mutex_lock(&mutex);	
	
	//ready queue is empty but file queue still has tasks wait for task thread
	while(queueEmpty(readyQueue) && !queueEmpty(fromFile))
	{
		//printf("%s waiting for readyqueue\n", cpu_name);
		//getTime(&now_hr, &now_min, &now_sec);
		//printf("begin time: %d:%d:%d\n", now_hr, now_min, now_sec);
		pthread_cond_wait(&full, &mutex);
	}

	
	isEmpty = (!queueEmpty(readyQueue)); //|| !queueEmpty(fromFile));

	if(queueEmpty(readyQueue) && queueEmpty(fromFile))
	{
		pthread_mutex_unlock(&mutex);	
	}

	return isEmpty;
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
 *function to output service and completion of tasks from cpu thread
 *IMPORT: a task, name of cpu and status(this is a preprocessor directive SERVING or DONE)
 */
void cpuThreadStats(process *task, char *cpu_name, int status)
{
	int j = 0;
	char str[200];

	//string concatanation
	j = sprintf(str, "\nStatistics for %s:\n", cpu_name);
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

	//write to file using func from fileIO.c
	writeToFile(simLog, str);

}

/*
 *function to calculate total wait time
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
 *function to calculate total turn around time
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

	//fileIO.c write stats to file
	writeTaskTimeStats(simLog, num_tasks, avg_wait, avg_TAT);
}


