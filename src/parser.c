/* 
 * Parser for IWBC
 * Created: February 20, 2025 by LHS
 * Last modified: February 27, 2025 by LHS
 *
 */

#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Debug variables for GDB inspection
Token* debug_token = NULL;
Parser* debug_parser = NULL;

// Node management functions
ASTNode* create_node(NodeType type, const char* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->children = NULL;
    node->children_count = 0;
    printf("Created node type=%d value=%s\n", type, value ? value : "null");
    return node;
}

void add_child(ASTNode* parent, ASTNode* child) {
    parent->children_count++;
    parent->children = realloc(parent->children, parent->children_count * sizeof(ASTNode*));
    parent->children[parent->children_count - 1] = child;
    printf("Added child to parent type=%d\n", parent->type);
}

Token* get_next_token(Parser* parser) {
    Token* token = parser->current_token;
    debug_token = token;
    printf("Current token: type=%d value=%s\n", 
           token->type, 
           token->value ? token->value : "null");
    parser->current_token = lexer_next_token(parser->lexer);
    return token;
}

// Forward declarations
ASTNode* parse_expression(Parser* parser);
ASTNode* parse_primary(Parser* parser);
ASTNode* parse_statement(Parser* parser);

ASTNode* parse_primary(Parser* parser) {
    Token* token = parser->current_token;
    printf("Parsing primary: type=%d value=%s\n", 
           token->type, 
           token->value ? token->value : "null");
    
    switch (token->type) {
        case TOKEN_NUMBER: {
            ASTNode* node = create_node(NODE_NUMBER, token->value);
            get_next_token(parser);
            return node;
        }
        case TOKEN_IDENTIFIER: {
            ASTNode* node = create_node(NODE_IDENTIFIER, token->value);
            get_next_token(parser);
            return node;
        }
        case TOKEN_STRING: {
            ASTNode* node = create_node(NODE_STRING, token->value);
            get_next_token(parser);
            return node;
        }
        default:
            printf("ERROR: Unexpected token in primary expression\n");
            return NULL;
    }
}

ASTNode* parse_expression(Parser* parser) {
    ASTNode* left = parse_primary(parser);
    if (!left) return NULL;
    
    while (parser->current_token->type == TOKEN_PLUS ||
           parser->current_token->type == TOKEN_MINUS ||
           parser->current_token->type == TOKEN_MULTIPLY ||
           parser->current_token->type == TOKEN_DIVIDE) {
        
        Token* op_token = parser->current_token;
        get_next_token(parser);
        
        ASTNode* right = parse_primary(parser);
        if (!right) return NULL;
        
        ASTNode* op_node = create_node(NODE_OPERATOR, op_token->value);
        add_child(op_node, left);
        add_child(op_node, right);
        left = op_node;
    }
    
    return left;
}

ASTNode* parse_statement(Parser* parser) {
    debug_parser = parser;
    debug_token = parser->current_token;
    
    printf("Parsing statement: type=%d value=%s\n", 
           debug_token->type,
           debug_token->value ? debug_token->value : "null");
    
    switch (parser->current_token->type) {
        case TOKEN_LET: {
            get_next_token(parser);
            
            if (parser->current_token->type != TOKEN_IDENTIFIER) {
                printf("ERROR: Expected identifier after LET\n");
                return NULL;
            }
            
            Token* identifier = parser->current_token;
            get_next_token(parser);
            
            if (parser->current_token->type != TOKEN_EQUALS) {
                printf("ERROR: Expected = after identifier\n");
                return NULL;
            }
            get_next_token(parser);
            
            ASTNode* expr = parse_expression(parser);
            if (!expr) return NULL;
            
            ASTNode* let_node = create_node(NODE_LET, NULL);
            add_child(let_node, create_node(NODE_IDENTIFIER, identifier->value));
            add_child(let_node, expr);
            return let_node;
        }
        
        case TOKEN_PRINT: {
            get_next_token(parser);
            ASTNode* expr = parse_expression(parser);
            if (!expr) return NULL;
            
            ASTNode* print_node = create_node(NODE_PRINT, NULL);
            add_child(print_node, expr);
            return print_node;
        }
        
        case TOKEN_EOF:
            printf("DEBUG: Reached end of file\n");
            return NULL;
            
        default:
            printf("ERROR: Unknown statement type: %d\n", parser->current_token->type);
            return NULL;
    }
}

Parser* parser_create(Lexer* lexer) {
    Parser* parser = malloc(sizeof(Parser));
    parser->lexer = lexer;
    parser->current_token = lexer_next_token(parser->lexer);
    debug_parser = parser;
    printf("Parser created, first token: type=%d\n", parser->current_token->type);
    return parser;
}

ASTNode* parser_parse(Parser* parser) {
    ASTNode* root = create_node(NODE_PROGRAM, NULL);
    printf("Starting program parse\n");
    
    while (parser->current_token->type != TOKEN_EOF) {
        ASTNode* statement = parse_statement(parser);
        if (!statement) {
            printf("Failed to parse statement, stopping\n");
            break;
        }
        add_child(root, statement);
    }
    
    printf("Completed program parse\n");
    return root;
}

void ast_destroy(ASTNode* node) {
    if (!node) return;
    
    for (int i = 0; i < node->children_count; i++) {
        ast_destroy(node->children[i]);
    }
    
    free(node->children);
    free(node->value);
    free(node);
}

void parser_destroy(Parser* parser) {
    free(parser);
}

