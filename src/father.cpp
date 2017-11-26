#include <iostream>
#include "headers/mem.hpp"
#include "headers/registers.hpp"
#include "headers/pc.hpp"
#include <fstream>
#include "headers/error_check.hpp"
#include <cinttypes>


int32_t mother(memory &mem, registers &CPUreg, program_counter &PC);
int32_t read_file(memory &mem, std::fstream &infile);
int32_t decode(memory &mem, registers &CPUreg,program_counter &PC, const  uint32_t instruction);
int32_t i_type(memory &mem, registers &CPUreg, program_counter &PC, const uint32_t instruction);
int32_t j_type(memory &mem, registers &CPUreg, program_counter &PC, const uint32_t instruction);
int32_t r_type(registers &CPUreg, program_counter &PC, const uint32_t instruction);


int main(int argc, char **argv)
{
    memory mem;
    registers CPUreg;
    program_counter PC;
    int result = 0;
    int i = 0;

    std::string filename = argv[1];

    std::fstream infile;

    infile.open(filename.c_str());

    if (!infile.is_open()) {std::cout<<"Invalid filename"<<std::endl; return 0;}

    if (read_file(mem, infile) == -11) {std::cout<<"File too large to read"<<std::endl; result = -11;} //If = -11 then binary too large to store in instruction memory

    infile.close();

    PC.increment();

    while (result == 0)
    {
        std::cout<<"round "<<i<<std::endl;
        result = mother(mem, CPUreg, PC);
        PC.increment();
        i++;
    }

    switch (result)
    {
        case -1: std::cout<<result<<" Schmei"<<std::endl; break;// Program completed successfully
        case -12: std::cout<<result<<" Invalid instruction"<<std::endl; break;// Invalid instruction
        case -11: std::cout<<result<<" Invalid address"<<std::endl; break;// Memory exception
        case -10: std::cout<<result<<" Arithmetic exception"<<std::endl; break;// Arithmetic exception
        default: std::cout<< result<<" Other error"<<std::endl; break;// Any other errors?
    }


    return 0;
}


int32_t mother(memory &mem, registers &CPUreg, program_counter &PC)
{
     int32_t instruction = mem.get_instruction(PC.get_PC()); // Retrieve instruction from memory

    return (decode(mem, CPUreg, PC, instruction)); // Decode and execute
}


int32_t read_file(memory &mem, std::fstream &infile)
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
        std::cout<<t<<std::endl;
        mem.store_byte(count, t);
        t = 0;
        count += 4;
    }
    return 0;
}


int32_t decode(memory &mem, registers &CPUreg,program_counter &PC, const uint32_t instruction)
{
    uint32_t opcode = instruction >> 25;

    switch (opcode)
    {
        case 0b000000: return (r_type(CPUreg, PC, instruction));// R - Type
        case 0b000010: return (j_type(mem, CPUreg, PC, instruction));// J - Type
        case 0b000011: return (j_type(mem, CPUreg, PC, instruction));// J - Type
        default: if (!invalid_opcode(opcode)) {return (i_type(mem, CPUreg, PC, instruction));} return -12;// I - Type
    }
}


int32_t i_type(memory &mem, registers &CPUreg, program_counter &PC, const unsigned int instruction)
{
    const short rs = (instruction >> 21) & 0b11111;
    const short rt = (instruction >> 16) & 0b11111;
    const short IMM = instruction & 0XFF;
    const short opcode = instruction >> 26;

    if (rs > 31 || rt > 31) return -11;
    if (rt == 0) return -11;

    switch (opcode)
    {
        case 0x04: if (mem_range_error(IMM)) {return -11;} if (CPUreg.reg[rs] == CPUreg.reg[rt]) PC.load_PC(IMM); return 0; return 0; //BEQ ADD PC LOAD THING HERE
        case 0x05: if (mem_range_error(IMM)) {return -11;} if (CPUreg.reg[rs] != CPUreg.reg[rt]) PC.load_PC(IMM); return 0; //BNE AND HERE
        case 0x08: if (addition_exception(CPUreg.reg[rs], IMM)) {return -10;} CPUreg.reg[rt] = CPUreg.reg[rs] + IMM; return 0;
        case 0x09: CPUreg.reg[rt] = (unsigned)(CPUreg.reg[rs]) + (unsigned)(IMM); return 0;
        case 0x0A: if (CPUreg.reg[rs] < IMM) CPUreg.reg[rt] = 1; return 0;
        case 0x0B: if ((unsigned)CPUreg.reg[rs] < (unsigned)(IMM)) CPUreg.reg[rt] = 1; return 0;
        case 0x0C: CPUreg.reg[rt] = CPUreg.reg[rs] & IMM; return 0;
        case 0x0D: CPUreg.reg[rt] = CPUreg.reg[rs] | IMM; return 0;
        case 0x0F: CPUreg.reg[rt] = IMM << 16; return 0;
        case 0x23: if (RW_error(CPUreg.reg[rs])) {return -11;} CPUreg.reg[rt] = mem.load_word(CPUreg.reg[rs]); return 0;
        case 0x24: if (RW_error(CPUreg.reg[rs])) {return -11;} CPUreg.reg[rt] = (unsigned)(mem.load_byte(CPUreg.reg[rs])); return 0;
        case 0x25: if (RW_error(CPUreg.reg[rs])) {return -11;} CPUreg.reg[rt] = (unsigned)(mem.load_hword(CPUreg.reg[rs])); return 0;
        case 0x28: if (RW_error(CPUreg.reg[rs])) {return -11;} if (write_to_zero(CPUreg.reg[rs])) {return -1;} mem.store_byte(CPUreg.reg[rs], CPUreg.reg[rt]); return 0;
        case 0x29: if (RW_error(CPUreg.reg[rs])) {return -11;} if (write_to_zero(CPUreg.reg[rs])) {return -1;} mem.store_hword(CPUreg.reg[rs], CPUreg.reg[rt]); return 0;
        case 0x30: if (RW_error(CPUreg.reg[rs])) {return -11;} if (write_to_zero(CPUreg.reg[rs])) {return -1;} mem.store_word(CPUreg.reg[rs], CPUreg.reg[rt]); return 0;
        default: return -12;
    }
}


