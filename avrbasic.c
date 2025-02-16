
#include "avrbasic.h"

char *fileLines[MAX_PROGRAM_LINES];
Variable variables[MAX_VARIABLES];
ProgramLine program[MAX_PROGRAM_LINES];
int variableCount = 0;
int programLineCount = 0;

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