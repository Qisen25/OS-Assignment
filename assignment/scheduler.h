/**
 *Scheduler head file
 *AUTHOR Kei Sum Wang, id: 19126089
 *
 **/

#pragma once

#include "fileIO.h"
#include "Queue.h"

typedef struct
{
	int task_id;
	int cpu_burst;
	int arrive_hr, arrive_min, arrive_sec;	
}process;



void* task(void* arg);
void* cpu(void *arg);
void getTime(int *hr, int *min, int *sec);
void calcWaitTime(process *inTask, int hr, int min, int sec);
void calcTurnAround(process *inTask, int hr, int min, int sec);
void taskTimeStats();
int criticalNotEmpty();
