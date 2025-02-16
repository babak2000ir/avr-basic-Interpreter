#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "avrbasic.h"
#include "avrbasic-exec.h"

Variable *findVariable(char *name)
{
    for (int i = 0; i < variableCount; i++)
    {
        if (strcmp(variables[i].name, name) == 0)
        {
            return &variables[i];
        }
    }
    return NULL;
}

void setVariable(char *name, int value)
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

int getVariableValue(char *name)
{
    Variable *var = findVariable(name);
    return var ? var->value : 0;
}

int findProgramLine(int lineNumber)
{
    for (int i = 0; i < programLineCount; i++)
    {
        if (program[i].lineNumber == lineNumber)
        {
            return i;
        }
    }
    fprintf(stderr, "Error: Line %d not found\n", lineNumber);
    return 0;
}

int programLineCounter = 0;

int gotoLine(char *param, TokenType paramType)
{
    int lineNo;
    if (paramType == TOKEN_VARIABLE)
    {
        lineNo = getVariableValue(param);
    }
    else
    {
        lineNo = atoi(param);
    }
    return findProgramLine(lineNo) - 1;
}

int getParamValue(char *param, TokenType paramType)
{
    if (paramType == TOKEN_VARIABLE)
    {
        return getVariableValue(param);
    }
    else
    {
        return atoi(param);
    }
}

