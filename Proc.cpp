#include <stdlib.h>
#include <sys/types.h>

class Proc {
	private:
	int32_t * registers;
	int32_t programCounter;

	/**
	 * Copy constructor
	 **/
	Proc(Proc &copy);

	public:

	/**
	 * Constructor
	 **/
	Proc();

	/**
	 * Deconstructor
	 **/
	~Proc();

	/**
	 * Returns the value stored in a register
	 *
	 * @param reg		register index
	 *
	 * @return register value
	 **/
	int getRegister(int reg);

	/**
	 * Sets the contents of a register to a particular value.
	 *
	 * @param reg 		register index
	 * @param value 	new value
	 **/
	void setRegister(int reg, int value);
};

Proc::Proc() 
{
	this->registers = (int32_t *) malloc(sizeof(int32_t) * 32);
	for (int x = 0; x < 32; x++)
	{
		this->registers[x] = 0;
	}
}

Proc::~Proc()
{
	free(this->registers);
}

void Proc::setRegister(int reg, int value)
{
	if (reg > 30)
		return;
	
	this->registers[reg] = value;
}

int Proc::getRegister(int reg)
{
	if (reg > 30)
		return 0;

	return this->registers[reg];
}
