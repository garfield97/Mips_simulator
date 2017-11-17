#include <iostream>
#include "headers/mem.hpp"
#include "headers/registers.hpp"
#include "headers/pc.hpp"
#include "r_type.cpp"
#include "i_type.cpp"
#include "j_type.cpp"


void decode(memory &memory, registers &registers,program_counter &PC, const unsigned int instruction)
{
    unsigned short opcode = instruction >> 25;

    switch (opcode)
    {
        case 0b000000: r_type(memory, registers, PC, instruction);// R - Type
        case 0b000010: j_type(memory, registers, PC, instruction);// J - Type
        case 0b000011: r_type(memory, registers, PC, instruction);// J - Type
        default: i_type(memory, registers, PC, instruction);// I - Type
    }
}