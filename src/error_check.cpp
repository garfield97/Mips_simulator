#include <climits>

bool RW_error(int access_addr) // Error code -11
{
    if (access_addr < 0x8000000 || access_addr >= 0x9000000) return true;
    return false;
}


bool mem_range_error(int access_addr) // Error code -11
{
    if (access_addr >= 0x40000000) return true;
    return false;
}


bool invalid_instruction(int access_addr) // Error code -12
{
    if (access_addr < 0x9000000 || access_addr >= 0x4400000) return true;
    return false;
}


bool addition_exception(unsigned short op1, unsigned int short op2) // Error code -10
{
    if (op1 > INT_MAX - op2) return true;
    return false;
}


bool division_error(unsigned short op1, unsigned int short op2) // Error code -10
{
    if (op2 == 0) return true;
    return false;
}