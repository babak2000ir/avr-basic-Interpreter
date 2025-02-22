#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avrbasic-tokenizer.h"
#include "avrbasic.h"

int tokenizerLineCounter = 0;

void addCommandToLine(ProgramLine* line, char *command)
{
    if (line->command[0] == '\0')
    {
        strcpy(line->command, command);
    }
    else
    {
        char extCommand[2] = {command[0] + '\0'};
        strcat(line->command, extCommand);
    }
}

void addParamToLine(ProgramLine* line, char *param, int paramCounter)
{
    line->paramArray = (Param*)realloc(line->paramArray, sizeof(Param) * (paramCounter + 1));
    strcpy(line->paramArray[paramCounter].param, param);
    line->paramArray[paramCounter].paramType = TOKEN_EXPRESSION;
}

void tokenizeLine(char *buffer, ProgramLine* line)
{
    int characterCounter = 0;
    int tokenCharacterCounter = 0;
    int solidTokenCharacterCounter = 0;
    int tokenCounter = 0;
    int paramCounter = 0;

    char token[100];
    char solidToken[100];

    // handle line no
    if (!(buffer[0]))
    {
        fprintf(stderr, "Error: Invalid line number \n");
        exit(1);
    }
    while (isalnum(buffer[characterCounter]))
    {
        token[characterCounter] = buffer[characterCounter++];
    }
    token[characterCounter++] = '\0';
    line->lineNumber = atoi(token);


    //convert solid "AND", "OR", "XOR", "MOD" to &, |, @, £


    // handle rest
    do 
    {
        //, = \n \0 stop
        if (is_lineStop(buffer[characterCounter])) //|| strlen(token) >= MAX_TOKEN_LEN
        {
            // Check Token if it's a command
            if (is_command(token))
            {
                addCommandToLine(line, token);
            }
            else
            {
                // it's an expression, save as a param in paramArray
                addParamToLine(line, token, paramCounter++);
            }

            tokenCharacterCounter = 0;
            solidTokenCharacterCounter = 0;
            token[0] = '\0';
            solidToken[0] = '\0';
        }
        else if (isspace(buffer[characterCounter]))
        {
            // space -> Check stop/continue
            if (is_command(token) && tokenCharacterCounter != 0)
            {
                addCommandToLine(line, token);
                tokenCharacterCounter = 0;
                token[0] = '\0';
            }
            else if (is_command(solidToken) && solidTokenCharacterCounter != 0)
            {
                addCommandToLine(line, solidToken);
                token[tokenCharacterCounter - solidTokenCharacterCounter] = '\0'; 
                addParamToLine(line, token, paramCounter++);
                tokenCharacterCounter = 0;
                token[0] = '\0';
            }

            solidTokenCharacterCounter = 0;
            solidToken[0] = '\0';
        }
        else
        {
            token[tokenCharacterCounter] = buffer[characterCounter];
            token[tokenCharacterCounter + 1] = '\0';
            solidToken[solidTokenCharacterCounter] = buffer[characterCounter];
            solidToken[solidTokenCharacterCounter + 1] = '\0';

            tokenCharacterCounter++;
            solidTokenCharacterCounter++;
        }
        characterCounter++;
    } while (buffer[characterCounter] != '\0');
}

void tokenize()
{
    char buffer[MAX_LINE_LEN];
    for (tokenizerLineCounter = 0; tokenizerLineCounter < programLineCount; tokenizerLineCounter++)
    {
        tokenizeLine(fileLines[tokenizerLineCounter], &program[tokenizerLineCounter]);
    }
}