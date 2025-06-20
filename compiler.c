#include <stdio.h>
#include "common.h"
#include "compiler.h"
#include "scanner.h"
#include "vm.h"


bool compile(const char* source, Chunk* chunk) {
    initScanner(source);
    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression.");
    int line = -1;
    for(;;) {
        Token token = scanToken();
        if(token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        }
        else {
            printf("    | ");
        }
        printf("%2d '%.*s'\n", token.type, token.length, token.start);

        if(token.type == TOKEN_EOF) break;
    }

}