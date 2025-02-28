/* 
 * Code Generator functions
 * Created: February 20, 2025 by LHS
 * Last modified: February 27, 2025 by LHS
 *
 */

#include "generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static LLVMValueRef get_printf_function(LLVMModuleRef module) {
    LLVMValueRef printf_func = LLVMGetNamedFunction(module, "printf");
    if (!printf_func) {
        LLVMTypeRef param_types[] = { LLVMPointerType(LLVMInt8Type(), 0) };
        LLVMTypeRef printf_type = LLVMFunctionType(LLVMInt32Type(), param_types, 1, 1);
        printf_func = LLVMAddFunction(module, "printf", printf_type);
        LLVMSetFunctionCallConv(printf_func, LLVMCCallConv);
        LLVMSetLinkage(printf_func, LLVMExternalLinkage);
    }
    return printf_func;
}

static LLVMValueRef generate_expression(Generator* gen, ASTNode* node) {
    switch (node->type) {
        case NODE_NUMBER: {
            int value = atoi(node->value);
            return LLVMConstInt(LLVMInt32Type(), value, 0);
        }
        
        case NODE_IDENTIFIER: {
            if (gen->var_count > 0) {
                LLVMValueRef var = gen->variables[0];
                return LLVMBuildLoad2(gen->builder, LLVMInt32Type(), var, "load");
            }
            return NULL;
        }
        
        case NODE_OPERATOR: {
            LLVMValueRef left = generate_expression(gen, node->children[0]);
            LLVMValueRef right = generate_expression(gen, node->children[1]);
            
            if (strcmp(node->value, "+") == 0) {
                return LLVMBuildAdd(gen->builder, left, right, "addtmp");
            } else if (strcmp(node->value, "-") == 0) {
                return LLVMBuildSub(gen->builder, left, right, "subtmp");
            } else if (strcmp(node->value, "*") == 0) {
                return LLVMBuildMul(gen->builder, left, right, "multmp");
            } else if (strcmp(node->value, "/") == 0) {
                return LLVMBuildSDiv(gen->builder, left, right, "divtmp");
            }
        }
        
        default:
            return NULL;
    }
}

static LLVMValueRef generate_print(Generator* gen, ASTNode* node) {
    LLVMValueRef printf_func = get_printf_function(gen->module);
    ASTNode* expr = node->children[0];
    
    if (expr->type == NODE_STRING) {
        char* format = "%s\n";
        LLVMValueRef fmt_str = LLVMBuildGlobalStringPtr(gen->builder, format, "fmt");
        LLVMValueRef str = LLVMBuildGlobalStringPtr(gen->builder, expr->value, "str");
        LLVMValueRef args[] = { fmt_str, str };
        LLVMTypeRef printf_type = LLVMFunctionType(LLVMInt32Type(), 
                                                  (LLVMTypeRef[]){LLVMPointerType(LLVMInt8Type(), 0)}, 
                                                  1, 1);
        return LLVMBuildCall2(gen->builder, printf_type, printf_func, args, 2, "");
    } else {
        char* format = "%d\n";
        LLVMValueRef fmt_str = LLVMBuildGlobalStringPtr(gen->builder, format, "fmt");
        LLVMValueRef value = generate_expression(gen, expr);
        LLVMValueRef args[] = { fmt_str, value };
        LLVMTypeRef printf_type = LLVMFunctionType(LLVMInt32Type(), 
                                                  (LLVMTypeRef[]){LLVMPointerType(LLVMInt8Type(), 0)}, 
                                                  1, 1);
        return LLVMBuildCall2(gen->builder, printf_type, printf_func, args, 2, "");
    }
}

static void generate_let(Generator* gen, ASTNode* node) {
    LLVMValueRef value = generate_expression(gen, node->children[1]);
    LLVMValueRef var = LLVMBuildAlloca(gen->builder, LLVMInt32Type(), node->children[0]->value);
    LLVMBuildStore(gen->builder, value, var);
    
    gen->var_count++;
    gen->variables = realloc(gen->variables, gen->var_count * sizeof(LLVMValueRef));
    gen->variables[gen->var_count - 1] = var;
}

Generator* generator_create(const char* module_name) {
    Generator* gen = malloc(sizeof(Generator));
    gen->module = LLVMModuleCreateWithName(module_name);
    gen->builder = LLVMCreateBuilder();
    
    LLVMTypeRef main_type = LLVMFunctionType(LLVMInt32Type(), NULL, 0, 0);
    gen->function = LLVMAddFunction(gen->module, "main", main_type);
    
    gen->current_block = LLVMAppendBasicBlock(gen->function, "entry");
    LLVMPositionBuilderAtEnd(gen->builder, gen->current_block);
    
    gen->variables = NULL;
    gen->var_count = 0;
    
    return gen;
}

void generator_generate(Generator* gen, ASTNode* node) {
    for (int i = 0; i < node->children_count; i++) {
        ASTNode* child = node->children[i];
        switch (child->type) {
            case NODE_PRINT:
                generate_print(gen, child);
                break;
            case NODE_LET:
                generate_let(gen, child);
                break;
        }
    }
    
    LLVMBuildRet(gen->builder, LLVMConstInt(LLVMInt32Type(), 0, 0));
}

void generator_write_bitcode(Generator* gen, const char* filename) {
    if (LLVMWriteBitcodeToFile(gen->module, filename) != 0) {
        fprintf(stderr, "Error writing bitcode to file\n");
    }
}

void generator_destroy(Generator* gen) {
    free(gen->variables);
    LLVMDisposeBuilder(gen->builder);
    LLVMDisposeModule(gen->module);
    free(gen);
}

