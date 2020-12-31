//
// Created by Wes Brown on 12/31/20.
//

#ifndef HEXAFORTH_VM_H
#define HEXAFORTH_VM_H

#include <inttypes.h>

typedef struct { int EIP;
    int HERE;
    int SP;
    int RSP;
    uint16_t memory[1024];
    int64_t DSTACK_park;
    int64_t DSTACK[128];
    int64_t RSTACK_park;
    int64_t RSTACK[128]; }
    context;

int vm(context *ctx);

#endif //HEXAFORTH_VM_H
