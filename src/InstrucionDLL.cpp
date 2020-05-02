#ifndef H_INSTR
#include "Instruction.cpp"
#endif

#define H_INSTRLL

class InstructionLL {
    private:
    InstructionLL * prev;
    InstructionLL * next;
    Instruction * instr;
    int number;

    public:
    InstructionLL(Instruction * instr, int number);
    ~InstructionLL();

    void link(InstructionLL * prev, InstructionLL * next);
    void unlink();

    Instruction * getInstruction();
    int getNumber();
    void setNext(InstructionLL * next);
    void setPrev(InstructionLL * prev);
    InstructionLL * getNext();
    InstructionLL * getPrev();
};

InstructionLL::InstructionLL(Instruction * instr, int number)
{
    this->instr = instr;
    this->number = number;
}

InstructionLL::~InstructionLL()
{
    delete this->instr;
}

void InstructionLL::link(InstructionLL * prev, InstructionLL * next)
{
    this->next = next;
    this->prev = prev;

    if (next) this->next->setPrev(this);
    if (prev) this->prev->setNext(this);
}

void InstructionLL::unlink()
{
    // If we're a singleton, do nothing.
    if (this->next == this->prev)
        return;

    if (this->next) this->next->setPrev(this->prev);
    if (this->prev) this->prev->setNext(this->next);

    delete this;
}

void InstructionLL::setNext(InstructionLL * next)
{
    this->next = next;
}

void InstructionLL::setPrev(InstructionLL * prev)
{
    this->prev = prev;
}

Instruction * InstructionLL::getInstruction()
{
    return this->instr;
}

int InstructionLL::getNumber()
{
    return this->number;
}

InstructionLL * InstructionLL::getNext()
{
    return this->next;
}

InstructionLL * InstructionLL::getPrev()
{
    return this->prev;
}
