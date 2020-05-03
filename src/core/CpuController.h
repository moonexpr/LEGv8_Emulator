#include <iostream>
#ifndef H_CpuModel
#include "CpuModel.cpp"
#endif

#ifndef H_INSTR
#include "../Instruction.cpp"
#endif

#define H_CpuController

class CpuController
{
    private:
    CpuModel * mdl;

    public:

    typedef void (CpuController::*OpFunc)(Instruction * instr);

    typedef int64_t (ArithmeticFunc)(int64_t a, int64_t b);
    typedef void (CustomFunc)(CpuModel * mdl, Instruction * instr);

    CpuController(CpuModel * mdl);
    void process(Instruction * instr);
    void batch(Instruction ** arrInstr);
    OpFunc getOperationFunc(Instruction * instr);

    void immediateHandle(Instruction * instr);
    void registerHandle(Instruction * instr);
    void branchHandle(Instruction * instr);
    void conditionalHandle(Instruction * instr);
    void memoryHandle(Instruction * instr);
    void unknownHandle(Instruction * instr);
};

