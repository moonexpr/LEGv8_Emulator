#include <cstdint>
#include <iostream>
#include <stdlib.h>

#define MEMORY_SIZE 4096

class MemModel
{
    private:
    int8_t * main;

    public:
    MemModel();
    ~MemModel();

    int8_t * getBasePtr();

    uint64_t readDoubleWord(int32_t addr);
    void writeDoubleWord(int32_t addr, uint64_t dw);
    uint32_t readWord(int32_t addr);
    void writeWord(int32_t addr, uint32_t w);
    uint16_t readHalfWord(int32_t addr);
    void writeHalfWord(int32_t addr, uint16_t hw);
};

MemModel::MemModel()
{
    this->main = (int8_t *) malloc(MEMORY_SIZE * 8);
}

MemModel::~MemModel()
{
    free(this->main);
}


int8_t * MemModel::getBasePtr()
{
    return this->main;
}

uint64_t MemModel::readDoubleWord(int32_t addr)
{
    uint64_t highOrder = (0ULL | this->readWord(addr)) << 32;
    uint64_t lowOrder  = (0ULL | this->readWord(addr + 4));

    return highOrder | lowOrder;
}

void MemModel::writeDoubleWord(int32_t addr, uint64_t dw)
{
    int32_t highOrder = dw >> 32;
    int32_t lowOrder  = dw & 0xFFFFFFFF;

    this->writeWord(addr, highOrder);
    this->writeWord(addr + 4, lowOrder);
}

uint32_t MemModel::readWord(int32_t addr)
{
    uint32_t highOrder = (this->readHalfWord(addr)) << 16;
    uint32_t lowOrder  = (this->readHalfWord(addr + 2));

    return highOrder | lowOrder;
}

void MemModel::writeWord(int32_t addr, uint32_t w)
{
    uint16_t highOrder = w >> 16;
    uint16_t lowOrder  = w & 0xFFFF;

    this->writeHalfWord(addr, highOrder);
    this->writeHalfWord(addr + 2, lowOrder);
}

uint16_t MemModel::readHalfWord(int32_t addr)
{
    int8_t * baseAddr = this->main + addr;

    uint16_t highOrder = *(baseAddr    ) << 8;
    uint16_t lowOrder  = *(baseAddr + 1) & 0xFF;

    return highOrder | lowOrder;
}

void MemModel::writeHalfWord(int32_t addr, uint16_t hw)
{
    int8_t * baseAddr = this->main + addr;


    *(baseAddr    ) = hw >> 8;
    *(baseAddr + 1) = hw & 0xFF;
}
