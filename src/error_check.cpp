#include <climits>

bool RW_error(int access_addr)
{
    if (access_addr > 0x4000000) return true;
    return false;
}


bool mem_range_error(int access_addr)
{
    if (access_addr > 0x30000008) return true;
    return false;
}


bool invalid_instruction(int access_addr)
{
    if (access_addr > 0x1FFFFFFF || access_addr < 0x4) return true;
    return false;
}


bool addition_exception(unsigned short op1, unsigned int short op2)
{
    if (op1 > INT_MAX - op2) return true;
    return false;
}


bool division_error(unsigned short op1, unsigned int short op2)
{
    if (op2 == 0) return true;
    return false;
}