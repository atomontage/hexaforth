//
// Created by Wes Brown on 12/31/20.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "compiler.h"
#include "vm_debug.h"

// Lowest level compiler call that writes an instruction and increments the
// HERE pointer to the next CELL.
void insert_opcode(context *ctx, instruction op) {
    printf("HERE[%d]: ", ctx->HERE);
    debug_instruction(op);
    memcpy(&ctx->memory[ctx->HERE], &op, 2);
    ctx->HERE++;
}

// This function is used by `compile_word` to determine when it's reached
// the end of an instruction stream that it needs to write out.
bool is_null_instruction(instruction ins) {
    return (*(uint16_t *) (void *) &ins == 0);
}

// Look up a string in our opcodes table, and if found, write the opcodes
// associated with the word into the image.
void compile_word(context *ctx, const char* word) {
    instruction* instructions = lookup_word(word);
    if (instructions) {
        int idx=0;
        instruction ins = instructions[idx];
        while (!is_null_instruction(ins)) {
            insert_opcode(ctx, ins);
            idx++;
            ins = instructions[idx];
        }
    } else {
        printf("ERROR: '%s' not found!\n", word);
    }
}

// This function takes a 64-bit signed integer and compiles it as a sequence
// of literal, shift, and invert instructions needed to reconstruct the
// literal on top of the stack.
void insert_literal(context *ctx, int64_t n) {
    printf("COMPILE_LITERAL: %lld\n", n);
    uint64_t acc = llabs(n);         // Our accumulator, really a deccumlator
    bool negative = n < 0;
    bool first_ins = true;

    // Create our template literal instruction.  We may produce one or more
    // literal instructions for numbers that are larger than LIT_BITS.
    instruction literal = {};
    literal.lit.lit_f = true;
    literal.lit.lit_add = false;
    uint8_t shifts = 0;

    // We loop until we have zero in our accumulator; but we always allow
    // a first loop for when n=0.
    while (acc != 0 || n == 0) {
        // This will cause NUM_BITS of acc to be stored in lit_v, with
        // correct signedness. As it's signed, it will be NUM_BITS+1 in
        // size.  We also store how many NUM_BITS left shifts the VM needs
        // to do to get the correct magnitude.
        literal.lit.lit_v = acc;
        // Check if we still have more literal instructions to write out.
        if (acc > LIT_UMASK) {
            acc = acc >> LIT_BITS;
        } else {
            // We're done constructing our literal instructions, so we force
            // acc to 0, and indicate that the VM should increment the stack
            // pointer.
            acc = 0;
        }
        // We don't write out intermediate literals that are zeroes, unless
        // the literal we want to write out is a literal zero.
        if (literal.lit.lit_v != 0 || n == 0) {
            literal.lit.lit_shifts = shifts;
            if (!first_ins) literal.lit.lit_add = true;
            // We are past the first instruction, so additional instructions
            // additive, adding the literal to the last literal.
            first_ins = false;
            // Write the encoded literal to our image.
            insert_opcode(ctx, literal);
        } else {
            dprintf("HERE[%d]: Skipping LIT instruction as lit_v == 0, shift=%d\n",
                    ctx->HERE, shifts);
        }
        shifts++;
    }
    // We have a negative number, so we need to apply two's complement to the
    // literal we've built up.  So we insert an instruction to invert the results.
    if (negative) compile_word(ctx, "invert");
}

