#include <stdint.h>

#ifndef H_CpuModel
#include "../core/CpuModel.cpp"
#endif

#ifndef H_FLAG
#include "../core/Flags.h"
#endif

void instr_branch(CpuModel * mdl, int32_t offset)
{
    // Clear our flags
    uint8_t * flags = mdl->getFlagsPtr();
    *flags &= 0;

    bool forward = offset > 0;

    InstructionLL * pc = mdl->getPc();
    for (int c = 0; c < abs(offset); c ++)
    {
        pc = forward
            ? pc->getNext()
            : pc->getPrev();
    }

    mdl->setPc(pc);
}

bool instr_branch_assert(CpuModel * mdl, int8_t test)
{
    int flags = *mdl->getFlagsPtr();

    switch(test)
    {
        /* EQ */ case 0x0: return flags & ZERO;
        /* NE */ case 0x1: return ! (flags & ZERO);
        /* HS */ case 0x2: return 0;
        /* LO */ case 0x3: return 0;
        /* MI */ case 0x4: return 0;
        /* PL */ case 0x5: return 0;
        /* VS */ case 0x6: return 0;
        /* VC */ case 0x7: return 0;
        /* HI */ case 0x8: return 0;
        /* LS */ case 0x9: return 0;
        /* GE */ case 0xA: return ! (flags & NEGATIVE) || (flags & ZERO);
        /* LT */ case 0xB: return (flags & NEGATIVE);
        /* GT */ case 0xC: return ! (flags & NEGATIVE);
        /* LE */ case 0xD: return (flags & NEGATIVE) || (flags & ZERO);
    
    }

    return 0;
}
