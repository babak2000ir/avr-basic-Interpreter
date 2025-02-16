#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avrbasic-tokenizer.h"
#include "avrbasic.h"

int tokenizerLineCounter = 0;
int j = 0; // Global variable j
int init_j()
{
    j = 0; // Initialize j
    return j;
}
int get_jpp()
{
    int prev = j; // Store the previous value
    if (j == MAX_TOKEN_LEN - 1)
    {
        fprintf(stderr, "Error: A token on line %d exceeded MAX_TOKEN_LEN of %d\n", tokenizerLineCounter + 1, MAX_TOKEN_LEN - 1);
        exit(1);
    }
    j++; // Increment j
    return prev;
}
int get_j()
{
    return j; // Return the current value of j
}

void tokenizeLine(char *buffer, ProgramLine *line)
{
    int i = 0, tokenCounter = 0;
    char token[MAX_TOKEN_LEN];
    TokenType previousTokenType = TOKEN_UNKNOWN;

    while (buffer[i] != '\0')
    {
        if (isspace(buffer[i]))
        {
            i++;
            continue;
        }

        init_j(); // Initialize j

        int negSign = ((previousTokenType == TOKEN_COMMAND || previousTokenType == TOKEN_OPERATOR) && buffer[i] == '-' && isdigit(buffer[i + 1]));

        // Identifiers (simple version)
        if (isalpha(buffer[i]))
        {
            while (isalnum(buffer[i]))
            {
                token[get_jpp()] = buffer[i++];
            }
            token[get_j()] = '\0';
            tokenCounter++;

            if (is_command(token))
            {
                previousTokenType = TOKEN_COMMAND;
                // printf("Command %i: %s\n", tokenCounter, token);
                if (line->command == NULL)
                {
                    line->command = strdup(token);
                }
                else if (line->extension.command1 == NULL)
                {
                    line->extension.command1 = strdup(token);
                }
                else
                {
                    line->extension.command2 = strdup(token);
                }
            }
            else
            {
                if (is_operator(token))
                {
                    previousTokenType = TOKEN_OPERATOR;
                    //printf("Operator %i: %s\n", tokenCounter, token);
                    line->opr = strdup(token);
                }
                else
                {
                    previousTokenType = TOKEN_VARIABLE;
                    //printf("Variable %i: %s\n", tokenCounter, token);
                    if (tokenCounter == 1) 
                    {
                        line->returnValue = strdup(token);
                    } 
                    else if (line->param1 == NULL)
                    {
                        line->param1 = strdup(token);
                    }
                    else if (line->param2 == NULL)
                    {
                        line->param2 = strdup(token);
                    }
                    else if (line->extension.param1 == NULL)
                    {
                        line->extension.param1 = strdup(token);
                    }
                    else
                    {
                        line->extension.param2 = strdup(token);
                    }
                }
            }
        }
        // Numbers
        else if (isdigit(buffer[i]) || negSign)
        {
            if (negSign)
            {
                token[get_jpp()] = buffer[i++];
            }
            while (isdigit(buffer[i]))
            {
                token[get_jpp()] = buffer[i++];
            }
            token[get_j()] = '\0';

            if (tokenCounter == 0)
            {
                previousTokenType = TOKEN_LINE_NUMBER;
                //printf("Line Number: %s\n", token);
                line->lineNumber = atoi(token);
            }
            else
            {
                tokenCounter++;
                previousTokenType = TOKEN_VALUE;
                //printf("Value %i: %s\n", tokenCounter, token);
                if (line->param1 == NULL)
                {
                    line->param1 = strdup(token);
                }
                else if (line->param2 == NULL)
                {
                    line->param2 = strdup(token);
                }
                else if (line->extension.param1 == NULL)
                {
                    line->extension.param1 = strdup(token);
                }
                else
                {
                    line->extension.param2 = strdup(token);
                }
            }
        }
        // Operators (simple single-character)
        else if (strchr("+-*/=,><^", buffer[i]))
        {
            while (ispunct(buffer[i]))
            {
                char* stop = strchr("=,^+", buffer[i]);
                token[get_jpp()] = buffer[i++];
                if (stop)
                {
                    break;
                }
            }
            token[get_j()] = '\0';
            tokenCounter++;
            previousTokenType = TOKEN_OPERATOR;
            //printf("Operator %i: %s\n", tokenCounter, token);
            line->opr = strdup(token);
        }
        // Unknown character
        else
        {
            printf("TOKEN_UNKNOWN %i: %c\n", tokenCounter, buffer[i]);
            i++;
            tokenCounter++;
        }
    }
}

void tokenize()
{
    char buffer[MAX_LINE_LEN];
    for (tokenizerLineCounter = 0; tokenizerLineCounter < programLineCount; tokenizerLineCounter++)
    {
        strcpy(buffer, fileLines[tokenizerLineCounter]);
        tokenizeLine(buffer, &program[tokenizerLineCounter]);
    }
}