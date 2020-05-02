#include <stdint.h>

#ifndef H_CpuModel
#include "../core/CpuModel.cpp"
#endif

#ifndef H_INSTR
#include "../Instruction.cpp"
#endif

#ifndef H_FLAG
#include "../core/Flags.h"
#endif

#ifndef H_PROVIDED
#include "../Provided.cpp"
#endif

int64_t instr_add(int64_t a, int64_t b)
{
    return a + b;
}

int64_t instr_and(int64_t a, int64_t b)
{
    return a & b;
}

int64_t instr_sub(int64_t a, int64_t b)
{
    return a - b;
}

int64_t instr_eor(int64_t a, int64_t b)
{
    return a ^ b;
}

int64_t instr_orr(int64_t a, int64_t b)
{
    return a | b;
}

int64_t instr_mul(int64_t a, int64_t b)
{
    return a * b;
}

int64_t instr_sdiv(int64_t a, int64_t b)
{
    return a / b;
}

int64_t instr_lsl(int64_t a, int8_t s)
{
    return a << s;
}

int64_t instr_lsr(int64_t a, int8_t s)
{
    return a >> s;
}

// TODO Rigorously test flags, this implementation is quite dodgy
int64_t instr_subs(int64_t a, int64_t b, uint8_t * flag)
{
    int64_t r = a - b;

    if (r > a)  *flag |= OVERFLOW;
    if (r == 0) *flag |= ZERO;

    return r;
}

void instr_prnt(CpuModel * mdl, Instruction * instr)
{
    int16_t reg = instr->decodeRd();
    int64_t value = mdl->getRegister(reg);
    printf("X%d\t: %ld\n", reg, value);
}

void instr_prnl(CpuModel * mdl, Instruction * instr)
{
    printf("\n");
}

void instr_dump(CpuModel * mdl, Instruction * instr)
{
    printf("Registers:\n");
    for (int r = 0; r < REGISTER_MAX; r++)
    {
        uint64_t value = mdl->getRegister(r);
        printf("X%d\t%#016lx (%ld)\n", r, value, value);
    }

    printf("\nStack\n");
    Provided::hexdump(stdout, mdl->getMemory()->getBasePtr(), STACK_SIZE);

    printf("\nMain Memory\n");
    Provided::hexdump(stdout, mdl->getMemory()->getBasePtr(), MEMORY_SIZE);

    printf("\nInstructions\n");
    InstructionLL * linkedList = mdl->getHead();
    while ((linkedList = linkedList->getNext()))
    {
        Instruction * cur = linkedList->getInstruction();
        if (cur->isValid())
        {
            printf(cur == instr ? " **\t" : "\t");
            cur->decode();
        }
    }
}

void instr_halt(CpuModel * mdl, Instruction * instr)
{
    instr_dump(mdl, instr);
    exit(0);
}

/**
Instruction * instr;
Mdl * mdl;
CpuController * ctrl = new CpuController(mdl);
ctrl->process(mdl);

**/
