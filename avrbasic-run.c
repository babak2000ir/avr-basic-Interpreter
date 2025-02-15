#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_VARIABLES 100
#define MAX_PROGRAM_LINES 255
#define MAX_TOKENS 10
#define MAX_TOKEN_LEN 6

typedef struct
{
    char name;
    int value;
} Variable;

typedef struct
{
    int lineNumber;
    char returnValue[MAX_TOKEN_LEN];
    char command[MAX_TOKEN_LEN];
    char param1[MAX_TOKEN_LEN];
    char operator[MAX_TOKEN_LEN];
    char param2[MAX_TOKEN_LEN];
    struct
    {
        char command1[MAX_TOKEN_LEN];
        char param1[MAX_TOKEN_LEN];
        char command2[MAX_TOKEN_LEN];
        char param2[MAX_TOKEN_LEN];
    } extension;
} ProgramLine;

Variable variables[MAX_VARIABLES];
ProgramLine program[MAX_PROGRAM_LINES];
int variableCount = 0;
int programLength = 0;

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
        fprintf(stderr, "Error: A token on line %d exceeded MAX_TOKEN_LEN of %d\n", programLength + 1, MAX_TOKEN_LEN - 1);
        exit(1);
    }
    j++; // Increment j
    return prev;
}
int get_j()
{
    return j; // Return the current value of j
}

void tokenize(char *buffer, ProgramLine *line)
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
                printf("Command %i: %s\n", tokenCounter, token);
            }
            else
            {
                if (is_operator(token))
                {
                    previousTokenType = TOKEN_OPERATOR;
                    printf("Operator %i: %s\n", tokenCounter, token);
                }
                else
                {
                    previousTokenType = TOKEN_VARIABLE;
                    printf("Variable %i: %s\n", tokenCounter, token);
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
                printf("Line Number: %s\n", token);
            }
            else
            {
                tokenCounter++;
                previousTokenType = TOKEN_VALUE;
                printf("Value %i: %s\n", tokenCounter, token);
            }
        }
        // Operators (simple single-character)
        else if (strchr("+-*/=,><^", buffer[i]))
        {
            while (ispunct(buffer[i]))
            {
                char *stop = strchr("=,^+", buffer[i]);
                token[get_jpp()] = buffer[i++];
                if (stop)
                {
                    break;
                }
            }
            token[get_j()] = '\0';
            tokenCounter++;
            previousTokenType = TOKEN_OPERATOR;
            printf("Operator %i: %s\n", tokenCounter, token);
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

void loadProgram(const char *filename, int lineNo)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Failed to open file");
        exit(1);
    }

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), file))
    {
        if (lineNo > 0)
        {
            if (programLength == lineNo - 1)
            {
                ProgramLine line;
                tokenize(buffer, &line);
                program[programLength++] = line;
                break;
            }
            else
            {
                programLength++;
            }
        }
        else
        {
            ProgramLine line;
            tokenize(buffer, &line);
            program[programLength++] = line;
        }
    }

    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <program file> [<line number>]\n", argv[0]);
        return 1;
    }

    int lineNo = 0;
    if (argc == 3)
    {
        lineNo = atoi(argv[2]);
    }

    loadProgram(argv[1], lineNo);
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

void executeProgram()
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
}
