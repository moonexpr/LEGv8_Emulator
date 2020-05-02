
#ifndef H_CpuModel
#include "Register.h"
#define H_CpuModel
#endif

#define STACK_SIZE 512
#include "CpuModel.h"

CpuModel::CpuModel() 
{
    for (int r = 0; r < REGISTER_MAX; r ++)
    {
        this->registers[r] = 0ULL;
    }

    this->memMdl = new MemModel();
    this->setRegister(SP, STACK_SIZE);
    this->setRegister(FP, STACK_SIZE);
}

CpuModel::~CpuModel() { }

InstructionLL * CpuModel::getPc()
{
    return this->pc;
}

void CpuModel::setPc(InstructionLL * pc)
{
    this->pc = pc;
}

void CpuModel::forwardPc()
{
    this->pc = this->pc->getNext();
}

void CpuModel::addToTail(Instruction * instr)
{
    if (!this->head)
    {
        this->head = new InstructionLL(instr, 1);
        this->tail = this->head;
        this->pc = this->head;
        return;
    }

    InstructionLL * linkedList = new InstructionLL(instr, this->tail->getNumber() + 1);
    linkedList->link(this->tail, 0);
    this->tail = linkedList;
}

MemModel * CpuModel::getMemory()
{
    return this->memMdl;
}

void CpuModel::setRegister(uint reg, int64_t value)
{
    if (reg >= XZR)
        return;

    this->registers[reg] = value;
    return;
}


int64_t CpuModel::getRegister(uint reg)
{
    if (reg >= XZR)
        return 0;

    return this->registers[reg];
}

uint8_t * CpuModel::getFlagsPtr()
{
    return &this->flags;
}
