/* 
 * Lexer for IWBC
 * Created: February 20, 2025 by LHS
 * Last modified: February 28, 2025 by LHS
 *
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include "lexer.h"


static const char* token_type_to_string(TokenType type) {
    switch(type) {
        case TOKEN_LET: return "LET";
        case TOKEN_PRINT: return "PRINT";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EQUALS: return "EQUALS";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_DIVIDE: return "DIVIDE";
        case TOKEN_EOF: return "EOF";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "UNDEFINED";
    }
}

static Token* create_token(TokenType type, const char* value, int line, int column) {
    Token* token = malloc(sizeof(Token));
    token->type = type;
    token->value = value ? strdup(value) : NULL;
    token->line = line;
    token->column = column;
    printf("Created token: %s, value: %s\n", token_type_to_string(type), value ? value : "null");
    return token;
}

static char peek(Lexer* lexer) {
    return lexer->source[lexer->position];
}

static char advance(Lexer* lexer) {
    char c = peek(lexer);
    if (c != '\0') {
        lexer->position++;
        lexer->column++;
        if (c == '\n') {
            lexer->line++;
            lexer->column = 0;
        }
    }
    return c;
}

static void skip_whitespace(Lexer* lexer) {
    while (isspace(peek(lexer))) {
        advance(lexer);
    }
}

static Token* read_identifier(Lexer* lexer) {
    int start_pos = lexer->position;
    int start_column = lexer->column;
    
    while (isalnum(peek(lexer)) || peek(lexer) == '_') {
        advance(lexer);
    }
    
    int length = lexer->position - start_pos;
    char* value = malloc(length + 1);
    strncpy(value, &lexer->source[start_pos], length);
    value[length] = '\0';
    
    TokenType type = TOKEN_IDENTIFIER;
    if (strcasecmp(value, "LET") == 0) type = TOKEN_LET;
    else if (strcasecmp(value, "PRINT") == 0) type = TOKEN_PRINT;
    else if (strcasecmp(value, "ECHO") == 0) type = TOKEN_PRINT; 
    
    Token* token = create_token(type, value, lexer->line, start_column);
    free(value);
    return token;
}

static Token* read_number(Lexer* lexer) {
    int start_pos = lexer->position;
    int start_column = lexer->column;
    
    while (isdigit(peek(lexer))) {
        advance(lexer);
    }
    
    int length = lexer->position - start_pos;
    char* value = malloc(length + 1);
    strncpy(value, &lexer->source[start_pos], length);
    value[length] = '\0';
    
    Token* token = create_token(TOKEN_NUMBER, value, lexer->line, start_column);
    free(value);
    return token;
}

static Token* read_string(Lexer* lexer) {
    int start_column = lexer->column;
    advance(lexer); // Skip opening quote
    
    int start_pos = lexer->position;
    while (peek(lexer) != '"' && peek(lexer) != '\0') {
        advance(lexer);
    }
    
    int length = lexer->position - start_pos;
    char* value = malloc(length + 1);
    strncpy(value, &lexer->source[start_pos], length);
    value[length] = '\0';
    
    if (peek(lexer) == '"') {
        advance(lexer); // Skip closing quote
    }
    
    Token* token = create_token(TOKEN_STRING, value, lexer->line, start_column);
    free(value);
    return token;
}

Lexer* lexer_create(const char* source) {
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->source = strdup(source);
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 0;
    printf("Lexer created with source: %s\n", source);
    return lexer;
}

Token* lexer_next_token(Lexer* lexer) {
    skip_whitespace(lexer);
    
    char c = peek(lexer);
    printf("Processing character: %c\n", c);
    
    if (c == '\0') {
        return create_token(TOKEN_EOF, NULL, lexer->line, lexer->column);
    }
    
    if (isalpha(c)) {
        return read_identifier(lexer);
    }
    
    if (isdigit(c)) {
        return read_number(lexer);
    }
    
    if (c == '"') {
        return read_string(lexer);
    }
    
    advance(lexer);
    switch (c) {
        case '=': return create_token(TOKEN_EQUALS, "=", lexer->line, lexer->column - 1);
        case '+': return create_token(TOKEN_PLUS, "+", lexer->line, lexer->column - 1);
        case '-': return create_token(TOKEN_MINUS, "-", lexer->line, lexer->column - 1);
        case '*': return create_token(TOKEN_MULTIPLY, "*", lexer->line, lexer->column - 1);
        case '/': return create_token(TOKEN_DIVIDE, "/", lexer->line, lexer->column - 1);
    }
    
    return create_token(TOKEN_UNKNOWN, NULL, lexer->line, lexer->column - 1);
}

void lexer_destroy(Lexer* lexer) {
    free(lexer->source);
    free(lexer);
}

