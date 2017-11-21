#ifndef PC_HPP
#define PC_HPP

#include <iostream>

class program_counter
{
public:

    //Initialise PC to 0
    program_counter();
    //To increment PC
    void increment();
    //To load a new value into PC
    void load_PC( int instr);
    // Getter for PC
    int get_PC() const;


private:

    int PC;
    
};

#endif
