#include <iostream>
#include "headers/mem.hpp"
#include <fstream>

void read_file(memory &memory, std::fstream &infile)
{
    char c;
    int t = 0;
    long count = 0;

    while (!(infile.eof()))
    {
        for (int i = 0; i < 8; i++)
        {
            infile.get(c);
            if (c == '1') t += (1 << (7-i));
        }
        memory.store_byte(count, t);
        t = 0;
        count++;
    }
}