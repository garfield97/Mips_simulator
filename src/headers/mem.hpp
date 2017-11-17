#ifndef MEM_HPP
#define MEM_HPP

#include <iostream>

class memory
{
    public:

    memory();
    unsigned int get_instruction(int location) const;
    int load_word(unsigned int loc) const;
    int load_byte(unsigned int loc) const;
    int load_hword(unsigned int loc) const;
    void store_word(int loc,  const int input);
    void store_hword(int loc,  const int input);
    void store_byte(int loc,  const int input);

    private: 

    int *mem;
};


#endif