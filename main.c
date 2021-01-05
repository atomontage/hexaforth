#include <stdlib.h>
#include "vm.h"
#include "vm_test.h"
#include "compiler.h"

void generate_test_program(context *ctx) {
    compile(ctx, "1 2 3 4 + 5 dup 6 drop drop 7 over nip 1+");
}

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
        {.input = "",
         .dstack = ""}
};

int main() {
    execute_tests(TESTS);
    // context *ctx = malloc(sizeof(context));
    // ctx->HERE = 0;
    // generate_test_program(ctx);
    // vm(ctx);
}
