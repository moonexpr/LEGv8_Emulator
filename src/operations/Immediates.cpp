#include <stdint.h>

#ifndef H_FLAG
#include "../core/Flags.h"
#endif

int64_t instr_addi(int64_t a, int64_t b)
{
    return a + b;
}

int64_t instr_andi(int64_t a, int64_t b)
{
    return a & b;
}

int64_t instr_subi(int64_t a, int64_t b)
{
    return a - b;
}

int64_t instr_eori(int64_t a, int64_t b)
{
    return a ^ b;
}

int64_t instr_orri(int64_t a, int64_t b)
{
    return a | b;
}

// TODO Rigorously test this implementation (really dodgy)
int64_t instr_subis(int64_t a, int64_t b, uint8_t * flag)
{
    int64_t r = a - b;

    if (r > a)  *flag |= OVERFLOW;
    if (r == 0) *flag |= ZERO;

    return r;
}
