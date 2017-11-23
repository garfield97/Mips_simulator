#include <climits>

bool RW_error(int access_addr) // Error code -11
{
    access_addr /= 4;
    if (access_addr < 0x8000000 || access_addr >= 0x9000000) return true;
    return false;
}


bool mem_range_error(int access_addr) // Error code -11
{
    access_addr /= 4;
    if (access_addr >= 0x40000000) return true;
    return false;
}


bool invalid_instruction(int access_addr) // Error code -12
{
    access_addr /= 4;
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
<<<<<<< HEAD
    
    
}


bool invalid_opcode(unsigned int opcode){

    switch(opcode){

        case 0x20: return false; break;
        case 0x24: return false; break;
        case 0x2b: return false; break;
        case 0x29: return false; break;
        case 0x28: return false; break;
        case 0x0f: return false; break;
        case 0x0d: return false; break;
        case 0x0c: return false; break;
        case 0x0e: return false; break;
        case 0x0a: return false; break;
        case 0x0b: return false; break;
        case 0x04: return false; break;
        case 0x05: return false; break;
        case 0x08: return false; break;
        case 0x09: return false; break;
        case 0x23: return false; break;
        case 0x21: return false; break;
        case 0x25: return false; break;
        case 0x02: return false; break;
        case 0x03: return false; break;
        default  : return true;  break;
    }
}


//error codes

//bool internal_error(
//invalid opcode


//bool io_error(
=======
}
>>>>>>> 25c17befab5a6723399a847e6c33456852c6fa49
