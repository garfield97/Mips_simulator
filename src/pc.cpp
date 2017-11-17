#include <iostream>
#include "pc.hpp"


program_counter::program_counter()
{
    PC = 0;
}


unsigned int program_counter::get_PC() const
{
    return PC;
}


void program_counter::increment()
{
    PC += 4;
}


void load_PC(unsigned int instr)
{
    PC = instr;
}