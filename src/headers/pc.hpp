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
    void load_PC(const int &new_PC);
    // Getter for PC
    unsigned int get_PC() const;

    int get_PC();

    void load_PC(unsigned int loc);

private:

    unsigned int PC;
    
};

#endif