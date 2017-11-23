#ifndef MEM_HPP
#define MEM_HPP

#include <iostream>

class memory
{
    public:

    memory();
    unsigned int get_instruction(int location) const;
    unsigned int load_word(int loc) const;
    unsigned int load_byte( int loc) const;
    unsigned int load_hword( int loc) const;
    void store_word(int loc,  const int input);
    void store_hword(int loc,  const int input);
    void store_byte(int loc,  const int input);

    private: 

    unsigned int *mem;
};


#endif
