#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STACK_SIZE 1024
#define WORD_NAME_SIZE 32
#define DICT_SIZE 256

// Stack for the interpreter
typedef struct {
    int data[STACK_SIZE];
    int top;
} Stack;

// Function pointer type for built-in commands
typedef void (*WordFunc)(Stack*);

// Dictionary entry
typedef struct Word {
    char name[WORD_NAME_SIZE];
    WordFunc func;
    struct Word* next;
} Word;

// Global stack and dictionary
Stack stack = {.top = -1};
Word* dictionary = NULL;

// Push a value onto the stack
void stack_push(Stack* s, int value) {
    if (s->top < STACK_SIZE - 1) {
        s->data[++s->top] = value;
    } else {
        printf("Stack overflow\n");
    }
}

// Pop a value from the stack
int stack_pop(Stack* s) {
    if (s->top >= 0) {
        return s->data[s->top--];
    } else {
        printf("Stack underflow\n");
        return 0;
    }
}

// Built-in word: +
void word_add(Stack* s) {
    int b = stack_pop(s);
    int a = stack_pop(s);
    stack_push(s, a + b);
}

// Built-in word: -
void word_subtract(Stack* s) {
    int b = stack_pop(s);
    int a = stack_pop(s);
    stack_push(s, a - b);
}

// Built-in word: *
void word_multiply(Stack* s) {
    int b = stack_pop(s);
    int a = stack_pop(s);
    stack_push(s, a * b);
}

// Built-in word: /
void word_divide(Stack* s) {
    int b = stack_pop(s);
    int a = stack_pop(s);
    if (b != 0) {
        stack_push(s, a / b);
    } else {
        printf("Division by zero\n");
    }
}

// Add a word to the dictionary
void add_word(const char* name, WordFunc func) {
    Word* word = malloc(sizeof(Word));
    strncpy(word->name, name, WORD_NAME_SIZE);
    word->func = func;
    word->next = dictionary;
    dictionary = word;
}

// Find a word in the dictionary
Word* find_word(const char* name) {
    for (Word* word = dictionary; word != NULL; word = word->next) {
        if (strcmp(word->name, name) == 0) {
            return word;
        }
    }
    return NULL;
}

// Function to execute a word
void execute(const char* token) {
    // Try to find the word in the dictionary
    Word* word = find_word(token);
    if (word) {
        word->func(&stack);
    } else {
        // Try to interpret the token as a number
        char* end;
        int value = strtol(token, &end, 10);
        if (*end == '\0') {
            stack_push(&stack, value);
        } else {
            printf("Unknown word: %s\n", token);
        }
    }
}

// Interpret a single line of input
void interpret(char* input) {
    char* token = strtok(input, " \t\n");
    while (token) {
        execute(token);
        token = strtok(NULL, " \t\n");
    }
}

// Read from a file and process each line
void interpret_file(FILE* file) {
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        interpret(line);
    }
}

// Main function
int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file.forth>\n", argv[0]);
        return 1;
    }

    // Add built-in words to the dictionary
    add_word("+", word_add);
    add_word("-", word_subtract);
    add_word("*", word_multiply);
    add_word("/", word_divide);

    // Open the Forth file
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    // Interpret the file
    interpret_file(file);

    // Close the file
    fclose(file);

    return 0;
}
