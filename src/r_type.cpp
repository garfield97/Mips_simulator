#include <iostream>
#include "headers/mem.hpp"
#include "headers/registers.hpp"
#include "headers/pc.hpp"


void r_type(memory &memory, registers &registers, program_counter &PC, const  int instruction)
{
    const int rs = (instruction >> 21) & 0b11111;
    const int rt = (instruction >> 16) & 0b11111;
    const int rd = (instruction >> 11) & 0b11111;
    const int shift_size = (instruction >> 6) & 0b11111;
    const int funct = instruction & 0b111111;

    switch (funct)
    {
        case 0x20    : registers.reg[rd] = registers.reg[rs] + registers.reg[rt]; //signed addition
        case 0x21    : registers.reg[rd] = registers.reg[rs] + registers.reg[rt]; //  addition
        case 0x22    : registers.reg[rd] = registers.reg[rs] - registers.reg[rt];  // signed subtraction
        case 0x23    : registers.reg[rd] = registers.reg[rs] - registers.reg[rt];  //  subtraction
        case 0x18    : // signed multiplication  
        case 0x19    :    //  multiplcation
        case 0x1a    :   // signed division
        case 0x1b    :     //  division
        case 0x10    : // move from HI
        case 0x12    : // Move from LO
        case 0x24    : registers.reg[rd] = registers.reg[rs] & registers.reg[rt]; // bitwise AND
        case 0x25    : registers.reg[rd] = registers.reg[rs] | registers.reg[rt]; //bitwise or
        case 0x26    : registers.reg[rd] = registers.reg[rs] ^ registers.reg[rt]; // bitwise xor
        case 0x27    : registers.reg[rd] = ~(registers.reg[rs] | registers.reg[rt]); //bitwise NOR
        case 0x2a    : if(registers.reg[rs]<registers.reg[rt]) registers.reg[rd]=1;
                       else registers.reg[rd]=0;        
        case 0x2b    : if(registers.reg[rs]<registers.reg[rt]) registers.reg[rd]=1;
                       else registers.reg[rd]=0;
        case 0x00    : registers.reg[rd] = registers.reg[rt]<<shift_size ;// shift size is the shift amount
        case 0x04    : registers.reg[rd] = registers.reg[rs]<<registers.reg[rt];
        case 0x02    : registers.reg[rd] = registers.reg[rt]>>shift_size;
        case 0x06    : registers.reg[rd] = registers.reg[rs]>>registers.reg[rt];
        case 0x03    : registers.reg[rd] = registers.reg[rt]>>shift_size; //arithmetic shift
        case 0x07    : registers.reg[rd] = registers.reg[rs]>>registers.reg[rt]; // arithmetic shift with the amount
        case 0x08    : PC.load_PC(registers.reg[rs]);
        case 0x09    : registers.reg[31] = PC.get_PC() + 4;
                       PC.load_PC(registers.reg[rs]);
        case 0x0c    :; //0c goes here

        
    }
}
