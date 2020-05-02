all: Emulator.o 
	g++ Emulator.o -o emul
	rm ./*.o -v

	
Emulator.o:
	g++ -c src/Emulator.cpp
