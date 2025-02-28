#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

#define TEST(name) void test_##name()
#define ASSERT(condition) do { \
    if (!(condition)) { \
        printf("Test failed: %s\n", #condition); \
        exit(1); \
    } \
} while (0)

TEST(basic_tokens) {
    const char* input = "LET x = 42";
    Lexer* lexer = lexer_create(input);
    
    Token* token = lexer_next_token(lexer);
    ASSERT(token->type == TOKEN_LET);
    free(token->value);
    free(token);
    
    token = lexer_next_token(lexer);
    ASSERT(token->type == TOKEN_IDENTIFIER);
    ASSERT(strcmp(token->value, "x") == 0);
    free(token->value);
    free(token);
    
    token = lexer_next_token(lexer);
    ASSERT(token->type == TOKEN_EQUALS);
    free(token->value);
    free(token);
    
    token = lexer_next_token(lexer);
    ASSERT(token->type == TOKEN_NUMBER);
    ASSERT(strcmp(token->value, "42") == 0);
    free(token->value);
    free(token);
    
    token = lexer_next_token(lexer);
    ASSERT(token->type == TOKEN_EOF);
    free(token->value);
    free(token);
    
    lexer_destroy(lexer);
}

TEST(string_literal) {
    const char* input = "PRINT \"Hello, World!\"";
    Lexer* lexer = lexer_create(input);
    
    Token* token = lexer_next_token(lexer);
    ASSERT(token->type == TOKEN_PRINT);
    free(token->value);
    free(token);
    
    token = lexer_next_token(lexer);
    ASSERT(token->type == TOKEN_STRING);
    ASSERT(strcmp(token->value, "Hello, World!") == 0);
    free(token->value);
    free(token);
    
    lexer_destroy(lexer);
}

int main() {
    printf("Running lexer tests...\n");
    
    test_basic_tokens();
    test_string_literal();
    
    printf("All tests passed!\n");
    return 0;
}

