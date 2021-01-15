//
// Created by Wes Brown on 12/31/20.
//

#ifndef HEXAFORTH_VM_H
#define HEXAFORTH_VM_H

#include <inttypes.h>
#include <stdio.h>
#include "vm_opcodes.h"

typedef struct { int EIP;
    int        HERE;
    int        SP;
    int        RSP;
    uint16_t   memory[65536];
    int64_t    DSTACK_park;
    int64_t    DSTACK[128];
    int64_t    RSTACK_park;
    int64_t    RSTACK[128];
    FILE       *OUT;
    FILE       *IN;
    char*      meta[32768];
    word_node* words; } context;

uint8_t clz(uint64_t N);
int vm(context *ctx);

#endif //HEXAFORTH_VM_H
