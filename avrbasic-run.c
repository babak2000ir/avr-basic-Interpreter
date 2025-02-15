#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "avrbasic.h"
#include "avrbasic-tokenizer.h"

// Lists of reserved words
const char *commands[32] = {"END", "CLEAR", "NEXT", "WEND", "DO", "GOTO", "WHILE", "UNTIL", "PRINT", "INPUT",
                            "LET", "POKE", "FOR", "IF", "PEEK", "ABS", "SGN", "INT", "SQR", "EXP", "LOG", "LOG10",
                            "SIN", "COS", "DEG", "RAD", "NOT", "SHL", "SHR", "TO", "THEN", "ELSE"};

const char *operators[16] = {"=", "<>", "<", ">", "<=", ">=", "+", "-", "*", "/", "AND", "OR", "XOR", "MOD", ",", "^"};

// Functions to check if the token is a reserved word
void toUppercase(char str[])
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = toupper(str[i]); // Convert each character
    }
}

int is_command(char *token)
{
    toUppercase(token);
    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
    {
        if (strcmp(token, commands[i]) == 0)
        {
            return 1; // Match found
        }
    }
    return 0; // No match found
}

int is_operator(char *token)
{
    toUppercase(token);
    for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++)
    {
        if (strcmp(token, operators[i]) == 0)
        {
            return 1; // Match found
        }
    }
    return 0; // No match found
}

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
    tokenize();
    printTable();
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
    // executeProgram();
    return 0;
}

Variable *findVariable(char name)
{
    for (int i = 0; i < variableCount; i++)
    {
        if (variables[i].name == name)
        {
            return &variables[i];
        }
    }
    return NULL;
}

void setVariable(char name, int value)
{
    Variable *var = findVariable(name);
    if (var == NULL)
    {
        if (variableCount < MAX_VARIABLES)
        {
            variables[variableCount].name = name;
            variables[variableCount].value = value;
            variableCount++;
        }
        else
        {
            printf("Variable limit exceeded\n");
        }
    }
    else
    {
        var->value = value;
    }
}

int getVariableValue(char name)
{
    Variable *var = findVariable(name);
    return var ? var->value : 0;
}

/* void executeProgram()
{
    for (int i = 0; i < programLength; i++)
    {
        ProgramLine line = program[i];
        if (strcmp(line.command, "LET") == 0)
        {
            int value = atoi(line.param2);
            setVariable(line.param1[0], value);
        }
        else if (strcmp(line.command, "PRINT") == 0)
        {
            int value = getVariableValue(line.param2[0]);
            printf("%d\n", value);
        }
        else if (strcmp(line.command, "IF") == 0)
        {
            int left = getVariableValue(line.param1[0]);
            int right = getVariableValue(line.param2[0]);
            int condition = 0;
            if (strcmp(line.operator, "=") == 0)
                condition = (left == right);
            else if (strcmp(line.operator, "<>") == 0)
                condition = (left != right);
            else if (strcmp(line.operator, "<") == 0)
                condition = (left < right);
            else if (strcmp(line.operator, ">") == 0)
                condition = (left > right);
            else if (strcmp(line.operator, "<=") == 0)
                condition = (left <= right);
            else if (strcmp(line.operator, ">=") == 0)
                condition = (left >= right);

            if (condition)
            {
                // Handle THEN and ELSE parts
            }
        }
        // Add more command handling here
    }
} */
