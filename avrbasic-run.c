#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "avrbasic.h"
#include "avrbasic-tokenizer.h"

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
    executeProgram();
    return 0;
}




