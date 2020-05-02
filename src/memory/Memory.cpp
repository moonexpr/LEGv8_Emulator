#include <cstdint>
#include <stdlib.h>

#define MEMORY_SIZE 2176

class MemModel
{
    private:
    int8_t * main;

    public:
    MemModel();
    ~MemModel();

    int64_t readDoubleWord(int8_t addr);
    void writeDoubleWord(int8_t addr, int64_t dw);
    int32_t readWord(int8_t addr);
    void writeWord(int8_t addr, int32_t w);
    int16_t readHalfWord(int8_t addr);
    void writeHalfWord(int8_t addr, int16_t hw);
};

MemModel::MemModel()
{
    this->main = (int8_t *) malloc(MEMORY_SIZE * 8);
}

MemModel::~MemModel()
{
    free(this->main);
}


int64_t MemModel::readDoubleWord(int8_t addr)
{
    int64_t highOrder = (0LL | this->readWord(addr)) << 32;
    int64_t lowOrder  = (0LL | this->readWord(addr + 4));

    return highOrder | lowOrder;
}

void MemModel::writeDoubleWord(int8_t addr, int64_t dw)
{
    int32_t highOrder = dw >> 32;
    int32_t lowOrder  = dw & 0xFFFF;

    this->writeWord(addr, highOrder);
    this->writeWord(addr + 4, lowOrder);
}

int32_t MemModel::readWord(int8_t addr)
{
    int32_t highOrder = this->readHalfWord(addr) << 16;
    int32_t lowOrder  = this->readHalfWord(addr + 2);

    return highOrder | lowOrder;
}

void MemModel::writeWord(int8_t addr, int32_t w)
{
    int8_t highOrder = w >> 16;
    int8_t lowOrder  = w & 0xFF;

    this->writeHalfWord(addr, highOrder);
    this->writeHalfWord(addr + 2, lowOrder);
}

int16_t MemModel::readHalfWord(int8_t addr)
{
    int8_t * baseAddr = this->main + addr;
    int8_t highOrder = *(baseAddr    ) << 8;
    int8_t lowOrder  = *(baseAddr + 1);

    return highOrder | lowOrder;
}

void MemModel::writeHalfWord(int8_t addr, int16_t hw)
{
    int8_t * baseAddr = this->main + addr;

    *(baseAddr    ) = hw >> 8;
    *(baseAddr + 4) = hw & 0xF;
}
