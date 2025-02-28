/* 
 * Lexer header file
 * Created: February 20, 2025 by LHS
 * Last modified: February 27, 2025 by LHS
 *
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stddef.h>

typedef enum {
    // Keywords
    TOKEN_LET,
    TOKEN_PRINT,
    TOKEN_DIM,
    TOKEN_FOR,
    TOKEN_TO,
    TOKEN_NEXT,
    TOKEN_WHILE,
    TOKEN_WEND,
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELSE,
    TOKEN_ENDIF,
    TOKEN_SELECT,
    TOKEN_CASE,
    TOKEN_ENDSELECT,
    TOKEN_FUNCTION,
    TOKEN_RETURN,
    TOKEN_END,
    
    // Operators
    TOKEN_EQUALS,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_GT,
    TOKEN_LT,
    
    // Delimiters
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_COMMA,
    
    // Values
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    
    // Special
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    int line;
    int column;
} Token;

typedef struct {
    char* source;
    int position;
    int line;
    int column;
} Lexer;

Lexer* lexer_create(const char* source);
Token* lexer_next_token(Lexer* lexer);
void lexer_destroy(Lexer* lexer);

#endif

