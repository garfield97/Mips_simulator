#include <iostream>
#include "headers/mem.hpp"

#define MEM_SIZE 0x40000000
#define NULL_START 0x00000000
#define NULL_END 0x00000000
#define EXEC_START 0x4000000
#define EXEC_END 0x4400000
#define RW_START 0x8000000
#define RW_END 0x9000000
#define GET_C_START 0xC000000
#define GET_C_END 0xC000001
#define PUT_C_START 0xC000001
#define PUT_C_END 0xC000002

// Allocate block of memory and split up into sub-blocks
memory::memory()
{
    mem = new int[MEM_SIZE]; // Declare memory of specified size in heap, so it is dynamically allocated

    for (int i = RW_START; i <= RW_END; i++) mem[i] = 0;//Initialise all data memory to 0
}


 int memory::get_instruction(int location) const
{
     int inst = (mem[location] << 24) + (mem[location+1] << 16) + (mem[location+2] << 8) + mem[location+3];

    return inst;
}


int memory::load_word( int loc) const
{
    loc += RW_START;
    
    int tmp = (mem[loc] << 24) + (mem[loc+1] << 16) + (mem[loc+2] << 8) + mem[loc+3];
    return tmp;
}


int memory::load_byte( int loc) const
{
    loc += RW_START;

    int tmp = mem[loc];
    return tmp;
}

int memory::load_hword( int loc) const
{
    loc += RW_START;

    int tmp = (mem[loc] << 24) + (mem[loc+1] << 16);
    return tmp;
}


void memory::store_word(int loc,  const int input)
{
    loc += RW_START;

    mem[loc] = input >> 24;
    mem[loc + 1] = (input >> 16) & 0xF;
    mem[loc + 2] = (input >> 8) & 0xF;
    mem[loc + 3] = input & 0xF;
}


void memory::store_byte(int loc, const int input)
{
    loc += RW_START;

    mem[loc] = input & 0xF;
}


void memory::store_hword(int loc, const int input)
{
    loc += RW_START;

    mem[loc] = (input >> 8) & 0xF;
    mem[loc + 1] = input & 0xF;
}
