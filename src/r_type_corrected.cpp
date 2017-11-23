#include <iostream>
#include "headers/mem.hpp"
#include "headers/registers.hpp"
#include "headers/pc.hpp"
#include <cmath>

void r_type(memory &memory, registers &registers, program_counter &PC, const unsigned int instruction)
{
    const int rs = (instruction >> 21) & 0b11111;
    const int rt = (instruction >> 16) & 0b11111;
    const int rd = (instruction >> 11) & 0b11111;
    const int shift = (instruction >> 6) & 0b11111;
    const int funct = instruction & 0b111111;

    switch (funct)
    {
        case 0x20    : registers.reg[rd] = registers.reg[rs] + registers.reg[rt]; //signed addition
        case 0x21    : registers.reg[rd] = registers.reg[rs] + registers.reg[rt]; // unsigned addition
        case 0x22    : registers.reg[rd] = registers.reg[rs] - registers.reg[rt];  // signed subtraction
        case 0x23    : registers.reg[rd] = registers.reg[rs] - registers.reg[rt];  // unsigned subtraction
        case 0x18    : registers.hi=(registers.reg[rs]*registers.reg[rt])>>32;       //signed multiplication
                       registers.lo=(registers.reg[rs]*registers.reg[rt])<<32)>>32;
        case 0x19    : registers.hi=(registers.reg[rs]*registers.reg[rt])>>32;       //unsigned multiplication
                       registers.lo=(registers.reg[rs]*registers.reg[rt])<<32)>>32;
        case 0x1a    : registers.lo=registers.reg[rs]/registers.reg[rt];             //signed division
                       registers.hi=registers.reg[rs]%registers.reg[rt]; 
        case 0x1b    : registers.lo=registers.reg[rs]/registers.reg[rt];        //unsigned division
                       registers.hi=registers.reg[rs]%registers.reg[rt];  
        case 0x10    : registers.reg[rd]=registers.hi; // move from hi
        case 0x12    : registers.reg[rd]=registers.lo; // move fromlo
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
        case 0x08    : PC.load_pc(registers.reg[rs]);
        default      : registers.reg[31] = pc+4;
                       PC.load_pc(registers.reg[rs]);
        //case 0x0c    :  // system call for OS services

        
    }
}

// ABS ALL THE OPERANDS;