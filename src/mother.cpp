#include <iostream>

#include "decoder.hpp"
#include "headers/mem.hpp"
#include "headers/registers.hpp"
#include "headers/pc.hpp"


void mother(memory &memory, registers &registers, program_counter &PC)
{
    do
    {
     int instruction = memory.get_instruction(PC.get_PC()); // Retrieve instruction from memory

    decode(memory, registers, PC, instruction); // Decode and execute


    PC.increment();
    }

    while (PC.get_PC() != 0);
}
