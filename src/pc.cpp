#include <iostream>
#include "headers/pc.hpp"


program_counter::program_counter()
: PC(0) {}


uint32_t program_counter::get_PC() const
{
    return PC;
}


void program_counter::increment()
{
    PC += 4;
}


void program_counter::load_PC(uint32_t instr)
{
	PC  = instr;
}
