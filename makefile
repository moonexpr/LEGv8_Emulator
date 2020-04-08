all: Emulator.o Instruction.o Proc.o
	g++ Emulator.o Instruction.o Proc.o -o emul
	rm ./*.o -v

	
Emulator.o:
	g++ -c Emulator.cpp

Instruction.o:
	g++ -c Instruction.cpp

Proc.o:
	g++ -c Proc.cpp
