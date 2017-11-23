#include <iostream>

#include "headers/mem.hpp"
#include "headers/registers.hpp"
#include "headers/pc.hpp"
#include <fstream>


void mother(memory &memory, registers &registers, program_counter &PC);
void read_file(memory &memory, std::fstream &infile);
void decode(memory &memory, registers &registers,program_counter &PC, const unsigned int instruction);
void i_type(memory &memory, registers &registers, program_counter &PC, const  int instruction);
void j_type(memory &memory, registers &registers, program_counter &PC, const   int instruction);
void r_type(memory &memory, registers &registers, program_counter &PC, const  int instruction);


int main(int argc, char **argv)
{
    memory memory;
    registers registers;
    program_counter PC;

    std::string filename = argv[1];

    std::fstream infile;

    infile.open(filename.c_str());

    read_file(memory, infile);

    mother(memory, registers, PC);

    return 0;
}

void mother(memory &memory, registers &registers, program_counter &PC)
{
    do
    {
     int instruction = memory.get_instruction(PC.get_PC()); // Retrieve instruction from memory

    decode(memory, registers, PC, instruction); // Decode and execute


    PC.increment();
    }

    while (PC.get_PC() != 0);
}
void read_file(memory &memory, std::fstream &infile)
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
        memory.store_byte(count, t);
        t = 0;
        count++;
    }
}
void decode(memory &memory, registers &registers,program_counter &PC, const unsigned int instruction)
{
    unsigned short opcode = instruction >> 25;

    switch (opcode)
    {
        case 0b000000: r_type(memory, registers, PC, instruction); break;// R - Type
        case 0b000010: j_type(memory, registers, PC, instruction); break;// J - Type
        case 0b000011: r_type(memory, registers, PC, instruction); break;// J - Type
        default: i_type(memory, registers, PC, instruction); break;// I - Type
    }
}
void i_type(memory &memory, registers &registers, program_counter &PC, const  int instruction)
{
    const int rs = (instruction >> 21) & 0b11111;
    const int rt = (instruction >> 16) & 0b11111;
    const int IMM = instruction & 0XFF;
    const int opcode = instruction >> 26;

    switch (opcode)
    {
        case 0x04: if (registers.reg[rs] == registers.reg[rt]); break; //BEQ ADD PC LOAD THING HERE
        case 0x05: if (registers.reg[rs] != registers.reg[rt]); break; //BNE AND HERE
        case 0x08: registers.reg[rt] = registers.reg[rs] + IMM; break;
        case 0x09: registers.reg[rt] = (unsigned)(registers.reg[rs]) + (unsigned)(IMM); break;
        case 0x0A: if (registers.reg[rs] < IMM) registers.reg[rt] = 1; break;
        case 0x0B: if (registers.reg[rs] < (unsigned)(IMM)) registers.reg[rt] = 1; break;
        case 0x0C: registers.reg[rt] = registers.reg[rs] & IMM; break;
        case 0x0D: registers.reg[rt] = registers.reg[rs] | IMM; break;
        case 0x0F: registers.reg[rt] = IMM << 16; break;
        case 0x23: registers.reg[rt] = memory.load_word(registers.reg[rs]); break;
        case 0x24: registers.reg[rt] = (unsigned)(memory.load_byte(registers.reg[rs])); break;
        case 0x25: registers.reg[rt] = (unsigned)(memory.load_hword(registers.reg[rs])); break;
        case 0x28: memory.store_byte(registers.reg[rs], registers.reg[rt]); break;
        case 0x29: memory.store_hword(registers.reg[rs], registers.reg[rt]); break;
        default: memory.store_word(registers.reg[rs], registers.reg[rt]); break;
    }
}
void j_type(memory &memory, registers &registers, program_counter &PC, const   int instruction)
{
    const int address = instruction & 0x3FFFFC;
    const short type = instruction >> 25;

    switch (type)
    {
        case 0x02:
        	PC.load_PC(address);
        	break;
        default:
        	registers.reg[31] = PC.get_PC();
        	PC.load_PC(address);
        	break;
    }
}
void r_type(memory &memory, registers &registers, program_counter &PC, const  int instruction)
{
    const int rs = (instruction >> 21) & 0b11111;
    const int rt = (instruction >> 16) & 0b11111;
    const int rd = (instruction >> 11) & 0b11111;
    const int shift_size = (instruction >> 6) & 0b11111;
    const int funct = instruction & 0b111111;

    switch (funct)
    {
        case 0x20    : registers.reg[rd] = registers.reg[rs] + registers.reg[rt];  break;//signed addition
        case 0x21    : registers.reg[rd] = registers.reg[rs] + registers.reg[rt]; break; //  addition
        case 0x22    : registers.reg[rd] = registers.reg[rs] - registers.reg[rt]; break;  // signed subtraction
        case 0x23    : registers.reg[rd] = registers.reg[rs] - registers.reg[rt]; break;  //  subtraction
        case 0x18    : // signed multiplication
        case 0x19    :    //  multiplcation
        case 0x1a    :   // signed division
        case 0x1b    :     //  division
        case 0x10    : // move from HI
        case 0x12    : // Move from LO
        case 0x24    : registers.reg[rd] = registers.reg[rs] & registers.reg[rt];  break;// bitwise AND
        case 0x25    : registers.reg[rd] = registers.reg[rs] | registers.reg[rt];  break;//bitwise or
        case 0x26    : registers.reg[rd] = registers.reg[rs] ^ registers.reg[rt]; break; // bitwise xor
        case 0x27    : registers.reg[rd] = ~(registers.reg[rs] | registers.reg[rt]); break; //bitwise NOR
        case 0x2a    : if(registers.reg[rs]<registers.reg[rt]) registers.reg[rd]=1;
                       else registers.reg[rd]=0; break;
        case 0x2b    : if(registers.reg[rs]<registers.reg[rt]) registers.reg[rd]=1;
                       else registers.reg[rd]=0; break;
        case 0x00    : registers.reg[rd] = registers.reg[rt]<<shift_size; break;// shift size is the shift amount
        case 0x04    : registers.reg[rd] = registers.reg[rs]<<registers.reg[rt]; break;
        case 0x02    : registers.reg[rd] = registers.reg[rt]>>shift_size; break;
        case 0x06    : registers.reg[rd] = registers.reg[rs]>>registers.reg[rt]; break;
        case 0x03    : registers.reg[rd] = registers.reg[rt]>>shift_size; break; //arithmetic shift
        case 0x07    : registers.reg[rd] = registers.reg[rs]>>registers.reg[rt]; break; // arithmetic shift with the amount
        case 0x08    : PC.load_PC(registers.reg[rs]); break;
        case 0x09    : registers.reg[31] = PC.get_PC() + 4;
                       PC.load_PC(registers.reg[rs]); break;
        case 0x0c    : /*0c goes here*/ ;  break;


    }
}
