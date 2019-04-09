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
void readFile(char *fileName, LinkedList *list)
{
    FILE *file = fopen(fileName, "r");
    char str[MAX_CHARS];
    
    if (file == NULL && validFormat != TRUE)/*if file does not exist, it is also not valid*/
    {
        perror("Error reading file");
    }
    else
    { 
        while (fgets(str, MAX_CHARS, file) != NULL)
        {
            sscanf(str, "%3d:%2d", &(show->time.hours), &(show->time.mins));/*get task id and cpu burst*/

            fgets(str, MAX_CHARS, file);/*read line after to get arrival time */
            sscanf(str, "%2d:%2d:%2d", &(show->time.hours), &(show->time.mins),  &(show->time.secs));/*store hours mins and secs*/

         
            enqueue(list, show);/*insert last into queue for ascending order*/        
       
        }

        if (ferror(file))/*check if any error when read*/
        {
            printf("Error reading file\n");
        }

        fclose(file);
    }

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
    else if (length < 1)/*at least 1 show should be in guide in order to write to file*/
    {
        printf("nothing to write to file: %s\n", fileName);
        fclose(file);
    }
    else
    {
        for (i = 0; i < length; i++)
        {
            /*hours and minutes should only have 2 digits,
              if it is single digit then just add zero
              at the front*/ 
            fprintf(file, "%02d:%02d - %s", guide[i]->time.hours, guide[i]->time.mins, guide[i]->name);
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


}
