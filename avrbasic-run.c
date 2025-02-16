#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "avrbasic.h"
#include "avrbasic-tokenizer.h"

void printTable()
{
    printf("%-5s %-10s %-10s %-10s %-5s %-10s | %-10s %-10s %-10s %-10s\n",
           "Line", "ReturnVal", "Command", "Param1", "Opr", "Param2",
           "ExtCmd1", "ExtParam1", "ExtCmd2", "ExtParam2");
    printf("--------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < programLineCount; i++)
    {
        printf("%-5d %-10s %-10s %-10s %-5s %-10s | %-10s %-10s %-10s %-10s\n",
               program[i].lineNumber,
               program[i].returnValue ? program[i].returnValue : "NULL",
               program[i].command ? program[i].command : "NULL",
               program[i].param1 ? program[i].param1 : "NULL",
               program[i].opr ? program[i].opr : "NULL",
               program[i].param2 ? program[i].param2 : "NULL",
               program[i].extension.command1 ? program[i].extension.command1 : "NULL",
               program[i].extension.param1 ? program[i].extension.param1 : "NULL",
               program[i].extension.command2 ? program[i].extension.command2 : "NULL",
               program[i].extension.param2 ? program[i].extension.param2 : "NULL");
    }
}

void loadProgram(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Failed to open file");
        exit(1);
    }

    char buffer[MAX_LINE_LEN];
    while (fgets(buffer, sizeof(buffer), file))
    {
        fileLines[programLineCount++] = strdup(buffer);
    }
    fclose(file);
    
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <program file>\n", argv[0]);
        return 1;
    }

    int lineNo = 0;
    loadProgram(argv[1]);
    tokenize();
    printTable();
    executeProgram();
    return 0;
}




