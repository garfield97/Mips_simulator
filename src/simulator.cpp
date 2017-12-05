#include <iostream>
#include "headers/mem.hpp"
#include "headers/registers.hpp"
#include "headers/pc.hpp"
#include <fstream>
#include "headers/error_check.hpp"
#include <cinttypes>
#include <exception>


int mother(memory &mem, registers &CPUreg, program_counter &PC);
int read_file(memory &mem, std::fstream &infile);
int decode(memory &mem, registers &CPUreg,program_counter &PC, const  uint32_t instruction);
int i_type(memory &mem, registers &CPUreg, program_counter &PC, const uint32_t instruction);
int j_type(memory &mem, registers &CPUreg, program_counter &PC, const uint32_t instruction);
int r_type(registers &CPUreg, program_counter &PC, const uint32_t instruction);
uint32_t arithmetic_shift_right(uint32_t input,uint32_t shift_size);


int main(int argc, char *argv[])
{
    memory mem;
    registers CPUreg;
    program_counter PC;
    int result = 0; // Stores result of execution
    std::string tmp;
    bool show;

    std::string filename = argv[1];

    for (int i = 0; i < argc; i++) // Enables flags to be set 
    {
        tmp = argv[i];
        if (tmp == "-pause") pause = true; // Pause after each instruction and wait for enter key
        if (tmp == "-show")  show = true; // Show register values after each instruction
    }

    std::fstream infile;

    infile.open(filename.c_str(), std::ios::binary | std::ios::in);

    if (!infile.is_open()) {std::cout<<"Invalid filename"<<std::endl; return 0;}

    if (read_file(mem, infile) == -12) {std::cout<<"File too large to read"<<std::endl; result = -12;} //If = -11 then binary too large to store in instruction memory

    infile.close();

    while (result == 0)
    {
        result = mother(mem, CPUreg, PC);
        CPUreg.reg[0] = 0;
        PC.increment();
    }

    switch (result)
    {
        case -1: std::cout<<result<<" Execution completed successfully"<<std::endl; break;// Program completed successfully
        case -12: std::cout<<result<<" Invalid instruction"<<std::endl; break;// Invalid instruction
        case -11: std::cout<<result<<" Invalid address"<<std::endl; break;// Memory exception
        case -10: std::cout<<result<<" Arithmetic exception"<<std::endl; break;// Arithmetic exception
        case -20: std::cout<<result<<" Internal error"<<std::endl; break;// Unkown error
        default: std::cout<< result<<" Other error"<<std::endl; break;// Any other errors?
    }


    return result;
}


int mother(memory &mem, registers &CPUreg, program_counter &PC)
{
    uint32_t instruction = mem.get_instruction(PC.get_PC()); // Retrieve instruction from memory

    return (decode(mem, CPUreg, PC, instruction)); // Decode and execute
}


int read_file(memory &mem, std::fstream &infile)
{
    char c;
    uint32_t t = 0;
    uint32_t count = 0;

    while (!infile.eof())
    {
        for (int i = 0; i < 32; i++)
        {
            infile >> c;
            if (c == '1') t += (1 << (31-i));
        }

        if (t == 0x00000000) break;

        mem.store_instruction(count, t);
        t = 0;
        count += 4;
    }

    return 0;
}


int decode(memory &mem, registers &CPUreg,program_counter &PC, const uint32_t instruction)
{
    uint32_t opcode = instruction >> 26;

    switch (opcode)
    {
        case 0b000000: return (r_type(CPUreg, PC, instruction));// R - Type
        case 0b000010: return (j_type(mem, CPUreg, PC, instruction));// J - Type
        case 0b000011: return (j_type(mem, CPUreg, PC, instruction));// J - Type
        default: return (i_type(mem, CPUreg, PC, instruction)); // I - Type
    }
}


