
#include <stdio.h>
#include "avrbasic.h"

char *fileLines[MAX_PROGRAM_LINES];
Variable variables[MAX_VARIABLES];
ProgramLine program[MAX_PROGRAM_LINES];
int variableCount = 0;
int programLineCount = 0;

// Lists of reserved words
const char *commands[] = {"END", "CLEAR", "NEXT", "WEND", "DO", "GOTO", "WHILE", "UNTIL", "PRINT", "INPUT",
                            "LET", "POKE", "FOR", "IF", "PEEK", "ABS", "SGN", "INT", "SQR", "EXP", "LOG", "LOG10",
                            "SIN", "COS", "DEG", "RAD", "NOT", "SHL", "SHR", "TO", "THEN", "ELSE"};

const char *operators[] = {"=", "<>", "<", ">", "<=", ">=", "+", "-", "*", "/", "AND", "OR", "XOR", "MOD", ",", "^"};

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

int is_lineStop(char* lastChar, int lastCharIndex)
{
    if (lastChar[lastCharIndex] == ',' || lastChar[lastCharIndex] == '\n' || lastChar[lastCharIndex] == '\0')
    {
        return 1;
    } 
    else if (lastChar[lastCharIndex] == '=')
    {
        if (lastCharIndex == 0 || (lastChar[lastCharIndex-1] != '>' && lastChar[lastCharIndex-1] != '<'))
        {
            return 1;
        }
    }
    return 0;
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

Stack stack;

// Function to initialize the stack
void initializeStack() {
    // Set top index to -1 to indicate an empty stack
    stack.top = -1;  
}

// Function to push an element onto the stack
void push(int value) {
    // Check for stack overflow
    if (stack.top == MAX_STACK_SIZE - 1) {
        fprintf(stderr, "Stack Overflow\n");
        return;
    }
    // Increment top and add the value to the top of the stack
    stack.arr[++stack.top] = value;
}

// Function to pop an element from the stack
int pop() {
    // Check for stack underflow
    if (stack.top == -1) {
        return 0;
    }
    // Return the top element 
    int popped = stack.arr[stack.top];
    // decrement top pointer
    stack.top--;
    // return the popped element
    return popped;
}

int peek() {
    // Check for stack underflow
    if (stack.top == -1) {
        return 0;
    }
    // Return the top element
    return stack.arr[stack.top];
}
