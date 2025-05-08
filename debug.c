#include <stdio.h>

#include "debug.h"

//todo finish implementing this
void disassembleChunk(Chunk *chunk, const char *name) {
    printf("== %s ==\n", name);

    for(int offset = 0; offset < chunk->count;offset++){
        offset = disassembleInstruction(chunk, offset);
    }
}