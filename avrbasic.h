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

typedef struct ProgrammingLineExtensionTag
{
    char *command1;
    char *param1;
    TokenType param1Type;
    char *command2;
    char *param2;
    TokenType param2Type;
} Extension;

typedef struct ProgramLineTag
{
    int lineNumber;
    char *returnValue;
    char *command;
    char *param1;
    TokenType param1Type;
    char *opr;
    char *param2;
    TokenType param2Type;
    Extension extension;
} ProgramLine;

typedef struct VariableTag
{
    char* name;
    int value;
} Variable;

extern char *fileLines[MAX_PROGRAM_LINES];
extern Variable variables[MAX_VARIABLES];
extern ProgramLine program[MAX_PROGRAM_LINES];
extern int variableCount;
extern int programLineCount;

// Lists of reserved words
extern const char *commands[32];
extern const char *operators[16];

// Functions to check if the token is a reserved word
extern void toUppercase(char str[]);
extern int is_command(char *token);
extern int is_operator(char *token);

#endif