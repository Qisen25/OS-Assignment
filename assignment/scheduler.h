/**
 *Scheduler head file
 *AUTHOR Kei Sum Wang, id: 19126089
 *
 **/

#pragma once

#define SERVING 1
#define DONE 2

#include "fileIO.h"
#include "Queue.h"

typedef struct
{
	int task_id;
	int cpu_burst;
	int arrive_hr, arrive_min, arrive_sec;	
	int serv_hr, serv_min, serv_sec;
	int comp_hr, comp_min, comp_sec;
}process;



void* task(void* arg);
void* cpu(void *arg);
void getTime(int *hr, int *min, int *sec);
void calcWaitTime(process *inTask);
void calcTurnAround(process *inTask);
void taskThreadStats(process *task);
void cpuThreadStats(process *task, char *cpu_name, int status);
void taskTimeStats();
int criticalNotEmpty();
