#include "common.h"
#include "assert.h"
#include "chunk.h"
#include "debug.h"
#include "testing.h"
#define TEST_LEVEL 5

int main(int argc, char* argv[])
{
    if(argc > 0) {
        for(int i = 0; i < argc; i++)
            printf("argument: %s\n", argv[i]);
    }
    Chunk chunk;
    initChunk(&chunk);
    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);
    writeChunk(&chunk, OP_RETURN, 123);
    disassembleChunk(&chunk, "test chunk 1");
    freeChunk(&chunk);

    Chunk chunk2;

    initChunk(&chunk2);
    int redo = addConstant(&chunk2, 2.4);
    writeChunk(&chunk2, OP_CONSTANT, 223);
    writeChunk(&chunk2, redo, 223);
    writeChunk(&chunk2, OP_RETURN, 223);

    disassembleChunk(&chunk2, "test chunk 2");
    freeChunk(&chunk2);
    //this is just hear for cleaner make commands lols
    printf("\n\n\n\n");
    return 0;
}

#if TEST_LEVEL < COMPILER
static void test_op_constant(Chunk *chunk)
{
    int constant = addConstant(chunk, 1.2);
    writeChunk(chunk, OP_CONSTANT);
    writeChunk(chunk, constant);

}
#endif