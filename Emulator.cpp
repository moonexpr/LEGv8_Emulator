#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <endian.h>
#include <stdio.h>
#include <string.h>
#include "Provided.cpp"
#include "Instruction.cpp"

char * helpText()
{
	return (char *) "usage: emul [FILE]\n";
}

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

void decode(Instruction * instr, char * asmText)
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

int main(int argc, char ** argv)
{

	/**
	 * 910007e0
	 * 910007e1
	 * 8b010000

	Instruction * instr =  new Instruction(0x910007e1);
	std::cout << std::hex << instr->decodeRd() << std::endl;

	 **/

	if (argc <= 1)
	{
		std::cout << helpText() << std::endl;
		return 0;
	}

	char * filePath = argv[1];
	struct stat buffer;
	
	int fileDescriptor = open(argv[1], O_RDONLY);
	if (fstat(fileDescriptor, &buffer) == -1)
	{
		std::cerr << "failed to get file size!" << std::endl;
	}

	FILE * file = fdopen(fileDescriptor, "r");
	int32_t * program = (int32_t *) mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fileDescriptor, 0);
	void * bprogram = malloc(buffer.st_size / 4);

	if (!bprogram) std::cerr << "malloc fail!";


	int pos;
	for (int pos = 0; pos < buffer.st_size / 4; pos ++)
	{
		program[pos] = be32toh(program[pos]);
		Instruction * inst = new Instruction((int32_t) program[pos]);
		char * asmText = (char *) malloc(sizeof(char) * 150);
		decode(inst, asmText);
	}

	return 0;
}
