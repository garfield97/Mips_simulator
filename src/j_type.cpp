#include <iostream>
#include "headers/mem.hpp"
#include "headers/registers.hpp"
#include "headers/pc.hpp"


void j_type(memory &memory, registers &registers, program_counter &PC, const   int instruction)
{
    const int address = instruction & 0x3FFFFC;
    const short type = instruction >> 25;

    switch (type)
    {
        case 0x02:
        	PC.load_PC(address);
        default:
        	registers.reg[31] = PC.get_PC();
        	PC.load_PC(address);
    }
}
