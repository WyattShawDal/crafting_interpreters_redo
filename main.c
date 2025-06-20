#include "common.h"
#include "assert.h"
#include "chunk.h"
#include "debug.h"
#include "testing.h"
#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void repl();
static void runFile(const char* path);

#if TEST_LEVEL < COMPILER
#include "logging.h"
static void run_tests();
static bool test_op_constant(void);
static bool test_op_negate(void);
static bool test_op_arithmetic_1(void);
static bool test_op_arithmetic_2(void);
#endif

int main(int argc, char* argv[])
{
    if (argc == 1) {
        repl();
    }
    else if(argc == 2) {
        runFile(argv[1]);
    }
    else {
        fprintf(stderr, "Usage: clox [path]\n");
        exit(64);
    }

    //run_tests();

    printf("\n\n\n\n");
    //this is just hear for cleaner make commands lols
    return 0;
}

static void repl() {
    char line[1024];
    for(;;) {
        printf("> ");
        if(!fgets(line, sizeof(line), stdin)) {
            break;
        }
        interpret(line);
    }
}

static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");
    if(file == NULL) {
        fprintf(stderr, "Could not open the file \"%s\".\n", path);
        exit(74);
    }
    //get the length of the file
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(fileSize + 1);
    if(buffer == NULL) {
        fprintf(stderr, "Not enough memeory to read \"%s\".\n", path);
        exit(74);
    }
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if(bytesRead < fileSize) {
        fprintf(stderr, "Could completely read the file\"%s\".\n", path);
        fprintf(stderr, "Read %zu/%zu bytes\n", bytesRead, fileSize);
        exit(74);
    }
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;

}

static void runFile(const char* path) {
    char* source = readFile(path);
    InterpretResult result = interpret(source);
    free(source);

    if(result == INTERPRET_COMPILE_ERROR) exit(65);
    if(result == INTERPRET_RUNTIME_ERROR) exit(70);
}


#if TEST_LEVEL < COMPILER
static void run_tests() {
    RUN_TEST_MS(test_op_constant);
    RUN_TEST_MS(test_op_negate);
    RUN_TEST_MS(test_op_arithmetic_1);
    RUN_TEST_MS(test_op_arithmetic_2);

}
static bool test_op_constant(void)
{
    Chunk chunk;

    initVM();
    initChunk(&chunk);

    int constant = addConstant(&chunk, 1.2);

    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);
    writeChunk(&chunk, OP_RETURN, 123);
    
    InterpretResult ret = interpret(&chunk);

    freeVM();
    freeChunk(&chunk);

    return ret == INTERPRET_OK;
}

static bool test_op_negate(void) {
    Chunk chunk;

    initVM();
    initChunk(&chunk);

    int constant = addConstant(&chunk, 1.2);

    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);
    writeChunk(&chunk, OP_NEGATE, 123);
    writeChunk(&chunk, OP_RETURN, 123);
    
    InterpretResult ret = interpret(&chunk);


    freeVM();
    freeChunk(&chunk);

    return ret == INTERPRET_OK;
}

static bool test_op_arithmetic_1(void) {
    Chunk chunk;

    initVM();
    initChunk(&chunk);
    /*
    result = (1.2 + 3.4) / -5.6
    */

    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    constant = addConstant(&chunk, 3.4);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    writeChunk(&chunk, OP_ADD, 123);
    
    constant = addConstant(&chunk, 5.6);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);
    
    writeChunk(&chunk, OP_DIVIDE, 123);
    writeChunk(&chunk, OP_NEGATE, 123);

    writeChunk(&chunk, OP_RETURN, 123);

    InterpretResult ret = interpret(&chunk);

    freeVM();
    freeChunk(&chunk);

    return ret == INTERPRET_OK;

}

static bool test_op_arithmetic_2(void) {
    Chunk chunk;

    initVM();
    initChunk(&chunk);
    /*
    result = 1 + 2 * 3 - 4 / -5
    */

    int constant = addConstant(&chunk, 2.0);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    constant = addConstant(&chunk, 3.0);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    writeChunk(&chunk, OP_MULTIPLY, 123);
    
    constant = addConstant(&chunk, 4.0);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    constant = addConstant(&chunk, 5.0);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);
    writeChunk(&chunk, OP_NEGATE, 123);
    writeChunk(&chunk, OP_DIVIDE, 123);

    writeChunk(&chunk, OP_SUBTRACT, 123);

    constant = addConstant(&chunk, 1.0);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);
    writeChunk(&chunk, OP_ADD, 123);


    writeChunk(&chunk, OP_RETURN, 123);

    InterpretResult ret = interpret(&chunk);

    freeVM();
    freeChunk(&chunk);

    return ret == INTERPRET_OK;
}
#endif