int32_t j_type(memory &mem, registers &CPUreg, program_counter &PC, const uint32_t instruction)
{
    const uint32_t address = instruction & 0x3FFFFC;
    const unsigned short type = instruction >> 25;

    if (mem_range_error(address)) return -11;

    switch (type)
    {
        case 0x02:
        	PC.load_PC(address);
        	return 0;
        default:
        	CPUreg.reg[31] = PC.get_PC();
        	PC.load_PC(address);
        	return 0;
    }

    return 0;
}


int32_t r_type(registers &CPUreg, program_counter &PC, const uint32_t instruction)
{
    const unsigned short rs = (instruction >> 21) & 0b11111;
    const unsigned short rt = (instruction >> 16) & 0b11111;
    const unsigned short rd = (instruction >> 11) & 0b11111;
    const unsigned short shift_size = (instruction >> 6) & 0b11111;
    const unsigned short funct = instruction & 0b111111;

    long long temp = 0;

    if (rd == 0) return -11;
    if (rs > 31 || rt > 31) return -11;

    switch (funct)
    {
    		case 0x20    : if (addition_exception(rs, rt)) {return -10;} CPUreg.reg[rd] = CPUreg.reg[rs] + CPUreg.reg[rt]; return 0;//signed addition
            case 0x21    : CPUreg.reg[rd] = (unsigned)CPUreg.reg[rs] + (unsigned)CPUreg.reg[rt]; return 0;// unsigned addition
            case 0x22    : CPUreg.reg[rd] = CPUreg.reg[rs] - CPUreg.reg[rt]; return 0; // signed subtraction
            case 0x23    : CPUreg.reg[rd] = (unsigned)CPUreg.reg[rs] - (unsigned)CPUreg.reg[rt]; return 0; // unsigned subtraction
            case 0x18    : temp=(CPUreg.reg[rs]*CPUreg.reg[rt]);     //signed multiplication
                           CPUreg.hi=(temp>>32);
                           CPUreg.lo=(temp & 0xFFFF); return 0;
            case 0x19    : temp=((unsigned)CPUreg.reg[rs]*(unsigned)CPUreg.reg[rt]);       //unsigned multiplication
                           CPUreg.hi=temp>>32;
                           CPUreg.lo=(temp<<32)>>32; return 0;
            case 0x1a    : if(division_error(rs, rt)) {return -10;} CPUreg.lo=CPUreg.reg[rs]/CPUreg.reg[rt];             //signed division
                           CPUreg.hi=CPUreg.reg[rs]%CPUreg.reg[rt]; return 0;
            case 0x1b    : if(division_error(rs, rt)) {return -10;} CPUreg.lo=(unsigned)CPUreg.reg[rs]/(unsigned)CPUreg.reg[rt];      //unsigned division
                           CPUreg.hi=CPUreg.reg[rs]%CPUreg.reg[rt];  return 0;
            case 0x10    : CPUreg.reg[rd]=CPUreg.hi; return 0;// move from hi
            case 0x12    : CPUreg.reg[rd]=CPUreg.lo; return 0;// move fromlo
            case 0x24    : CPUreg.reg[rd] = CPUreg.reg[rs] & CPUreg.reg[rt]; return 0;// bitwise AND
            case 0x25    : CPUreg.reg[rd] = CPUreg.reg[rs] | CPUreg.reg[rt]; return 0;//bitwise or
            case 0x26    : CPUreg.reg[rd] = CPUreg.reg[rs] ^ CPUreg.reg[rt]; return 0;// bitwise xor
            case 0x27    : CPUreg.reg[rd] = ~(CPUreg.reg[rs] | CPUreg.reg[rt]);return 0; //bitwise NOR
            case 0x2a    : if(CPUreg.reg[rs]<CPUreg.reg[rt]) CPUreg.reg[rd]=1; return 0;
            case 0x2b    : if(CPUreg.reg[rs]<CPUreg.reg[rt]) CPUreg.reg[rd]=1; return 0;
            case 0x00    : CPUreg.reg[rd] = CPUreg.reg[rt]<<shift_size; return 0;// shift size is the shift amount
            case 0x04    : CPUreg.reg[rd] = CPUreg.reg[rs]<<CPUreg.reg[rt]; return 0;
            case 0x02    : CPUreg.reg[rd] = CPUreg.reg[rt]>>shift_size; return 0;
            case 0x06    : CPUreg.reg[rd] = CPUreg.reg[rs]>>CPUreg.reg[rt]; return 0;
            case 0x03    : CPUreg.reg[rd] = CPUreg.reg[rt]>>shift_size; return 0; //arithmetic shift
            case 0x07    : CPUreg.reg[rd] = CPUreg.reg[rs]>>CPUreg.reg[rt]; return 0;// arithmetic shift with the amount
            case 0x08    : if (invalid_instruction(CPUreg.reg[rs])) {return -12;} PC.load_PC(CPUreg.reg[rs]); return 0;
            case 0x09    : if (RW_error(CPUreg.reg[rs])) {return -11;} CPUreg.reg[31] = PC.get_PC(); PC.load_PC(CPUreg.reg[rs]); return 0;
            default: return -12;
    }
}
