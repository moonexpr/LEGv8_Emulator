

#include "Instruction.h"

Instruction::Instruction(int32_t code)
{
    this->code = code;
}

Instruction::~Instruction()
{

}


bool Instruction::isValid()
{
    return this->getType() != Unknown;
}

int32_t Instruction::mask(int start, int end)
{
    unsigned int shift = 32 - end;
    unsigned int range = end - start;
    return (this->code >> shift) & ~(0xFFFFFFFF << range);
}

const char * Instruction::getName()
{
    std::map<int, const char *> names = instructionNames();
    int32_t code = this->getOpcode();
    std::map<int, const char *>::iterator iter = names.find(code);

    if (iter == names.end())
    {
        return "UNSUPPORTED";
    }

    return iter->second;
}
int32_t Instruction::getOpcode()
{
    switch (this->getType())
    {
        case Register:
            return this->mask(0, 11);
            break;

        case Immediate:
            return this->mask(0, 10);
            break;

        case Load:
            return this->mask(0, 11);
            break;

        case Branch:
            return this->mask(0, 6);
            break;

        case CondBranch:
            return this->mask(0, 8);
            break;

        case ImmediateWide:
            return this->mask(0, 11);
            break;

        case Unknown:
            return this->mask(0, 11);
            break;
    }

    return 0;
}

int32_t Instruction::decodeRt()
{
    return this->mask(27, 32);
}

int32_t Instruction::decodeRd()
{
    return this->mask(27, 32);
}

int32_t Instruction::decodeRn()
{
    return this->mask(22, 27);
}

int32_t Instruction::decodeShamt()
{
    return this->mask(16, 22);
}

int32_t Instruction::decodeRm()
{
    return this->mask(11, 16);
}

int32_t Instruction::decodeALU()
{
    return Provided::sign_extend(this->mask(10, 22), 11);
}

int32_t Instruction::decodeBRAddr()
{
    return Provided::sign_extend(this->mask(6, 32), 25);
}

int32_t Instruction::decodeDTAddr()
{
    return this->mask(12, 20);
}

int32_t Instruction::decodeCondAddr()
{
    return Provided::sign_extend(this->mask(8, 27), 18);
}

int32_t Instruction::decodeOp()
{
    return this->mask(21, 22);
}

enum InstType Instruction::getType()
{
    std::map<int, enum InstType> map;

    /* ADD */
    map[0b10001011000]      = Register;
    /* ADDI */
    map[0b1001000100]       = Immediate;
    /* AND */
    map[0b10001010000]      = Register;
    /* ANDI */
    map[0b1001001000]       = Immediate;
    /* B */
    map[0b000101]           = Branch;
    /* B.C */
    map[0b01010100]         = CondBranch;
    /* BL */
    map[0b100101]           = Branch;
    /* BR */
    map[0b11010110000]      = Register;
    /* CBNZ */
    map[0b10110101]         = CondBranch;
    /* CBZ */
    map[0b10110100]         = CondBranch;
    /* EOR */
    map[0b11001010000]      = Register;
    /* EORI */
    map[0b1101001000]       = Immediate;
    /* LDUR */
    map[0b11111000010]      = Load;
    /* LSL */
    map[0b11010011011]      = Register;
    /* LSR */
    map[0b11010011010]      = Register;
    /* MUL */
    map[0b10011011000]      = Register;
    /* ORR */
    map[0b10101010000]      = Register;
    /* ORRI */
    map[0b1011001000]       = Immediate;
    /* STUR */
    map[0b11111000000]      = Load;
    /* SUB */
    map[0b11001011000]      = Register;
    /* SUBI */
    map[0b1101000100]       = Immediate;
    /* SUBIS */
    map[0b1111000100]       = Immediate;
    /* SUBS */
    map[0b11101011000]      = Register;
    /* PRNT */
    map[0b11111111101]      = Register;
    /* PRNL */
    map[0b11111111100]      = Register;
    /* DUMP */
    map[0b11111111110]      = Register;
    /* HALT */
    map[0b11111111111]      = Register;

    int op6 = this->mask(0, 6);
    int op8 = this->mask(0, 8);
    int op9 = this->mask(0, 9);
    int op10 = this->mask(0, 10); 
    int op11 = this->mask(0, 11);

    if (map.find(op6) != map.end())
        return map[op6];

    if (map.find(op8) != map.end())
        return map[op8];

    if (map.find(op9) != map.end())
        return map[op9];

    if (map.find(op10) != map.end())
        return map[op10];

    if (map.find(op11) != map.end())
        return map[op11];

    return Unknown;
}
