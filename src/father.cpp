#include <iostream>
#include "headers/mem.hpp"
#include "headers/registers.hpp"
#include "headers/pc.hpp"
#include <fstream>
#include "error_check.cpp"


int mother(memory &mem, registers &CPUreg, program_counter &PC);
int read_file(memory &mem, std::fstream &infile);
int decode(memory &mem, registers &CPUreg,program_counter &PC, const unsigned int instruction);
int i_type(memory &mem, registers &CPUreg, program_counter &PC, const unsigned int instruction);
int j_type(memory &mem, registers &CPUreg, program_counter &PC, const unsigned int instruction);
int r_type(registers &CPUreg, program_counter &PC, const unsigned int instruction);


int main(int argc, char **argv)
{
    memory mem;
    registers CPUreg;
    program_counter PC;
    int result = 0;

    std::string filename = argv[1];

    std::fstream infile;

    infile.open(filename.c_str());

    if (read_file(mem, infile) == -11) ; //If = -11 then binary too large to store in instruction memory

    do
    {
        result = mother(mem, CPUreg, PC);
        PC.increment();
    }

    while (result == 0); // Continue execution so long as no errors/ exceptions occur

    switch (result)
    {
        case -1: // Program completed successfully
        case -12: // Invalid instruction
        case -11: // Memory exception
        case -10: // Arithmetic exception
        default: // Any other errors?
    }


    return 0;
}


int mother(memory &mem, registers &CPUreg, program_counter &PC)
{
     int instruction = mem.get_instruction(PC.get_PC()); // Retrieve instruction from memory

    return (decode(mem, CPUreg, PC, instruction)); // Decode and execute
}


int read_file(memory &mem, std::fstream &infile)
{
    char c;
    int t = 0;
    long count = 0;

    while (!(infile.eof()))
    {
        for (int i = 0; i < 8; i++)
        {
            infile.get(c);
            if (c == '1') t += (1 << (7-i));
        }
        if (invalid_instruction(count)) return -12;
        mem.store_byte(count, t);
        t = 0;
        count++;
    }
    return 0;
}


int decode(memory &mem, registers &CPUreg,program_counter &PC, const unsigned int instruction)
{
    unsigned short opcode = instruction >> 25;

    if (invalid_opcode(opcode)) return -12;

    switch (opcode)
    {
        case 0b000000: return (r_type(CPUreg, PC, instruction));// R - Type
        case 0b000010: return (j_type(mem, CPUreg, PC, instruction));// J - Type
        case 0b000011: return (j_type(mem, CPUreg, PC, instruction));// J - Type
        default: return (i_type(mem, CPUreg, PC, instruction));// I - Type
    }
}


int i_type(memory &mem, registers &CPUreg, program_counter &PC, const unsigned int instruction)
{
    const short rs = (instruction >> 21) & 0b11111;
    const short rt = (instruction >> 16) & 0b11111;
    const short IMM = instruction & 0XFF;
    const short opcode = instruction >> 26;

    if (rs > 31 || rt > 31) return -11;

    switch (opcode)
    {
        case 0x04: if (mem_range_error(IMM)) return -11; if (CPUreg.reg[rs] == CPUreg.reg[rt]) PC.load_PC(IMM); break; //BEQ ADD PC LOAD THING HERE
        case 0x05: if (mem_range_error(IMM)) return -11; if (CPUreg.reg[rs] != CPUreg.reg[rt]) PC.load_PC(IMM); break; //BNE AND HERE
        case 0x08: CPUreg.reg[rt] = CPUreg.reg[rs] + IMM; break;
        case 0x09: CPUreg.reg[rt] = (unsigned)(CPUreg.reg[rs]) + (unsigned)(IMM); break;
        case 0x0A: if (CPUreg.reg[rs] < IMM) CPUreg.reg[rt] = 1; break;
        case 0x0B: if (CPUreg.reg[rs] < (unsigned)(IMM)) CPUreg.reg[rt] = 1; break;
        case 0x0C: CPUreg.reg[rt] = CPUreg.reg[rs] & IMM; break;
        case 0x0D: CPUreg.reg[rt] = CPUreg.reg[rs] | IMM; break;
        case 0x0F: CPUreg.reg[rt] = IMM << 16; break;
        case 0x23: if (RW_error(CPUreg.reg[rs])) {return -11;} CPUreg.reg[rt] = mem.load_word(CPUreg.reg[rs]); break;
        case 0x24: if (RW_error(CPUreg.reg[rs])) {return -11;} CPUreg.reg[rt] = (unsigned)(mem.load_byte(CPUreg.reg[rs])); break;
        case 0x25: if (RW_error(CPUreg.reg[rs])) {return -11;} CPUreg.reg[rt] = (unsigned)(mem.load_hword(CPUreg.reg[rs])); break;
        case 0x28: if (RW_error(CPUreg.reg[rs])) {return -11;} if (write_to_zero(CPUreg.reg[rs])) return -1; mem.store_byte(CPUreg.reg[rs], CPUreg.reg[rt]); break;
        case 0x29: if (RW_error(CPUreg.reg[rs])) {return -11;} if (write_to_zero(CPUreg.reg[rs])) return -1; mem.store_hword(CPUreg.reg[rs], CPUreg.reg[rt]); break;
        case 0x30: if (RW_error(CPUreg.reg[rs])) {return -11;} if (write_to_zero(CPUreg.reg[rs])) return -1; mem.store_word(CPUreg.reg[rs], CPUreg.reg[rt]); break;
        default: return -12; 
    }
}


