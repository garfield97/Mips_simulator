#include <iostream>
#include "headers/mem.hpp"
#include "headers/registers.hpp"
#include "headers/pc.hpp"
#include <fstream>
#include "error_check.cpp"


void mother(memory &mem, registers &CPUreg, program_counter &PC);
void read_file(memory &mem, std::fstream &infile);
int decode(memory &mem, registers &CPUreg,program_counter &PC, const unsigned int instruction);
void i_type(memory &mem, registers &CPUreg, program_counter &PC, const unsigned int instruction);
int j_type(memory &mem, registers &CPUreg, program_counter &PC, const unsigned int instruction);
int r_type(registers &CPUreg, program_counter &PC, const unsigned int instruction);


int main(int argc, char **argv)
{
    memory mem;
    registers CPUreg;
    program_counter PC;

    std::string filename = argv[1];

    std::fstream infile;

    infile.open(filename.c_str());

    read_file(mem, infile);

    mother(mem, CPUreg, PC);

    return 0;
}

void mother(memory &mem, registers &CPUreg, program_counter &PC)
{
    do
    {
     int instruction = mem.get_instruction(PC.get_PC()); // Retrieve instruction from memory

    decode(mem, CPUreg, PC, instruction); // Decode and execute


    PC.increment();
    }

    while (PC.get_PC() != 0);
}
void read_file(memory &mem, std::fstream &infile)
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
        mem.store_byte(count, t);
        t = 0;
        count++;
    }
}
int decode(memory &mem, registers &CPUreg,program_counter &PC, const unsigned int instruction)
{
    unsigned short opcode = instruction >> 25;

    switch (opcode)
    {
        case 0b000000: return (r_type(CPUreg, PC, instruction)); break;// R - Type
        case 0b000010: return (j_type(mem, CPUreg, PC, instruction)); break;// J - Type
        case 0b000011: return (j_type(mem, CPUreg, PC, instruction)); break;// J - Type
        default: i_type(mem, CPUreg, PC, instruction); break;// I - Type
    }
}
void i_type(memory &mem, registers &CPUreg, program_counter &PC, const unsigned int instruction)
{
    const short rs = (instruction >> 21) & 0b11111;
    const short rt = (instruction >> 16) & 0b11111;
    const short IMM = instruction & 0XFF;
    const short opcode = instruction >> 26;

    switch (opcode)
    {
        case 0x04: if (CPUreg.reg[rs] == CPUreg.reg[rt]); break; //BEQ ADD PC LOAD THING HERE
        case 0x05: if (CPUreg.reg[rs] != CPUreg.reg[rt]); break; //BNE AND HERE
        case 0x08: CPUreg.reg[rt] = CPUreg.reg[rs] + IMM; break;
        case 0x09: CPUreg.reg[rt] = (unsigned)(CPUreg.reg[rs]) + (unsigned)(IMM); break;
        case 0x0A: if (CPUreg.reg[rs] < IMM) CPUreg.reg[rt] = 1; break;
        case 0x0B: if (CPUreg.reg[rs] < (unsigned)(IMM)) CPUreg.reg[rt] = 1; break;
        case 0x0C: CPUreg.reg[rt] = CPUreg.reg[rs] & IMM; break;
        case 0x0D: CPUreg.reg[rt] = CPUreg.reg[rs] | IMM; break;
        case 0x0F: CPUreg.reg[rt] = IMM << 16; break;
        case 0x23: CPUreg.reg[rt] = mem.load_word(CPUreg.reg[rs]); break;
        case 0x24: CPUreg.reg[rt] = (unsigned)(mem.load_byte(CPUreg.reg[rs])); break;
        case 0x25: CPUreg.reg[rt] = (unsigned)(mem.load_hword(CPUreg.reg[rs])); break;
        case 0x28: mem.store_byte(CPUreg.reg[rs], CPUreg.reg[rt]); break;
        case 0x29: mem.store_hword(CPUreg.reg[rs], CPUreg.reg[rt]); break;
        default: mem.store_word(CPUreg.reg[rs], CPUreg.reg[rt]); break;
    }
}
int j_type(memory &mem, registers &CPUreg, program_counter &PC, const unsigned int instruction)
{
    const unsigned int address = instruction & 0x3FFFFC;
    const unsigned short type = instruction >> 25;

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
}
int r_type(registers &CPUreg, program_counter &PC, const unsigned int instruction)
{
    const unsigned short rs = (instruction >> 21) & 0b11111;
    const unsigned short rt = (instruction >> 16) & 0b11111;
    const unsigned short rd = (instruction >> 11) & 0b11111;
    const unsigned short shift_size = (instruction >> 6) & 0b11111;
    const unsigned short funct = instruction & 0b111111;

    if (rt == 0) return -11;

    switch (funct)
    {
<<<<<<< HEAD
        case 0x20    : if (!addition_exception(rs, rt)) {return -10;} CPUreg.reg[rd] = CPUreg.reg[rs] + CPUreg.reg[rt];  break;//signed addition
        case 0x21    : if (!addition_exception(rs, rt)) {return -10;} CPUreg.reg[rd] = CPUreg.reg[rs] + CPUreg.reg[rt]; break; //  addition
        case 0x22    : CPUreg.reg[rd] = CPUreg.reg[rs] - CPUreg.reg[rt]; break;  // signed subtraction
        case 0x23    : CPUreg.reg[rd] = CPUreg.reg[rs] - CPUreg.reg[rt]; break;  //  subtraction
        case 0x18    : // signed multiplication
        case 0x19    :    //  multiplcation
        case 0x1a    : if(!division_error(rs, rt)) {return -10;})  // signed division
        case 0x1b    : if(!division_error(rs, rt)) {return -10;}    //  division
        case 0x10    : // move from HI
        case 0x12    : // Move from LO
        case 0x24    : CPUreg.reg[rd] = CPUreg.reg[rs] & CPUreg.reg[rt];  break;// bitwise AND
        case 0x25    : CPUreg.reg[rd] = CPUreg.reg[rs] | CPUreg.reg[rt];  break;//bitwise or
        case 0x26    : CPUreg.reg[rd] = CPUreg.reg[rs] ^ CPUreg.reg[rt]; break; // bitwise xor
        case 0x27    : CPUreg.reg[rd] = ~(CPUreg.reg[rs] | CPUreg.reg[rt]); break; //bitwise NOR
        case 0x2a    : if(CPUreg.reg[rs]<CPUreg.reg[rt]) CPUreg.reg[rd]=1;
                       else CPUreg.reg[rd]=0; break;
        case 0x2b    : if(CPUreg.reg[rs]<CPUreg.reg[rt]) CPUreg.reg[rd]=1;
                       else CPUreg.reg[rd]=0; break;
        case 0x00    : CPUreg.reg[rd] = CPUreg.reg[rt]<<shift_size; break;// shift size is the shift amount
        case 0x04    : CPUreg.reg[rd] = CPUreg.reg[rs]<<CPUreg.reg[rt]; break;
        case 0x02    : CPUreg.reg[rd] = CPUreg.reg[rt]>>shift_size; break;
        case 0x06    : CPUreg.reg[rd] = CPUreg.reg[rs]>>CPUreg.reg[rt]; break;
        case 0x03    : CPUreg.reg[rd] = CPUreg.reg[rt]>>shift_size; break; //arithmetic shift
        case 0x07    : CPUreg.reg[rd] = CPUreg.reg[rs]>>CPUreg.reg[rt]; break; // arithmetic shift with the amount
        case 0x08    : PC.load_PC(CPUreg.reg[rs]); break;
        case 0x09    : CPUreg.reg[31] = PC.get_PC() + 4;
                       PC.load_PC(CPUreg.reg[rs]); break;
        case 0x0c    : /*0c goes here*/ ;  break;
        default: return -12; // Invalid instruction was attempted
=======
    		case 0x20    : CPUreg.reg[rd] = CPUreg.reg[rs] + CPUreg.reg[rt]; break;//signed addition
            case 0x21    : CPUreg.reg[rd] = CPUreg.reg[rs] + CPUreg.reg[rt]; break;// unsigned addition
            case 0x22    : CPUreg.reg[rd] = CPUreg.reg[rs] - CPUreg.reg[rt]; break; // signed subtraction
            case 0x23    : CPUreg.reg[rd] = CPUreg.reg[rs] - CPUreg.reg[rt]; break; // unsigned subtraction
            case 0x18    : CPUreg.hi=(CPUreg.reg[rs]*CPUreg.reg[rt])>>32;     //signed multiplication
                           CPUreg.lo=(CPUreg.reg[rs]*CPUreg.reg[rt])<<32)>>32; break;
            case 0x19    : CPUreg.hi=(CPUreg.reg[rs]*CPUreg.reg[rt])>>32;       //unsigned multiplication
                           CPUreg.lo=(CPUreg.reg[rs]*CPUreg.reg[rt])<<32)>>32; break;
            case 0x1a    : CPUreg.lo=CPUreg.reg[rs]/CPUreg.reg[rt];             //signed division
                           CPUreg.hi=CPUreg.reg[rs]%CPUreg.reg[rt]; break;
            case 0x1b    : CPUreg.lo=CPUreg.reg[rs]/CPUreg.reg[rt];      //unsigned division
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
            case 0x08    : PC.load_PC(CPUreg.reg[rs]);break;
            default      : CPUreg.reg[31] = PC.increment();
                           PC.load_PC(CPUreg.reg[rs]); break;
    //case 0x0c    :  // system call for OS services


>>>>>>> 1909e4ccdbc64600da9ca5dd365b481c2330ea7a
    }

    return 0; // Return 0 if no error occured
}
