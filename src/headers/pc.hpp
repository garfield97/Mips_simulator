#ifndef PC_HPP
#define PC_HPP

#include <iostream>
#include <cinttypes>

class program_counter
{
public:

    //Initialise PC to 0
    program_counter();
    //To increment PC
    void increment();
    //To load a new value into PC
    void load_PC(uint32_t instr);
    // Getter for PC
    uint32_t get_PC() const;


private:

    uint32_t PC;
    
};

#endif
