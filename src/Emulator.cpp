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
