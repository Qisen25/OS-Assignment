/*
 * file header file
 */
#pragma once

#include "Queue.h"
#include "scheduler.h"

int readFile(char *fileName, Queue *q);
int writeToFile(char *fileName, char *str);
int writeTaskTimeStats(char *fileName, float num_tasks, float avg_wait, float avg_TAT);
int wipeLog(char *fileName);
