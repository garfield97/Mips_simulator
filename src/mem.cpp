#include <iostream>
#include "headers/mem.hpp"

#define MEM_SIZE 0x3000000C
#define NULL_START 0x00000003
#define NULL_END 0x00000004
#define EXEC_START 0x10000000
#define EXEC_END 0x11000000
#define RW_START 0x20000000
#define RW_END 0x24000000
#define GET_C_START 0x30000000
#define GET_C_END 0x30000004
#define PUT_C_START 0x30000004
#define PUT_C_END 0x30000008

// Allocate block of memory and split up into sub-blocks
memory::memory()
{
    mem = new unsigned int[MEM_SIZE]; // Declare memory of specified size in heap, so it is dynamically allocated

    for (int i = RW_START; i <= RW_END; i++) mem[i] = 0;//Initialise all data memory to 0
}


 unsigned int memory::get_instruction(int location) const
{
    location /= 4;
     unsigned int inst = mem[location];

    return inst;
}


unsigned int memory::load_word(int loc) const
{
    loc /= 4;
    loc += RW_START;
    
    unsigned int tmp = mem[loc];
    return tmp;
}


unsigned int memory::load_byte(int loc) const
{
    int offset = loc % 4;
    loc /= 4;
    loc += RW_START;

    unsigned int tmp = 0;

    switch (offset)
    {
        case 0: tmp = mem[loc] >> 23; break;
        case 1: tmp = (mem[loc] >> 15) & 0xFF; break;
        case 3: tmp = (mem[loc] >> 7) & 0xFF; break;
        default: tmp = mem[loc] & 0xFF; break;
    }
    return tmp;
}

unsigned int memory::load_hword(int loc) const
{
    int offset = loc % 4;
    loc /= 4;
    loc += RW_START;

    unsigned int tmp = 0;

    switch (offset)
    {
        case 0: tmp = (mem[loc] >> 15) & 0xFFFF; break;
        default: tmp = (mem[loc] & 0xFFFF); break;
    }
    return tmp;
}


void memory::store_word(int loc,  const int input)
{
    loc /= 4;
    loc += RW_START;

    mem[loc] = input;
}


void memory::store_byte(int loc, const int input)
{
    int offset = loc % 4;
    loc /= 4;
    loc += RW_START;
    unsigned int in = input & 0xFF;

    mem[loc] = 0;

    switch (offset)
    {
        case 1: mem[loc] = in << 8; break;
        case 2: mem[loc] = in << 16; break;
        case 3: mem[loc] = in << 24; break;
        default: mem[loc] = in; break;
    }
}


void memory::store_hword(int loc, const int input)
{
    int offset = loc % 4;
    loc /= 4;
    loc += RW_START;

    unsigned int in = input & 0xFFFF;

    mem[loc] = 0;

    switch (offset)
    {
        case 0: mem[loc] = in;
        default: mem[loc] = in << 16; break;
    }
}
