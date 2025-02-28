/* 
 * Compiler for IWBC
 * Created: February 20, 2025 by LHS
 * Last modified: February 27, 2025 by LHS
 *
 * Goals:
 * 1. Command line parsing
 * 2. Build pipeline coordination
 * 3. Error reporting
 * 4. Resource cleanup
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "lexer.h"
#include "parser.h"
#include "generator.h"

static char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* buffer = malloc(size + 1);
    size_t read_size = fread(buffer, 1, size, file);
    buffer[read_size] = '\0';
    
    fclose(file);
    return buffer;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input.iwb> <output.bc>\n", argv[0]);
        return 1;
    }
    
    char* source = read_file(argv[1]);
    if (!source) {
        return 1;
    }
    
    Lexer* lexer = lexer_create(source);
    Parser* parser = parser_create(lexer);
    ASTNode* ast = parser_parse(parser);
    
    Generator* gen = generator_create("iwbasic_module");
    generator_generate(gen, ast);
    generator_write_bitcode(gen, argv[2]);
    
    // Cleanup
    generator_destroy(gen);
    ast_destroy(ast);
    parser_destroy(parser);
    lexer_destroy(lexer);
    free(source);
    
    return 0;
}

