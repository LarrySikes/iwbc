#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_LET: return "LET";
        case TOKEN_PRINT: return "PRINT";
        case TOKEN_DIM: return "DIM";
        case TOKEN_FOR: return "FOR";
        case TOKEN_TO: return "TO";
        case TOKEN_NEXT: return "NEXT";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_WEND: return "WEND";
        case TOKEN_IF: return "IF";
        case TOKEN_THEN: return "THEN";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_ENDIF: return "ENDIF";
        case TOKEN_SELECT: return "SELECT";
        case TOKEN_CASE: return "CASE";
        case TOKEN_ENDSELECT: return "ENDSELECT";
        case TOKEN_FUNCTION: return "FUNCTION";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_END: return "END";
        case TOKEN_EQUALS: return "EQUALS";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_DIVIDE: return "DIVIDE";
        case TOKEN_GT: return "GT";
        case TOKEN_LT: return "LT";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "UNDEFINED";
    }
}

void print_token(Token* token) {
    printf("Token: %-12s | Value: %-15s | Line: %-4d | Column: %-4d\n",
           token_type_to_string(token->type),
           token->value ? token->value : "null",
           token->line,
           token->column);
}

int main(int argc, char* argv[]) {
    const char* input;
    char* buffer = NULL;

    if (argc > 1) {
        FILE* file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
            return 1;
        }
        
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        buffer = malloc(size + 1);
        fread(buffer, 1, size, file);
        buffer[size] = '\0';
        fclose(file);
        
        input = buffer;
    } else {
        input = "LET x = 42\n"
               "PRINT \"Current value is: \"\n"
               "PRINT x\n"
               "IF x > 40 THEN\n"
               "    PRINT \"Greater than 40\"\n"
               "ENDIF\n";
    }
    
    printf("Lexical Analysis Output:\n");
    printf("----------------------------------------\n");
    
    Lexer* lexer = lexer_create(input);
    Token* token;
    
    while (1) {
        token = lexer_next_token(lexer);
        print_token(token);
        
        if (token->type == TOKEN_EOF) {
            free(token->value);
            free(token);
            break;
        }
        
        free(token->value);
        free(token);
    }
    
    lexer_destroy(lexer);
    free(buffer);  // Only free if we allocated it
    
    return 0;
}

