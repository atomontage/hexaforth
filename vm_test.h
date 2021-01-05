//
// Created by Wes Brown on 1/1/21.
//

#ifndef HEXAFORTH_VM_TEST_H
#define HEXAFORTH_VM_TEST_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    const char* label;
    const char* init;
    const char* input;
    const char* dstack;
    const char* rstack;
} hexaforth_test;

typedef struct {
    uint64_t sz;
    int64_t* elems;
} counted_array;

bool execute_test(hexaforth_test test);
bool execute_tests(hexaforth_test* tests);

#endif //HEXAFORTH_VM_TEST_H