int j_type(memory &mem, registers &CPUreg, program_counter &PC, const unsigned int instruction)
{
    const unsigned int address = instruction & 0x3FFFFC;
    const unsigned short type = instruction >> 25;

    if (mem_range_error(address)) return -11;

    switch (type)
    {
        case 0x02:
        	PC.load_PC(address);
        	break;
        default:
        	CPUreg.reg[31] = PC.get_PC();
        	PC.load_PC(address);
        	break;
    }

    return 0;
}


int r_type(registers &CPUreg, program_counter &PC, const unsigned int instruction)
{
    const unsigned short rs = (instruction >> 21) & 0b11111;
    const unsigned short rt = (instruction >> 16) & 0b11111;
    const unsigned short rd = (instruction >> 11) & 0b11111;
    const unsigned short shift_size = (instruction >> 6) & 0b11111;
    const unsigned short funct = instruction & 0b111111;
    
    long long temp;

    if (rt == 0) return -11;
    if (rs > 31 || rt > 31) return -11;

    switch (funct)
    {
    		case 0x20    : if (addition_exception(rs, rt)) CPUreg.reg[rd] = CPUreg.reg[rs] + CPUreg.reg[rt]; break;//signed addition
            case 0x21    : if (addition_exception(rs, rt)) CPUreg.reg[rd] = CPUreg.reg[rs] + CPUreg.reg[rt]; break;// unsigned addition
            case 0x22    : CPUreg.reg[rd] = CPUreg.reg[rs] - CPUreg.reg[rt]; break; // signed subtraction
            case 0x23    : CPUreg.reg[rd] = CPUreg.reg[rs] - CPUreg.reg[rt]; break; // unsigned subtraction
            case 0x18    : temp=(CPUreg.reg[rs]*CPUreg.reg[rt]);     //signed multiplication
                           CPUreg.hi=(temp>>32); 
                           CPUreg.lo=(temp<<32)>>32; break;
            case 0x19    : temp=(CPUreg.reg[rs]*CPUreg.reg[rt]);       //unsigned multiplication
                           CPUreg.hi=temp>>32;       
                           CPUreg.lo=(temp<<32)>>32; break;
            case 0x1a    : if(division_error(rs, rt)) {return -10;} CPUreg.lo=CPUreg.reg[rs]/CPUreg.reg[rt];             //signed division
                           CPUreg.hi=CPUreg.reg[rs]%CPUreg.reg[rt]; break;
            case 0x1b    : if(division_error(rs, rt)) {return -10;} CPUreg.lo=CPUreg.reg[rs]/CPUreg.reg[rt];      //unsigned division
                           CPUreg.hi=CPUreg.reg[rs]%CPUreg.reg[rt];  break;
            case 0x10    : CPUreg.reg[rd]=CPUreg.hi; break;// move from hi
            case 0x12    : CPUreg.reg[rd]=CPUreg.lo; break;// move fromlo
            case 0x24    : CPUreg.reg[rd] = CPUreg.reg[rs] & CPUreg.reg[rt]; break;// bitwise AND
            case 0x25    : CPUreg.reg[rd] = CPUreg.reg[rs] | CPUreg.reg[rt]; break;//bitwise or
            case 0x26    : CPUreg.reg[rd] = CPUreg.reg[rs] ^ CPUreg.reg[rt]; break;// bitwise xor
            case 0x27    : CPUreg.reg[rd] = ~(CPUreg.reg[rs] | CPUreg.reg[rt]);break; //bitwise NOR
            case 0x2a    : if(CPUreg.reg[rs]<CPUreg.reg[rt]) CPUreg.reg[rd]=1;
                           else CPUreg.reg[rd]=0;        break;
            case 0x2b    : if(CPUreg.reg[rs]<CPUreg.reg[rt]) CPUreg.reg[rd]=1;
                           else CPUreg.reg[rd]=0;break;
            case 0x00    : CPUreg.reg[rd] = CPUreg.reg[rt]<<shift_size; break;// shift size is the shift amount
            case 0x04    : CPUreg.reg[rd] = CPUreg.reg[rs]<<CPUreg.reg[rt]; break;
            case 0x02    : CPUreg.reg[rd] = CPUreg.reg[rt]>>shift_size; break;
            case 0x06    : CPUreg.reg[rd] = CPUreg.reg[rs]>>CPUreg.reg[rt]; break;
            case 0x03    : CPUreg.reg[rd] = CPUreg.reg[rt]>>shift_size; break; //arithmetic shift
            case 0x07    : CPUreg.reg[rd] = CPUreg.reg[rs]>>CPUreg.reg[rt]; break;// arithmetic shift with the amount
            case 0x08    : if (invalid_instruction(CPUreg.reg[rs])) return -12; PC.load_PC(CPUreg.reg[rs]);break;
            case 0x09    : if (RW_error(CPUreg.reg[rs])) {return -11;} CPUreg.reg[31] = PC.get_PC(); PC.load_PC(CPUreg.reg[rs]); break;
            default: return -12; 
    }

    return 0; // Return 0 if no error occured
}
