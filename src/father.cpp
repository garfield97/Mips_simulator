#include <iostream>
#include "mother.cpp"
#include "headers/mem.hpp"
#include "headers/registers.hpp"
#include "headers/pc.hpp"
#include <fstream>
#include "read_file.cpp"


int main(int argc, char **argv)
{
    memory memory;
    registers registers;
    program_counter PC;

    std::string filename = argv[1];

    std::fstream infile;

    infile.open(filename.c_str());

    read_file(memory, infile);

    mother(memory, registers, PC);

    return 0;
}
