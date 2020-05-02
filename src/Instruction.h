#include <map>
#include <sys/types.h>
#include <iostream>
#include <string.h>


#ifndef H_PROVIDED
#include "Provided.cpp"
#endif

#define H_INSTR

std::map<int, const char *> instructionNames()
{
    std::map<int, const char *> names;

    names[0b10001011000] 	= "ADD";
    names[0b1001000100 ] 	= "ADDI";
    names[0b10001010000] 	= "AND";
    names[0b1001001000 ] 	= "ANDI";
    names[0b000101     ] 	= "B";
    names[0b01010100   ] 	= "B.C";
    names[0b100101     ] 	= "BL";
    names[0b11010110000] 	= "BR";
    names[0b10110101   ] 	= "CBNZ";
    names[0b10110100   ] 	= "CBZ";
    names[0b11001010000] 	= "EOR";
    names[0b1101001000 ] 	= "EORI";
    names[0b11111000010] 	= "LDUR";
    names[0b11010011011] 	= "LSL";
    names[0b11010011010] 	= "LSR";
    names[0b10011011000] 	= "MUL";
    names[0b10101010000] 	= "ORR";
    names[0b1011001000 ] 	= "ORRI";
    names[0b11111000000] 	= "STUR";
    names[0b11001011000] 	= "SUB";
    names[0b1101000100 ] 	= "SUBI";
    names[0b1111000100 ] 	= "SUBIS";
    names[0b11101011000] 	= "SUBS";
    names[0b11111111101] 	= "PRNT";
    names[0b11111111100] 	= "PRNL";
    names[0b11111111110] 	= "DUMP";
    names[0b11111111111] 	= "HALT";

    return names;
}

std::map<int, const char *> conditionalNames()
{
    std::map<int, const char *> names;

    names[0x0]	= "EQ";
    names[0x1]	= "NE";
    names[0x2]	= "HS";
    names[0x3]	= "LO";
    names[0x4]	= "MI";
    names[0x5]	= "PL";
    names[0x6]	= "VS";
    names[0x7]	= "VC";
    names[0x8]	= "HI";
    names[0x9]	= "LS";
    names[0xa]	= "GE";
    names[0xb]	= "LT";
    names[0xc]	= "GT";
    names[0xd]	= "LE";

    return names;
}

enum InstType
{
    Register,
    Immediate,
    Load,
    Branch,
    CondBranch,
    ImmediateWide,
    Unknown
};

class Instruction
{
    private:
    int32_t code;
    enum InstType type;
    int64_t sign_extend(int64_t value, int bits);

    public:
    Instruction(int32_t code);
    ~Instruction();

    bool isValid();
    int32_t mask(int start, int end);
    void decode();
    InstType getType();

    const char * getName();
    int32_t getOpcode();
    int32_t decodeRt();
    int32_t decodeRd();
    int32_t decodeRn();
    int32_t decodeShamt();
    int32_t decodeRm();
    int32_t decodeALU();
    int32_t decodeDTAddr();
    int32_t decodeBRAddr();
    int32_t decodeCondAddr();
    int32_t decodeOp();
};
