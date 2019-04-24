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

double num_tasks, total_waiting_time, total_turnaround_time; 
int now_hr, now_min, now_sec;//global variables, current local time
int bufferSize, fileSize;

pthread_cond_t full, empty;
pthread_mutex_t mutex;

Queue* readyQueue;
Queue* fromFile;

pthread_t task_thread, cpu1, cpu2, cpu3;
char *name1 = "CPU-1", *name2 = "CPU-2", *name3 = "CPU-3"; //names of cpus


int main(int argc, char *argv[])
{
	process *theTask;
    if(argc < 3)
    {
        printf("Not enough arguments\nCheck readme for argument details\n");
        return -1;
    }

    bufferSize = atoi(argv[2]);

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

    //theTask = peekFirst(readyQueue);

    //printf("taskid %d", theTask->task_id);

    freeQueue(fromFile, 0);
    freeQueue(readyQueue, 0);
    return 0;
}

/**
 *task method
 *PURPPOSE: used by the task thread to add tasks to ready queue
 *IMPORTS: m (capacity of tasks in ready queue)
 */
void* task(void *arg)
{

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

	    if(readyQueue->length == bufferSize)
	    {
	    	pthread_cond_signal(&full);//signal that processes are available in readyqueue
   		}

	    pthread_mutex_unlock(&mutex);//unlock mutex after done enqueueing
	    //todo create if cond here

	        

	}

	printf("\nNumber of tasks put into Ready-Queue: %d\n", fileSize);
	getTime(&now_hr, &now_min, &now_sec);
	printf("Terminate at time: %d:%d:%d\n",  now_hr, now_min, now_sec);
	pthread_exit(NULL);

}

/**
 *cpu
 *PURPOSE: To perform CPU operations
 */
void* cpu(void *arg)
{
	int task_counter;
	char *cpu_name = (char*)arg;
	process* task;

	task_counter = 0;
	pthread_mutex_lock(&mutex);//lock to prevent possible race conditions
	while(readyQueue->length > 0 || fromFile->length > 0)
	{

		

		//readyQueue is empty wait for tasks to be queued,fileQueue should not be empty
		while(queueEmpty(readyQueue) && !queueEmpty(fromFile))
		{
			printf("waiting for readyqueue\n");
			pthread_cond_wait(&full, &mutex);
		}



		//critical section getting task
		/*prevent a thread from dequeuing
		*if the ready queue is already empty
		*issues occur when there are less tasks
		*from file than # of cpus
		*/
		//if(!queueEmpty(readyQueue))
		//{
			task = dequeue(readyQueue);

				
		

		//printf("***Consuming task%d*****\n", task->task_id);

		//if(readyQueue->length == 0)
		//{
		
		
			
			pthread_mutex_unlock(&mutex);


			//remainder section aka consuming

			//pthread_mutex_lock(&mutex);
			sleep(task->cpu_burst);
			getTime(&now_hr, &now_min, &now_sec);
			task_counter++;
			printf("in %d******************", readyQueue->length);//test
			printf("\nStatistics for %s:\n", cpu_name);
			printf("Task %d\n", task->task_id);
			printf("Arrival time: %d:%d:%d\n", task->arrive_hr, task->arrive_min, task->arrive_sec);
			printf("Completion time: %d:%d:%d\n", now_hr, now_min, now_sec);
			
			free(task);

			if(queueEmpty(readyQueue))
			{
				pthread_cond_signal(&empty);
			}
			//pthread_mutex_unlock(&mutex);
		//}

		//pthread_cond_signal(&empty);
		
		printf("out if %d************\n", readyQueue->length);
	}   
	pthread_mutex_unlock(&mutex);
	printf("***CPU Termination*****\n");
	printf("%s terminates after servicing %d tasks\n", cpu_name, task_counter);//todo make this proper
    //pthread_exit(NULL);// exit for consumer makes main hang do not use
    return NULL;
}

void getTime(int *hr, int *min, int *sec)
{
	time_t now;

	time(&now);

	struct tm *curTime = localtime(&now);

	*hr = curTime->tm_hour;
	*min = curTime->tm_min;
	*sec = curTime->tm_sec;
}