#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_VARIABLES 100
#define MAX_PROGRAM_LINES 255
#define MAX_TOKENS 10
#define MAX_TOKEN_LEN 6

typedef struct {
    char name;
    int value;
} Variable;

typedef struct {
    int lineNumber;
    char returnValue[MAX_TOKEN_LEN];
    char command[MAX_TOKEN_LEN];
    char param1[MAX_TOKEN_LEN];
    char operator[MAX_TOKEN_LEN];
    char param2[MAX_TOKEN_LEN];
    struct {
        char command[MAX_TOKEN_LEN];
        char param1[MAX_TOKEN_LEN];
        char operator[MAX_TOKEN_LEN];
        char param2[MAX_TOKEN_LEN];
    }  extension;
} ProgramLine;

Variable variables[MAX_VARIABLES];
ProgramLine program[MAX_PROGRAM_LINES];
int variableCount = 0;
int programLength = 0;

void loadProgram(const char* filename);
void parseLine(char *buffer, ProgramLine *line);
void tokenize(char *buffer);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <program file>\n", argv[0]);
        return 1;
    }

    loadProgram(argv[1]);
    //executeProgram();
    return 0;
}

void loadProgram(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(1);
    }

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), file)) {
        ProgramLine line;
        tokenize(buffer);
        //program[programLength++] = line;
    }

    fclose(file);
}

void tokenize(char *buffer) {
    int i = 0, j = 0;
    char token[MAX_TOKEN_LEN];

    while (buffer[i] != '\0') {
        if (isspace(buffer[i])) {
            i++;
            continue;
        }

        j = 0;

        // Identifiers (simple version)
        if (isalpha(buffer[i])) {
            while (isalnum(buffer[i])) {
                token[j++] = buffer[i++];
            }
            token[j] = '\0';
            printf("TOKEN_IDENTIFIER: %s\n", token);
        }
        // Numbers
        else if (isdigit(buffer[i])) {
            while (isdigit(buffer[i])) {
                token[j++] = buffer[i++];
            }
            token[j] = '\0';
            printf("TOKEN_NUMBER: %s\n", token);
        }
        // Operators (simple single-character)
        else if (strchr("+-*/=;,", buffer[i])) {
            token[0] = buffer[i++];
            token[1] = '\0';
            printf("TOKEN_OPERATOR: %s\n", token);
        }
        // Unknown character
        else {
            printf("TOKEN_UNKNOWN: %c\n", buffer[i]);
            i++;
        }
    }
}

void parseLine(char *buffer, ProgramLine *line) {
    char *tokens[MAX_TOKENS];
    int tokenCount = 0;
    
    int lineNumber = 0;
    if (!sscanf(buffer, "%d", &lineNumber) == 1) {
        printf("Failed to extract number.\n");
    }

    char *delimiters = " ,";
    char *token = strtok(buffer, delimiters);
    while (token != NULL && tokenCount < MAX_TOKENS) {
        tokens[tokenCount++] = token;
        token = strtok(NULL, delimiters);
    }
    
    if (tokenCount > 0) line->lineNumber = atoi(tokens[0]);
    if (tokenCount > 1) strncpy(line->returnValue, tokens[1], sizeof(line->returnValue) - 1);
    if (tokenCount > 2) strncpy(line->command, tokens[2], sizeof(line->command) - 1);
    if (tokenCount > 3) strncpy(line->param1, tokens[3], sizeof(line->param1) - 1);
    if (tokenCount > 4) strncpy(line->operator, tokens[4], sizeof(line->operator) - 1);
    if (tokenCount > 5) strncpy(line->param2, tokens[5], sizeof(line->param2) - 1);
    if (tokenCount > 6) strncpy(line->extension.command, tokens[6], sizeof(line->extension.command) - 1);
    if (tokenCount > 7) strncpy(line->extension.param1, tokens[7], sizeof(line->extension.param1) - 1);
    if (tokenCount > 8) strncpy(line->extension.operator, tokens[8], sizeof(line->extension.operator) - 1);
    if (tokenCount > 9) strncpy(line->extension.param2, tokens[9], sizeof(line->extension.param2) - 1);
}


Variable* findVariable(char name) {
    for (int i = 0; i < variableCount; i++) {
        if (variables[i].name == name) {
            return &variables[i];
        }
    }
    return NULL;
}

void setVariable(char name, int value) {
    Variable* var = findVariable(name);
    if (var == NULL) {
        if (variableCount < MAX_VARIABLES) {
            variables[variableCount].name = name;
            variables[variableCount].value = value;
            variableCount++;
        } else {
            printf("Variable limit exceeded\n");
        }
    } else {
        var->value = value;
    }
}

int getVariableValue(char name) {
    Variable* var = findVariable(name);
    return var ? var->value : 0;
}

void executeProgram() {
    for (int i = 0; i < programLength; i++) {
        ProgramLine line = program[i];
        if (strcmp(line.command, "LET") == 0) {
            int value = atoi(line.param2);
            setVariable(line.param1[0], value);
        } else if (strcmp(line.command, "PRINT") == 0) {
            int value = getVariableValue(line.param2[0]);
            printf("%d\n", value);
        } else if (strcmp(line.command, "IF") == 0) {
            int left = getVariableValue(line.param1[0]);
            int right = getVariableValue(line.param2[0]);
            int condition = 0;
            if (strcmp(line.operator, "=") == 0) condition = (left == right);
            else if (strcmp(line.operator, "<>") == 0) condition = (left != right);
            else if (strcmp(line.operator, "<") == 0) condition = (left < right);
            else if (strcmp(line.operator, ">") == 0) condition = (left > right);
            else if (strcmp(line.operator, "<=") == 0) condition = (left <= right);
            else if (strcmp(line.operator, ">=") == 0) condition = (left >= right);

            if (condition) {
                // Handle THEN and ELSE parts
            }
        }
        // Add more command handling here
    }
}







