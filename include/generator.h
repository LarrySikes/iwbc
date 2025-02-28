/* 
 * Generator header file
 * Created: February 20, 2025 by LHS
 * Last modified: February 27, 2025 by LHS
 *
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include <llvm-c/Core.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Analysis.h>
#include "parser.h"

typedef struct {
    LLVMModuleRef module;
    LLVMBuilderRef builder;
    LLVMValueRef function;
    LLVMBasicBlockRef current_block;
    LLVMValueRef* variables;
    int var_count;
} Generator;

Generator* generator_create(const char* module_name);
void generator_generate(Generator* gen, ASTNode* ast);
void generator_write_bitcode(Generator* gen, const char* filename);
void generator_destroy(Generator* gen);

#endif

