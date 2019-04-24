/*
 * file dealing with file io
 * by Kei Sum Wang, student id: 19126089
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fileIO.h"

#define MAX_CHARS 100

//TODO tweak this further to suit OS assignment

/*
 * processFile
 * IMPORTS: string fileName, and a pointer to LinkedList.
 * PURPOSE: reads
 */
int readFile(char *fileName, Queue *q)
{
    FILE *file = fopen(fileName, "r");
    char str[MAX_CHARS];
    process* task = NULL;
    
    if (file == NULL)
    {
        printf("\n");
        perror("Error writing to file");
        return -1;
    }
    else
    {
        while (fgets(str, MAX_CHARS, file) != NULL)
        {
            task = (process*)malloc(sizeof(process));
            sscanf(str, "%3d %2d", &(task->task_id), &(task->cpu_burst));/*get task id and cpu burst*/
             
            enqueue(q, task);/*insert last into queue for ascending order*/        
           
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
 * PURPOSE: writes content
 */
void writeToFile(char *fileName)
{
    FILE *file = fopen(fileName, "w");
    int i;


    if (file == NULL)
    {
        printf("\n");
        perror("Error writing to file");
    }
    else
    {
        for (i = 0; i < 10; i++)
        {
            /*hours and minutes should only have 2 digits,
              if it is single digit then just add zero
              at the front*/ 
            fprintf(file, "sssssssssssss");
        }

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

}



