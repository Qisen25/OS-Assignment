/*
 * file dealing with file io
 * AUTHOR: Kei Sum Wang, student id: 19126089
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fileIO.h"

#define MAX_CHARS 100


/*
 * processFile
 * IMPORTS: string fileName, and a queue.
 * PURPOSE: reads in task file and stores tasks in a struct, adds tasks into a queue
 */
int readFile(char *fileName, Queue *q)
{
    FILE *file = fopen(fileName, "r");
    char str[MAX_CHARS];
    int check1, check2;
    process* task = NULL;
    
    if (file == NULL)
    {
        printf("\n");
        perror("Error reading");
        return -1;
    }
    else
    {
        while (fgets(str, MAX_CHARS, file) != NULL)//read line by line
        {
            sscanf(str, "%d %d", &check1, &check2);
            if(check1 > 0 && check2 > 0)//id and burst are (+)ve
            {
                task = (process*)malloc(sizeof(process));//alloc memory for task
                sscanf(str, "%d %d", &(task->task_id), &(task->cpu_burst));/*get task id and cpu burst*/
                 
                enqueue(q, task);/*insert last into queue for ascending order*/    
            }
            else
            {
               printf("Error reading: id and burst time must be positive"); 
               fclose(file);
               return -1;
            }    
           
        }

        if (ferror(file))/*check if any error when read*/
        {
                printf("Error reading file\n");
        }

        fclose(file);
    }

    return 0;
}

/*
 * writeToFile
 * IMPORTS: string fileName
 * PURPOSE: appends str to file
 */
int writeToFile(char *fileName, char *str)
{
    FILE *file = fopen(fileName, "a");

    if (file == NULL)
    {
        printf("\n");
        perror("Error writing to file");
        return -1;
    }
    else
    {

        fprintf(file, "%s", str);

        if (ferror(file))
        {
            printf("Error writing to file");/*if error found, print error*/
        }

        fclose(file);
    }
    return 0;
}

/*
 * writeTaskStats
 * IMPORTS: string fileName, num of task, avg wait time and avg turn around
 * PURPOSE: appends time statistics of process simulation
 */
int writeTaskTimeStats(char *fileName, float num_tasks, float avg_wait, float avg_TAT)
{
    FILE *file = fopen(fileName, "a");

    if (file == NULL)
    {
        printf("\n");
        perror("Error writing to file");
        return -1;
    }
    else
    {

        fprintf(file,"Number of tasks: %d\n", (int)num_tasks);
        fprintf(file, "Average waiting time: %.2f seconds\n", avg_wait);
        fprintf(file, "Average turn around time: %.2f seconds\n", avg_TAT);

        if (ferror(file))
        {
            printf("Error writing to file");/*if error found, print error*/
        }
        else
        {
            printf("\nWrite to file: %s success\n", fileName);/*print success if no error found when writing*/
        }

        fclose(file);
    }

    return 0;
}

/*
 *function to clear contents of file
 *IMPORT: file name
 */
int wipeLog(char *fileName)
{
    fclose(fopen(fileName, "w"));

    return 0;
}



