#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"
#include <stdint.h>

#define STACK_INITIAL_CAPACITY 256

typedef struct {
    Chunk *chunk;
    uint8_t *ip;
    Value* stack;  // Changed from fixed array to pointer
    Value* stackTop;
    int stackCapacity;  // Track current capacity
}VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(const char* source);
void push(Value value);
Value pop();


#endif