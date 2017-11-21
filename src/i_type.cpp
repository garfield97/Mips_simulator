#include <iostream>
#include "headers/mem.hpp"
#include "headers/registers.hpp"
#include "headers/pc.hpp"
#include <cmath>


void i_type(memory &memory, registers &registers, program_counter &PC, const  int instruction)
{
    const int rs = (instruction >> 21) & 0b11111;
    const int rt = (instruction >> 16) & 0b11111;
    const int IMM = instruction & 0XFF;
    const int opcode = instruction >> 26;

    switch (opcode)
    {
        case 0x04: if (registers.reg[rs] == registers.reg[rt]); //BEQ
        case 0x05: if (registers.reg[rs] != registers.reg[rt]); //BNE
        case 0x08: registers.reg[rt] = registers.reg[rs] + IMM;
        case 0x09: registers.reg[rt] = (unsigned)(registers.reg[rs]) + (unsigned)(IMM);
        case 0x0A: if (registers.reg[rs] < IMM) registers.reg[rt] = 1;
        case 0x0B: if (registers.reg[rs] < (unsigned)(IMM)) registers.reg[rt] = 1;
        case 0x0C: registers.reg[rt] = registers.reg[rs] & IMM;
        case 0x0D: registers.reg[rt] = registers.reg[rs] | IMM;
        case 0x0F: registers.reg[rt] = IMM << 16;
        case 0x23: registers.reg[rt] = memory.load_word(registers.reg[rs]);
        case 0x24: registers.reg[rt] = (unsigned)(memory.load_byte(registers.reg[rs]));
        case 0x25: registers.reg[rt] = (unsigned)(memory.load_hword(registers.reg[rs]));
        case 0x28: memory.store_byte(registers.reg[rs], registers.reg[rt]);
        case 0x29: memory.store_hword(registers.reg[rs], registers.reg[rt]);
        default: memory.store_word(registers.reg[rs], registers.reg[rt]);
    }
}
