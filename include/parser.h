/* 
 * Parser header file
 * Created: February 20, 2025 by LHS
 * Last modified: February 27, 2025 by LHS
 *
 */

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum {
    NODE_PROGRAM,
    NODE_LET,
    NODE_PRINT,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_FUNCTION,
    NODE_RETURN,
    NODE_CALL,
    NODE_NUMBER,
    NODE_STRING,
    NODE_IDENTIFIER,
    NODE_OPERATOR,
    NODE_ARRAY_ACCESS
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char* value;
    struct ASTNode** children;
    int children_count;
    int line;
    int column;
} ASTNode;

typedef struct {
    Lexer* lexer;
    Token* current_token;
} Parser;

Parser* parser_create(Lexer* lexer);
ASTNode* parser_parse(Parser* parser);
void parser_destroy(Parser* parser);
void ast_destroy(ASTNode* node);

#endif

