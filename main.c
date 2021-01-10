#include <stdlib.h>
#include "vm.h"
#include "vm_test.h"
#include "vm_debug.h"
#include "compiler.h"

static hexaforth_test TESTS[] = {
        {.label = "8-bit literals",
         .input = "1 2 3 4",
         .dstack = "1 2 3 4"},
        {.label = "Context cleared after tests",
         .input = "6 7 8 9 10",
         .dstack = "6 7 8 9 10"},
        {.label = "invert ( a -- ~a )",
         .input = "1 2 1024 invert",
         .dstack = "1 2 -1024"},
        {.label = "negative literals",
         .input = "-1 -2 -3 -4 -5",
         .dstack = "-1 -2 -3 -4 -5"},
        {.label = "16-bit literals",
         .input = "32768 65535 -32768 -65535 49151 -49151",
         .dstack = "32768 65535 -32768 -65535 49151 -49151"},
        {.label = "32-bit literals",
         .input = "4294967295 -4294967295 "
                  "4294967040 -4294967040 "
                  "2147483520 -2147483520",
         .dstack = "4294967295 -4294967295 "
                   "4294967040 -4294967040 "
                   "2147483520 -2147483520"},
        {.label = "48-bit literals",
         .input = "140737488355328 -140737488355328",
         .dstack = "140737488355328 -140737488355328"},
        {.label = "swap ( a b -- b a )",
         .input = "5 4 2 1 swap",
         .dstack = "5 4 1 2"},
        {.label = "over ( a b -- a b a )",
         .input = "5 4 3 2 over",
         .dstack = "5 4 3 2 3"},
        {.label = "nip ( a b -- b )",
         .input = "5 4 3 2 nip",
         .dstack = "5 4 2"},
        {.label = "drop ( a b -- a )",
         .input = "5 4 3 2 drop",
         .dstack = "5 4 3"},
        {.label = "+ ( a b -- a+b )",
         .input = "10 15 +",
         .dstack = "25"},
        {.label = "1+ ( a -- a+1 )",
         .input = "1 2 3 1+",
         .dstack = "1 2 4"},
        {.label = "2+ ( a -- a+2 )",
         .input = "1 2 3 2+",
         .dstack = "1 2 5"},
        {.label = "lshift ( a b -- a << b )",
         .input = "2 1 lshift",
         .dstack = "4"},
        {.label = "rshift ( a b -- a >> b )",
         .input = "2 1 rshift",
         .dstack = "1"},
        {.label = "rshift ( a b -- a >> b )",
         .input = "4 1 rshift",
         .dstack = "2"},
        {.label = "- ( a b -- b-a )",
         .input = "500 300 -",
         .dstack = "200"},
        {.label = "2* ( a -- a << 1 )",
         .input = "512 2*",
         .dstack = "1024"},
        {.label = "2/ ( a -- a >> 1 )",
         .input = "512 2/",
         .dstack = "256"},
        {.label = "@ ( a -- [a] )",
         .init = "1024 -1024 2048 -2048 4096 -4096",
         .input = "0 @ 4 @ 8 @ 12 @ 16 @ 20 @",
         .dstack = "1024 -1024 2048 -2048 4096 -4096"},
        {.label = "! ( a b -- )",
         .init = "0 0",
         .input = "1024 0 ! -1024 4 ! 0 @ 4 @",
         .dstack = "1024 -1024"},
        {.label = ">r ( a -- R: a )",
         .input = "1024 >r",
         .dstack = "",
         .rstack = "1024"},
        {.label = "r@ ( R:a -- a R: a )",
         .input = "1024 >r r@ r@",
         .dstack  = "1024 1024",
         .rstack = "1024" },
        {.label = "r> ( R:a -- a )",
         .input = "1024 >r r@ 2* r>",
         .dstack = "2048 1024",
         .rstack = ""},
        {.label = "48-bit string stack literal",
         .input = "'deadbe'",
         .dstack = "111473265304932 1"},
        {.label = "64-bit string stack literal",
         .input = "'deadbeef'",
         .dstack = "7378415037781730660 1"},
        {.label = "96-bit string stack literals",
         .input = "'deadbeefdead'",
         .dstack = "1684104548 7378415037781730660 2"},
        {.label = "8emit",
         .input = "'deadbe' drop 8emit",
         .io_expected = "deadbe" },
        {.label = "8emit",
         .input = "'deadbeef' drop 8emit",
         .io_expected = "deadbeef" },
        {.label = "8emit",
         .input = "'deadbeefdead' drop 8emit swap 8emit",
         .io_expected = "deadbeefdead"},
        {.label = "key",
         .input = "key emit key emit key emit key emit",
         .io_input = "dead",
         .io_expected = "dead"},
        {.input = ""},
};

void load_hex(const char* filepath, context *ctx) {
    FILE* hexfile = fopen(filepath, "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    if (hexfile == NULL)
        exit(EXIT_FAILURE);

    ctx->HERE=0;

    while ((read = getline(&line, &len, hexfile)) != -1) {
        *(uint32_t*)&(ctx->memory[ctx->HERE]) = (uint32_t)strtol(line, NULL, 16);
        if (ctx->memory[ctx->HERE] != 0) {
            printf("[0x%0.4x] ", ctx->HERE);
            debug_instruction(*(instruction*)&ctx->memory[ctx->HERE]);
        }
        if (ctx->memory[ctx->HERE+1] != 0) {
            printf("[0x%0.4x] ", (ctx->HERE+1));
            debug_instruction(*(instruction*)&ctx->memory[ctx->HERE+1]);
        }
        ctx->HERE+=2;
    }

    fclose(hexfile);
    if (line)
        free(line);
}

int main() {
    if(init_opcodes()) {
        execute_tests(TESTS);
    };
    generate_basewords_fs();
    // context *ctx = calloc(sizeof(context), 1);
    // load_hex("../build/test.hex", ctx);
    // ctx->EIP=0x462C / 2;
    // vm(ctx);

    // context *ctx = malloc(sizeof(context));
    // ctx->HERE = 0;
    // generate_test_program(ctx);
    // vm(ctx);
}
