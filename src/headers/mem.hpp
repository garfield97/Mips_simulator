#ifndef MEM_HPP
#define MEM_HPP

#include <iostream>
#include <cinttypes>

class memory
{
    public:

    memory();
    uint32_t get_instruction(uint32_t location) const;
    uint32_t load_word(uint32_t loc);
    uint32_t load_byte(uint32_t loc) const;
    uint32_t load_hword(uint32_t loc) const;
    void store_word(uint32_t loc, const int32_t input);
    void store_hword(uint32_t loc, const int32_t input);
    void store_byte(uint32_t loc, const int32_t input);
    uint32_t get_c(uint32_t loc_corrected);
    void put_c(int32_t item);

    private: 

    uint32_t *mem;
};


#endif
