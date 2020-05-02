#include <cstdint>
#include <stdlib.h>

#ifndef H_INSTRLL
#include "../InstrucionDLL.cpp"
#endif

#include "../memory/Memory.cpp"

#define REGISTER_MAX 32

class CpuModel
{
    private:
    MemModel * memMdl;
    int64_t registers[32];
    InstructionLL * head;
    InstructionLL * tail;
    InstructionLL * pc;

    uint8_t flags;
    
    public:

    CpuModel();
    ~CpuModel();

    InstructionLL * getHead();
    InstructionLL * getPc();
    void setPc(InstructionLL * pc);
    void forwardPc();
    void addToTail(Instruction * instr);

    MemModel * getMemory();

    void setRegister(uint reg, int64_t value);
    int64_t getRegister(uint reg);
    uint8_t * getFlagsPtr();
};