void executeProgram()
{
    while (programLineCounter < programLineCount)
    {
        ProgramLine line = program[programLineCounter];
        if (strcmp(line.command, "END") == 0)
        {
            printf("Program ended\n");
            return;
        }
        else if (strcmp(line.command, "CLEAR") == 0)
        {
            // set all variables to zero
            for (int j = 0; j < variableCount; j++)
            {
                variables[j].value = 0;
            }
        }
        else if (strcmp(line.command, "FOR") == 0) // Deploy
        {
            printf("Next line\n");
        }
        else if (strcmp(line.command, "WHILE") == 0) // Deploy
        {
            printf("Wend line\n");
        }
        else if (strcmp(line.command, "DO") == 0) // Deploy
        {
            printf("Do line\n");
        }
        else if (strcmp(line.command, "GOTO") == 0)
        {
            programLineCounter = gotoLine(line.param1, line.param1Type);
        }
        else if (strcmp(line.command, "WEND") == 0)
        {
            printf("While line\n");
        }
        else if (strcmp(line.command, "UNTIL") == 0)
        {
            printf("Until line\n");
        }
        else if (strcmp(line.command, "PRINT") == 0)
        {
            printf("%d\n", getParamValue(line.param1, line.param1Type));
        }
        else if (strcmp(line.command, "INPUT") == 0)
        {
            if (line.param1Type == TOKEN_VARIABLE)
            {
                int value;
                printf("?");
                scanf("%d", &value);
                setVariable(line.param1, value);
            }
            else
            {
                fprintf(stderr, "Error: INPUT command requires a variable name\n");
            }
        }
        else if (strcmp(line.command, "LET") == 0)
        {
            if (line.param1Type == TOKEN_VARIABLE)
            {
                if (line.param2Type == TOKEN_VARIABLE)
                {
                    setVariable(line.param1, getVariableValue(line.param2));
                }
                else
                {
                    setVariable(line.param1, atoi(line.param2));
                }
            }
            else
            {
                fprintf(stderr, "Error: LET command requires a variable name\n");
            }
        }
        else if (strcmp(line.command, "POKE") == 0) // Deploy
        {
            printf("Poke line\n");
        }
        else if (strcmp(line.command, "NEXT") == 0) // Deploy
        {
            printf("For line\n");
        }
        else if (strcmp(line.command, "IF") == 0)
        {
            int p1 = getParamValue(line.param1, line.param1Type);
            int p2 = getParamValue(line.param2, line.param2Type);

            if (strcmp(line.opr, "=") == 0)
            {
                if (p1 == p2)
                {
                    programLineCounter = gotoLine(line.extension.param1, line.extension.param1Type);
                }
                else
                {
                    programLineCounter = gotoLine(line.extension.param2, line.extension.param2Type);
                }
            }
            else if (strcmp(line.opr, "<>") == 0)
            {
                if (p1 != p2)
                {
                    programLineCounter = gotoLine(line.extension.param1, line.extension.param1Type);
                }
                else
                {
                    programLineCounter = gotoLine(line.extension.param2, line.extension.param2Type);
                }
            }
            else if (strcmp(line.opr, "<") == 0)
            {
                if (p1 < p2)
                {
                    programLineCounter = gotoLine(line.extension.param1, line.extension.param1Type);
                }
                else
                {
                    programLineCounter = gotoLine(line.extension.param2, line.extension.param2Type);
                }
            }
            else if (strcmp(line.opr, ">") == 0)
            {
                if (p1 > p2)
                {
                    programLineCounter = gotoLine(line.extension.param1, line.extension.param1Type);
                }
                else
                {
                    programLineCounter = gotoLine(line.extension.param2, line.extension.param2Type);
                }
            }
            else if (strcmp(line.opr, "<=") == 0)
            {
                if (p1 <= p2)
                {
                    programLineCounter = gotoLine(line.extension.param1, line.extension.param1Type);
                }
                else
                {
                    programLineCounter = gotoLine(line.extension.param2, line.extension.param2Type);
                }
            }
            else if (strcmp(line.opr, ">=") == 0)
            {
                if (p1 >= p2)
                {
                    programLineCounter = gotoLine(line.extension.param1, line.extension.param1Type);
                }
                else
                {
                    programLineCounter = gotoLine(line.extension.param2, line.extension.param2Type);
                }
            }
            else
            {
                fprintf(stderr, "Error: Invalid operator\n");
            }
        }
        else if (strcmp(line.command, "PEEK") == 0) // Deploy
        {
            printf("Peek line\n");
        }
        else if (strcmp(line.command, "ABS") == 0) 
        {
            setVariable(line.returnValue, abs(getParamValue(line.param1, line.param1Type)));
        }
        else if (strcmp(line.command, "SGN") == 0)
        {
            if (getParamValue(line.param1, line.param1Type) >= 0)
            {
                setVariable(line.returnValue, 1);
            }
            else
            {
                setVariable(line.returnValue, -1);
            }
        }
        else if (strcmp(line.command, "INT") == 0) // Deploy
        {
            printf("Int line\n");
        }
        else if (strcmp(line.command, "SQR") == 0)
        {
            setVariable(line.returnValue, sqrt(getParamValue(line.param1, line.param1Type)));
        }
        else if (strcmp(line.command, "EXP") == 0)
        {
            setVariable(line.returnValue, exp(getParamValue(line.param1, line.param1Type)));
        }
        else if (strcmp(line.command, "LOG") == 0)
        {
            setVariable(line.returnValue, log(getParamValue(line.param1, line.param1Type)));
        }
        else if (strcmp(line.command, "LOG10") == 0)
        {
            setVariable(line.returnValue, log10(getParamValue(line.param1, line.param1Type)));
        }
        else if (strcmp(line.command, "SIN") == 0)
        {
            setVariable(line.returnValue, sin(getParamValue(line.param1, line.param1Type)));
        }
        else if (strcmp(line.command, "COS") == 0)
        {
            setVariable(line.returnValue, cos(getParamValue(line.param1, line.param1Type)));
        }
        else if (strcmp(line.command, "DEG") == 0)
        {
            setVariable(line.returnValue, getParamValue(line.param1, line.param1Type) * 180.0 / M_PI);
        }
        else if (strcmp(line.command, "RAD") == 0)
        {
            setVariable(line.returnValue, getParamValue(line.param1, line.param1Type) * M_PI / 180);
        }
        else if (strcmp(line.command, "NOT") == 0)
        {
            setVariable(line.returnValue, ~getParamValue(line.param1, line.param1Type));
        }
        else if (strcmp(line.command, "SHL") == 0)
        {
            setVariable(line.returnValue, getParamValue(line.param1, line.param1Type)<<getParamValue(line.param2, line.param2Type));
        }
        else if (strcmp(line.command, "SHR") == 0)
        {
            setVariable(line.returnValue, getParamValue(line.param1, line.param1Type)>>getParamValue(line.param2, line.param2Type));
        }
        else if (strcmp(line.command, "TO") == 0) // Deploy
        {
            printf("To line\n");
        }
        else if (strcmp(line.command, "THEN") == 0) // Deploy
        {
            printf("Then line\n");
        }
        else if (strcmp(line.command, "ELSE") == 0) // Deploy
        {
            printf("Else line\n");
        }
        else
        {
            fprintf(stderr, "Error: Unknown command: %s\n", line.command);
            exit(1);
        }

        programLineCounter++;
    }
}