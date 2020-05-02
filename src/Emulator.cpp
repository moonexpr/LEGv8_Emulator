#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <endian.h>
#include <stdio.h>
#include <string.h>

#ifndef H_PROVIDED
#include "Provided.cpp"
#endif

#ifndef H_CpuModel
#include "core/CpuModel.cpp"
#endif

#ifndef H_CpuController
#include "core/CpuController.cpp"
#endif

#ifndef H_INSTR
#include "Instruction.cpp"
#endif

#define H_EMULATOR

char * helpText()
{
    return (char *) "usage: emul [FILE]\n";
}


/*void decode(Instruction * instr, char * asmText)
{

    std::map<int, const char *> names = instructionNames();

    enum InstType type = instr->getType();
    switch(type)
    {
        case Register:
        {
            const char * instrName = names[instr->getOpcode()];
            if (!strcmp(instrName, "LSL") || !strcmp(instrName, "LSR"))
                printf("%s\tX%d, X%d, #%d\n", names[instr->getOpcode()], instr->decodeRd(), instr->decodeRn(), instr->decodeShamt());
            else if (!strcmp(instrName, "BR"))
                printf("%s\tX%d\n", names[instr->getOpcode()], instr->decodeRn());
            else
                printf("%s\tX%d, X%d, X%d\n", names[instr->getOpcode()], instr->decodeRd(), instr->decodeRn(), instr->decodeRm());
            break;
        }

        case Immediate:
            printf("%s\tX%d, X%d, #%d\n", names[instr->getOpcode()], instr->decodeRd(), instr->decodeRn(), instr->decodeALU());
            break;

        case Load:
            printf("%s\tX%d, [X%d, #%d]\n", names[instr->getOpcode()], instr->decodeRt(), instr->decodeRn(), instr->decodeDTAddr());
            break;

        case Branch:
            printf("%s\t%d\n", names[instr->getOpcode()], instr->decodeAddr());
            break;

        case CondBranch:
        {
            const char * instrName = names[instr->getOpcode()];
            std::map<int, const char *> condNames = conditionalNames();
            if (!strcmp(instrName, "B.C")) 
                printf("B.%s\t%d\n", condNames[instr->decodeRt()], instr->decodeCondAddr());
            else 
                printf("%s\tX%d, %d\n", names[instr->getOpcode()], instr->decodeRt(), instr->decodeCondAddr());

            break;
        }

        case ImmediateWide:
            printf("! Unsupported");
            break;

        case Unknown:
            printf("! Unsupported");
            break;
    }
}

void dumpRegisters(CpuModel * mdl)
{
    for (int r = 0; r < REGISTER_MAX; r ++)
    {
        printf("X%d: %d\n", r, mdl->getRegister(r));
    }
}
*/

int main(int argc, char ** argv)
{

    if (argc <= 1)
    {
        std::cout << helpText() << std::endl;
        return 0;
    }

    struct stat buffer;
    
    int fileDescriptor = open(argv[1], O_RDONLY);
    if (fstat(fileDescriptor, &buffer) == -1)
    {
        std::cerr << "E: Failed to get file size!" << std::endl;
        return 0;
    }

    int32_t * program = (int32_t *) mmap(0, buffer.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fileDescriptor, 0);

    if (!program) std::cerr << "malloc fail!";

#ifdef _DEBUG_DECODE
    int pos;
    for (int pos = 0; pos < buffer.st_size; pos ++)
    {
        program[pos] = be32toh(program[pos]);
        Instruction * inst = new Instruction((int32_t) program[pos]);
        char * asmText = (char *) malloc(sizeof(char) * 150);
        decode(inst, asmText);
    }
#endif

    CpuModel * mdl = new CpuModel();
    CpuController * ctrl = new CpuController(mdl);

    // Create our instruction linked list.
    int line;
    for (line = 0; line < buffer.st_size; line ++)
    {
        program[line] = be32toh(program[line]);
        mdl->addToTail(new Instruction((int32_t) program[line]));
    }

    InstructionLL * pc;
    while((pc = mdl->getPc()))
    {
        Instruction * instr = pc->getInstruction();

        // We check validity here because branches assume unimplemented instructions as valid instructions
        if (instr->isValid()) ctrl->process(instr);

        mdl->forwardPc();
    }

    return 0;
}
