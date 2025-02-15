#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_VARIABLES 100
#define MAX_PROGRAM_LINES 255
#define MAX_LINE_LEN 100
#define MAX_TOKENS 10
#define MAX_TOKEN_LEN 6

// Token Types Enum
typedef enum
{
    TOKEN_UNKNOWN,
    TOKEN_LINE_NUMBER,
    TOKEN_COMMAND,
    TOKEN_VARIABLE,
    TOKEN_VALUE,
    TOKEN_OPERATOR
} TokenType;

typedef struct
{
    int lineNumber;
    char returnValue[MAX_TOKEN_LEN];
    char command[MAX_TOKEN_LEN];
    char param1[MAX_TOKEN_LEN];
    char opr[MAX_TOKEN_LEN];
    char param2[MAX_TOKEN_LEN];
    struct
    {
        char command1[MAX_TOKEN_LEN];
        char param1[MAX_TOKEN_LEN];
        char command2[MAX_TOKEN_LEN];
        char param2[MAX_TOKEN_LEN];
    } extension;
} ProgramLine;

typedef struct
{
    char name;
    int value;
} Variable;

extern char* fileLines[MAX_PROGRAM_LINES];
extern Variable variables[MAX_VARIABLES];
extern ProgramLine program[MAX_PROGRAM_LINES];
extern int variableCount;
extern int programLineCount;

#endif