int i_type(memory &mem, registers &CPUreg, program_counter &PC, const unsigned int instruction)
{
    try
    {
    const unsigned short rs = (instruction >> 21) & 0b11111;
    const unsigned short rt = (instruction >> 16) & 0b11111;
    uint32_t IMM = instruction & 0XFFFF;
    const unsigned short opcode = instruction >> 26;

    //IMM = sign_extend(IMM);

    switch (opcode)
    {
        case 0x04: if (mem_range_error(PC.get_PC()+4+(IMM<<2))) {return -11;} if (CPUreg.reg[rs] == CPUreg.reg[rt]) {PC.load_PC(PC.get_PC()+4+(IMM<<2), true);} if (access_zero(IMM) == true) {return -1;} return 0; // beq
        case 0x05: if (mem_range_error(PC.get_PC()+4+(IMM<<2))) {return -11;} if (CPUreg.reg[rs] != CPUreg.reg[rt]) {PC.load_PC(PC.get_PC()+4+(IMM<<2), true);} return 0; // bne
        case 0x01: if (mem_range_error(PC.get_PC()+4+(IMM<<2))) {return -11;} 
                    if (CPUreg.reg[rt] == 0x1) {if (CPUreg.reg[rs] >= 0) PC.load_PC(PC.get_PC()+4+(IMM<<2), true); return 0;} //bgez
                    else if (CPUreg.reg[rt] == 0b10001) {if (CPUreg.reg[rs] >= 0) {CPUreg.reg[31] = PC.get_PC(); PC.load_PC(PC.get_PC()+4+(IMM<<2), true);} if (access_zero(IMM) == true) {return -1;}return 0;} //bgezal
                    else if (CPUreg.reg[rt] == 0x0) {if (CPUreg.reg[rs] < 0) {PC.load_PC(PC.get_PC()+4+(IMM<<2), true);} return 0;} //bltz
                    else if (CPUreg.reg[rt] == 0x10) {if (CPUreg.reg[rs] < 0) {CPUreg.reg[31] = PC.get_PC(); PC.load_PC(PC.get_PC()+4+(IMM<<2), true);} return 0;} //bltzal
                    return -12;
        case 0x07: if (mem_range_error(PC.get_PC()+4+(IMM<<2))) {return -11;}
                    if (CPUreg.reg[rs] > 0) {PC.load_PC(PC.get_PC()+4+(IMM<<2), true);} return 0; // bgtz
        case 0x06: if (mem_range_error(PC.get_PC()+4+(IMM<<2))) {return -11;} 
                    if (CPUreg.reg[rs] <= 0) {PC.load_PC(PC.get_PC()+4+(IMM<<2), true);} return 0; // blez
        case 0x08: if (addition_exception(CPUreg.reg[rs], IMM)) {return -10;} CPUreg.reg[rs] = CPUreg.reg[rt] + IMM; return 0; //addi
        case 0x09: CPUreg.reg[rs] = (unsigned)(CPUreg.reg[rt]) + (unsigned)(IMM); return 0; // addiu
        case 0x0A: if (CPUreg.reg[rs] < IMM) CPUreg.reg[rt] = 1; return 0; // slti
        case 0x0B: if ((unsigned)CPUreg.reg[rs] < (unsigned)(IMM)) CPUreg.reg[rt] = 1; return 0; // sltiu
        case 0x0C: CPUreg.reg[rs] = CPUreg.reg[rt] & IMM; return 0; // andi
        case 0x0D: CPUreg.reg[rs] = CPUreg.reg[rt] | IMM; return 0; // ori
        case 0x0F: mem.store_word(CPUreg.reg[rt], (IMM << 16)); return 0; // lui
        case 0x14: CPUreg.reg[rt] = CPUreg.reg[rs] ^ IMM; // xori
        case 0x23: CPUreg.reg[rt] = mem.load_word(IMM + CPUreg.reg[rs]); return 0; // lw
        case 0x24: CPUreg.reg[rt] = (unsigned)(mem.load_byte(IMM + CPUreg.reg[rs])); return 0; // lbu
        case 0x25: CPUreg.reg[rt] = (unsigned)(mem.load_hword(IMM + CPUreg.reg[rs])); return 0; // lhu
        case 0x28: if (write_to_zero(IMM + CPUreg.reg[rs])) {return -1;} mem.store_byte(IMM + CPUreg.reg[rs], CPUreg.reg[rt]); return 0; // sb
        case 0x29: if (write_to_zero(IMM + CPUreg.reg[rs])) {return -1;} mem.store_hword(IMM + CPUreg.reg[rs], CPUreg.reg[rt]); return 0; // sh
        case 0x30: if (write_to_zero(IMM + CPUreg.reg[rs])) {return -1;} mem.store_word(IMM + CPUreg.reg[rs], CPUreg.reg[rt]); return 0; // sw
        case 0x22: if (write_to_zero(IMM + CPUreg.reg[rs])) CPUreg.reg[rt] = mem.load_word_left(IMM + CPUreg.reg[rs]); return 0;// lwl
        case 0x26: if (write_to_zero(IMM + CPUreg.reg[rs])) CPUreg.reg[rt] = mem.load_word_right(IMM + CPUreg.reg[rs]); return 0;// lwr
        case 0x20: CPUreg.reg[rt] = (mem.load_byte(IMM + CPUreg.reg[rs])); return 0; // lb
        case 0x21: CPUreg.reg[rt] = (mem.load_hword(IMM + CPUreg.reg[rs])); return 0; // lh
        default: return -12;
    }
    }

    catch (const std::system_error &a)
    {
        return -20;
    }
}


