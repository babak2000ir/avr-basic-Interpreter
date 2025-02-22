#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avrbasic-tokenizer.h"
#include "avrbasic.h"

int tokenizerLineCounter = 0;

void addCommandToLine(ProgramLine *line, char *command)
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

void addParamToLine(ProgramLine *line, char *param, int paramCounter)
{
    line->paramArray = (Param *)realloc(line->paramArray, sizeof(Param) * (paramCounter + 1));
    strcpy(line->paramArray[paramCounter].param, param);
    line->paramArray[paramCounter].paramType = TOKEN_EXPRESSION;
}

typedef struct TokenTag
{
    char token[MAX_PARAM_LEN];
    int tokenCharacterCounter;
} Token;

void resetToken(Token *token)
{
    token->tokenCharacterCounter = 0;
    token->token[0] = '\0';
}

void addCharToToken(Token *token, char c)
{
    token->token[token->tokenCharacterCounter++] = c;
    token->token[token->tokenCharacterCounter] = '\0';
}

void tokenizeLine(char *buffer, ProgramLine *line)
{
    int characterCounter = 0;
    int tokenCounter = 0;
    int paramCounter = 0;

    Token token = {
        .tokenCharacterCounter = 0,
    };
    Token solidToken = {
        .tokenCharacterCounter = 0,
    };

    // handle line no
    if (!(buffer[0]))
    {
        fprintf(stderr, "Error: Invalid line number \n");
        exit(1);
    }
    while (isalnum(buffer[characterCounter]))
    {
        addCharToToken(&token, buffer[characterCounter++]);
    }
    
    line->lineNumber = atoi(token.token);
    resetToken(&token);

    // handle rest
    do
    {
        //, = \n \0 stop
        if (is_lineStop(buffer, characterCounter)) //|| strlen(token) >= MAX_TOKEN_LEN
        {
            // Check Token if it's a command
            if (is_command(token.token))
            {
                addCommandToLine(line, token.token);
            }
            else
            {
                // it's an expression, save as a param in paramArray
                addParamToLine(line, token.token, paramCounter++);
            }

            resetToken(&token);
            resetToken(&solidToken);
        }
        else if (isspace(buffer[characterCounter]))
        {
            // space -> Check stop/continue
            if (is_command(token.token) && token.tokenCharacterCounter != 0)
            {
                addCommandToLine(line, token.token);
                resetToken(&token);
            }
            else if (is_command(solidToken.token))
            {
                addCommandToLine(line, solidToken.token);
                token.token[token.tokenCharacterCounter - solidToken.tokenCharacterCounter] = '\0';
                addParamToLine(line, token.token, paramCounter++);
                resetToken(&token);
            }
            else
            {
                if (token.tokenCharacterCounter != 0)
                {
                    addCharToToken(&token, buffer[characterCounter]);
                }
            }

            resetToken(&solidToken);
        }
        else
        {
            addCharToToken(&token, buffer[characterCounter]);
            addCharToToken(&solidToken, buffer[characterCounter]);
        }
        characterCounter++;
    } while (buffer[characterCounter] != '\0');

    if (token.tokenCharacterCounter  > 0)
    {
        addParamToLine(line, token.token, paramCounter++);
    }
}

void tokenize()
{
    char buffer[MAX_LINE_LEN];
    for (tokenizerLineCounter = 0; tokenizerLineCounter < programLineCount; tokenizerLineCounter++)
    {
        tokenizeLine(fileLines[tokenizerLineCounter], &program[tokenizerLineCounter]);
    }
}