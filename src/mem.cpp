#include "headers/mem.hpp"
#include <bitset>

#define MEM_SIZE 0x40000000
#define NULL_START 0x00000000
#define NULL_END 0x00000000
#define EXEC_START 0x9000000
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
    mem = new uint32_t [MEM_SIZE]; // Declare memory of specified size in heap, so it is dynamically allocated

    for (int i = RW_START; i <= RW_END; i++) mem[i] = 0;//Initialise all data memory to 0
}


 uint32_t memory::get_instruction(uint32_t loc) const
{
    loc /= 4;

    loc += EXEC_START;

    uint32_t inst = mem[loc];

    return inst;
}


void memory::store_instruction(uint32_t loc, uint32_t inst)
{
    loc /= 4;
    loc += EXEC_START;

    mem[loc] = inst;
}


uint32_t memory::load_word(uint32_t loc)
{
    loc /= 4;
    loc += RW_START;

    if (loc == GET_C_START) return (get_c(loc));
    
    uint32_t tmp = mem[loc];

    return tmp;
}


uint32_t memory::load_byte(uint32_t loc) const
{
    int pos = loc % 4;
    loc /= 4;
    loc += RW_START;
    uint32_t tmp = 0;

    switch (pos)
    {
        case 0: tmp = (mem[loc] >> 16); break;
        default: tmp = (mem[loc] & 0xFFFF); break;
    }

    return tmp;
}

uint32_t memory::load_hword(uint32_t loc) const
{
    int pos = loc % 4;
    loc /= 4;
    loc += RW_START;
    
    uint32_t tmp = 0;

    switch (pos)
    {
        case 0: tmp = (mem[loc] >> 23); break;
        case 1: tmp = (mem[loc] >> 15) & 0xFF; break;
        case 2: tmp = (mem[loc] >> 7) & 0xFF; break;
        default: tmp = mem[loc] & 0xFF; break;
    }

    return tmp;
}


void memory::store_word(uint32_t loc,  const int32_t input)
{
    loc /= 4;
    loc += RW_START;

    if (loc == PUT_C_START) put_c(input); return;

    mem[loc] = input;
}


void memory::store_byte(uint32_t loc, const int32_t input)
{
    int pos = loc % 4;
    loc /= 4;
    loc += RW_START;

    switch (pos)
    {
        case 0: mem[loc] = (input & 0xFF) << 23; break;
        case 1: mem[loc] = (input & 0xFF) << 15; break;
        case 2: mem[loc] = (input & 0xFF) << 7; break;
        default: mem[loc] = input * 0xFF;
    }
}


void memory::store_hword(uint32_t loc, const int32_t input)
{
    int pos = loc % 4;
    loc /= 4;
    loc += RW_START;

    switch(pos)
    {
        case 0: mem[loc] = (input & 0xFF) << 15; break;
        default: mem[loc] = (input & 0xFF); break;
    }
}


uint32_t memory::get_c(uint32_t loc_corrected)
{
    uint32_t tmp;

    scanf("%" SCNd32, &tmp);
    
    return tmp;
}


void memory::put_c(int32_t item)
{
    std::cout<<std::bitset<32>(item)<<std::endl;
}