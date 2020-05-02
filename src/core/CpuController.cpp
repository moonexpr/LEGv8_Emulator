#include <map>

#include "CpuController.h"
#include "../operations/Register.cpp"
#include "../operations/Immediates.cpp"
#include "../operations/Branch.cpp"

CpuController::CpuController(CpuModel * mdl)
{
    this->mdl = mdl;
}

void CpuController::process(Instruction * instr)
{
    std::cout << instr->getName() << std::endl;
    OpFunc callback = getOperationFunc(instr);
    (*this.*callback)(instr);
}

CpuController::OpFunc CpuController::getOperationFunc(Instruction * instr)
{
    switch (instr->getType())
    {
        case Register       : return &CpuController::registerHandle;
        case Immediate      : return &CpuController::immediateHandle;
        case Branch         : return &CpuController::branchHandle;
        case CondBranch     : return &CpuController::conditionalHandle;
        case Load           : return &CpuController::memoryHandle;

        // UNIMPLEMENTED //
        case ImmediateWide  : return &CpuController::unknownHandle;
        case Unknown        : return &CpuController::unknownHandle;
    }

    return 0;
}

void CpuController::unknownHandle(Instruction * instr)
{
    // UNIMPLEMENTED
    return;
}

void CpuController::registerHandle(Instruction * instr)
{
    std::map<int, ArithmeticFunc *> map;

    map[0b10001011000]	= &instr_add;
    map[0b10001010000]	= &instr_and;
    map[0b11001010000]	= &instr_eor;
    map[0b10101010000]	= &instr_orr;
    map[0b11001011000]	= &instr_sub;

    std::map<int, CustomFunc *> custom;
    custom[0b11111111101]  = &instr_prnt;
    custom[0b11111111100]  = &instr_prnl;
    custom[0b11111111110]  = &instr_dump;
    custom[0b11111111111]  = &instr_halt;

    std::map<int, ArithmeticFunc *>::iterator regIter = map.find(instr->getOpcode());
    std::map<int, CustomFunc *>::iterator custIter = custom.find(instr->getOpcode());

    int64_t Rn = mdl->getRegister(instr->decodeRn());
    int64_t Rm = mdl->getRegister(instr->decodeRm());

    // Register instruction implementation
    if (regIter != map.end())
    {
        // Run our arbitary register instruction.
        mdl->setRegister(
            instr->decodeRd(),
            regIter->second(Rn, Rm)
        );
    }

    // Custom instruction implementation
    if (custIter != custom.end())
    {
        custIter->second(this->mdl, instr);
    }

    // SUBS
    if (0b11101011000 == instr->getOpcode())
    {
        mdl->setRegister(
            instr->decodeRd(),
            instr_subs(Rn, Rm, this->mdl->getFlagsPtr())
        );
    }

    // BR
    if (0b11010110000 == instr->getOpcode())
    {
        int64_t Rt = mdl->getRegister(instr->decodeRn());
        int32_t distance = Rt - mdl->getPc()->getNumber();

        instr_branch(mdl, distance);
    }
}

void CpuController::immediateHandle(Instruction * instr)
{
    std::map<int, ArithmeticFunc *> map;

    map[0b1001000100 ] 	= &instr_addi;
    map[0b1001001000 ] 	= &instr_andi;
    map[0b1101001000 ] 	= &instr_eori;
    map[0b1011001000 ] 	= &instr_orri;
    map[0b1101000100 ] 	= &instr_subi;

    std::map<int, ArithmeticFunc *>::iterator immIter = map.find(instr->getOpcode());

    int64_t Rn = mdl->getRegister(instr->decodeRn());

    // Register instruction implementation
    if (immIter != map.end())
    {

        // Run our arbitary register instruction.
        mdl->setRegister(
            instr->decodeRd(),
            immIter->second(Rn, instr->decodeALU())
        );
    }

    // SUBIS
    if (0b1111000100 == instr->getOpcode())
    {
        int64_t Rm = mdl->getRegister(instr->decodeRm());
        mdl->setRegister(
            instr->decodeRd(),
            instr_subis(Rn, Rm, this->mdl->getFlagsPtr())
        );
    }
}

void CpuController::branchHandle(Instruction * instr)
{
    // Branch and Link (BL)
    if (0b100101 == instr->getOpcode())
        mdl->setRegister(LR, mdl->getPc()->getNumber());
    
    // Branch, and invoke an inner process to avoid stepping on main while-loop
    instr_branch(this->mdl, instr->decodeBRAddr());
    this->process(mdl->getPc()->getInstruction());
}

void CpuController::conditionalHandle(Instruction * instr)
{
    int64_t Rt = this->mdl->getRegister(instr->decodeRt());

    switch(instr->getOpcode())
    {
        /* B.cond */ case 0b01010100: if (! instr_branch_assert(this->mdl, Rt)) return;
        /* CBNZ   */ case 0b10110101: if (! (Rt != 0)) return;
        /* CBZ    */ case 0b10110100: if (! (Rt == 0)) return;
    }

    instr_branch(this->mdl, mdl->getRegister(instr->decodeCondAddr()));
}

void CpuController::memoryHandle(Instruction * instr)
{
    MemModel * mem = this->mdl->getMemory();
    int64_t addr = this->mdl->getRegister(instr->decodeRn()) + instr->decodeDTAddr();
    int64_t Rt   = this->mdl->getRegister(instr->decodeRt());

    switch(instr->getOpcode())
    {
        /* LDUR */ 
        case 0b11111000010:
            this->mdl->setRegister(instr->decodeRt(), mem->readDoubleWord(addr));
            break;

        /* STUR */ 
        case 0b11111000000:
            mem->writeDoubleWord(addr, Rt);
            break;
    }
}
