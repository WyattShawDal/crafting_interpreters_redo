#include "common.h"
#include "vm.h"
#include "debug.h"
#include "memory.h"  // Add this include
#include "compiler.h"

#include <stdio.h>
VM vm; 

static InterpretResult run();
static void resetStack();

void initVM() {
    resetStack();
}

void freeVM() {
    FREE_ARRAY(Value, vm.stack, vm.stackCapacity);
}

#if TEST_LEVEL < 5
InterpretResult interpret(Chunk* chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code; //vm instruction pointer
    return run();
}
#endif

InterpretResult interpret(const char* source) {
    compile(source);
    return INTERPRET_OK;
}

static InterpretResult run() {
    #define READ_BYTE() (*vm.ip++)
    #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
    // turns out we can pass operators as arguments to macros.. lol
    #define BINARY_OP(op) \
        do { \
            double b = pop(); \
            double a = pop(); \
            push(a op b); \
        } while (0)

    for(;;) {
        #ifdef DEBUG_TRACE_EXECUTION
        printf("STACK:");
        for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        if(vm.stackTop == vm.stack) { //if we're still pointing to the base of the stack
            printf("[ NULL ]"); //stack is empty
        }
        printf("\n");
        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
        //subtraction done to convert offset into a relative offset from the beginning of the byte code
        #endif
        uint8_t instruction;
        switch(instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                push(READ_CONSTANT());
                break;
            }
            case OP_NEGATE: push(-pop()); break;
            case OP_ADD: BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break; 
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE: BINARY_OP(/); break;
            case OP_RETURN: {
                printValue(pop());
                printf("\n");
                //always ends with an op_return
                return INTERPRET_OK;
            }
            default: {
                printf("Unknown opcode %d\n", instruction);
                return INTERPRET_RUNTIME_ERROR;
            }
        }
    }
    #undef READ_CONSTANT
    #undef READ_BYTE
    #undef BINARY_OP
    return INTERPRET_RUNTIME_ERROR; //this is unreachable, but we need to return something
}

static void resetStack() {
    vm.stack = NULL;
    vm.stackCapacity = 0;
    vm.stackTop = vm.stack;
}

void push(Value value) {
    // Check if we need to grow the stack
    if (vm.stackTop - vm.stack >= vm.stackCapacity) {
        int oldCapacity = vm.stackCapacity;
        vm.stackCapacity = GROW_CAPACITY(oldCapacity);
        size_t stackOffset = vm.stackTop - vm.stack;  // Save offset before realloc
        vm.stack = GROW_ARRAY(Value, vm.stack, oldCapacity, vm.stackCapacity);
        vm.stackTop = vm.stack + stackOffset;  // Restore stackTop after realloc
    }
    
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}