int j_type(memory &mem, registers &CPUreg, program_counter &PC, const uint32_t instruction)
{
    try 
    {
    uint32_t address = instruction & 0x3FFFFC;
    const unsigned short type = instruction >> 26;
    uint32_t tmp = PC.get_PC()+4;

    address = address << 2;
    tmp = tmp & 0xF0000000;
    address += tmp;

    if (mem_range_error(address)) return -11;
    if (access_zero(address) == true) {return -1;}

    switch (type)
    {
        case 0x02:
        	PC.load_PC(address, true); // j
        	return 0;
        case 0x03:
        	CPUreg.reg[31] = PC.get_PC(); // jal
        	PC.load_PC(address, true);
        	return 0;
        default: return -12;
    }
    }

    catch(const std::system_error &a)
    {
        return -20;
    }
}


int r_type(registers &CPUreg, program_counter &PC, const uint32_t instruction)
{
    try {
    const unsigned short rs = (instruction >> 21) & 0b11111;
    const unsigned short rt = (instruction >> 16) & 0b11111;
    const unsigned short rd = (instruction >> 11) & 0b11111;
    const unsigned short shift_size = (instruction >> 6) & 0b11111;
    const unsigned short funct = instruction & 0b111111;

    unsigned long long temp = 0;


    switch (funct)
    {
    		case 0x20    : if (addition_exception(rs, rt)) {return -10;} CPUreg.reg[rd] = CPUreg.reg[rs] + CPUreg.reg[rt]; return 0;//add
            case 0x21    : CPUreg.reg[rd] = (unsigned)CPUreg.reg[rs] + (unsigned)CPUreg.reg[rt]; return 0;// addu
            case 0x22    : if (subtraction_exception(CPUreg.reg[rs], CPUreg.reg[rt])) {return -10;} CPUreg.reg[rd] = CPUreg.reg[rs] - CPUreg.reg[rt]; return 0; // sub
            case 0x23    : if (subtraction_exception(CPUreg.reg[rs], CPUreg.reg[rt])) {return -10;} CPUreg.reg[rd] = (unsigned)CPUreg.reg[rs] - (unsigned)CPUreg.reg[rt]; return 0; // subu
            case 0x18    : temp=(CPUreg.reg[rs]*CPUreg.reg[rt]);// mult
                           CPUreg.hi=(temp>>32);
                           CPUreg.lo=(temp & 0xFFFF); return 0;
            case 0x19    : temp=((unsigned)CPUreg.reg[rt]*(unsigned)CPUreg.reg[rs]);// multu
                           CPUreg.hi=temp>>32;
                           CPUreg.lo=(temp<<32)>>32; return 0;
            case 0x1a    : if(division_error(rs, rt)) {CPUreg.reg[rd] = 1;} // div
                            else {CPUreg.hi = CPUreg.reg[rs] % CPUreg.reg[rt]; CPUreg.lo = CPUreg.reg[rs]/CPUreg.reg[rt]; } return 0;
            case 0x1b    : if(division_error(rs, rt)) {CPUreg.reg[rd] = 1;} // divu
                            else {CPUreg.hi = (unsigned)CPUreg.reg[rs] % (unsigned)CPUreg.reg[rt]; CPUreg.lo = (unsigned)CPUreg.reg[rs]/(unsigned)CPUreg.reg[rt]; } return 0;
            case 0x10    : CPUreg.reg[rd]=CPUreg.hi; return 0;// mfhi
            case 0x12    : CPUreg.reg[rd]=CPUreg.lo; return 0;// mflo
            case 0x24    : CPUreg.reg[rd] = CPUreg.reg[rs] & CPUreg.reg[rt]; return 0;// and
            case 0x25    : CPUreg.reg[rd] = CPUreg.reg[rs] | CPUreg.reg[rt]; return 0;// or
            case 0x26    : CPUreg.reg[rd] = CPUreg.reg[rs] ^ CPUreg.reg[rt]; return 0;// xor
            case 0x27    : CPUreg.reg[rd] = ~(CPUreg.reg[rs] | CPUreg.reg[rt]);return 0; // nor
            case 0x2a    : if(CPUreg.reg[rt]<CPUreg.reg[rd]) CPUreg.reg[rs]=1; return 0; // slt
            case 0x2b    : if(CPUreg.reg[rt]<CPUreg.reg[rd]) CPUreg.reg[rs]=1; return 0; // sltu
            case 0x00    : CPUreg.reg[rd] = CPUreg.reg[rt]<<shift_size; return 0;// sll
            case 0x04    : CPUreg.reg[rd] = CPUreg.reg[rt]<<(CPUreg.reg[rs]&0x3F); return 0; // sllv
            case 0x02    : CPUreg.reg[rd] = CPUreg.reg[rt]>>shift_size; return 0; // srl
            case 0x06    : CPUreg.reg[rd] = CPUreg.reg[rt]>>(CPUreg.reg[rs]&0x3F); return 0; // srlv
            case 0x03    : CPUreg.reg[rd] = arithmetic_shift_right(CPUreg.reg[rt], shift_size); return 0; // sra
            case 0x07    : CPUreg.reg[rd] = arithmetic_shift_right(CPUreg.reg[rt], CPUreg.reg[rs]&0x3F); return 0;// srav
            case 0x11    : CPUreg.hi = CPUreg.reg[rd]; // mthi
            case 0x13    : CPUreg.lo = CPUreg.reg[rd]; // mtlo
            case 0x08    : if (invalid_instruction(CPUreg.reg[rs])) {return -12;} PC.load_PC(CPUreg.reg[rs], true);if (access_zero(CPUreg.reg[rs]) == true) {return -1;} return 0; //jr
            case 0x09    : CPUreg.reg[31] = PC.get_PC(); PC.load_PC(CPUreg.reg[rs], true); if (access_zero(CPUreg.reg[rs]) == true) {return -1;}return 0; //jalr
            default: return -12;
    }
    }

    catch (const std::system_error &a)
    {
        return -20;
    }
}


uint32_t arithmetic_shift_right(uint32_t input,uint32_t shift_size){

    uint32_t temp;
    temp=input;
    temp=temp>>31;
    
    long sign_extend_size=4294967296;
    
    if(temp==1){
        
        for(int i=0;i<shift_size;i++)
        {
            input=input>>1;
            input=input+sign_extend_size;  
        }        
    }
    if(temp==0)
    {
        input=input>>shift_size;
    
    }

    return input;
}



