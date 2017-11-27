#include <climits>
#include <iostream>
#include <cinttypes>
#include <cstdlib>

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

bool RW_error(uint32_t access_addr) // Error code -11
{
    access_addr /= 4;

    if (access_addr < RW_START || access_addr >= RW_END) return true;
    return false;
}


bool mem_range_error(uint32_t access_addr) // Error code -11
{
    access_addr /= 4;

    if (access_addr >= MEM_SIZE) return true;
    return false;
}


bool invalid_instruction(uint32_t access_addr) // Error code -12
{
    access_addr /= 4;

    if (access_addr < EXEC_START || access_addr >= EXEC_END) return true;
    return false;
}


bool addition_exception(uint32_t op1, uint32_t op2) // Error code -10
{
    if (op1 > INT_MAX - op2) return true;
    return false;
}


bool subtraction_exception(uint32_t op1, uint32_t op2)
{
    long long tmp = op1;

    if (abs(tmp) > abs((INT_MAX + op2)) return true;
    return false;
}


bool division_error(uint32_t op1, uint32_t op2) // Not an exception but yields unpredictable result
{
    if (op2 == 0) return true;
    return false;
}


bool invalid_opcode(uint32_t opcode){

    switch(opcode){

        case 0x04: return false; break;
        case 0x05: return false; break;
        case 0x08: return false; break;
        case 0x09: return false; break;
        case 0x0A: return false;break;
        case 0x0B: return false;break;
        case 0x0C: return false;break;
        case 0x0D: return false;break;
        case 0x0F: return false;break;
        case 0x23: return false;break;
        case 0x24: return false;break;
        case 0x25: return false;break;
        case 0x28: return false;break;
        case 0x29: return false; break;
        case 0x30: return false; break;
        default  : return true;break;
    }
}

bool write_to_zero(int32_t access_addr)
{
    if (access_addr == 0) return true;

    access_addr /= 4;

    if (access_addr < 1) return true;

    return false;
}


//error codes

//bool internal_error(
//invalid opcode


//bool io_error(
