#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, char* argv[])
{
    Chunk chunk;
    initChunk(&chunk);
    writeChunk(&chunk, OP_RETURN);

    disassembleChunk(&chunk, "test chunk");
    printf("Chunk allocated at %p\nContains value: %d", &chunk, chunk.code[0]);
    freeChunk(&chunk);
    return 0;
}