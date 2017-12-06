#include <iostream>
#include <fstream>

using namespace std;


void write_csv(fstream &outfile, string id, string instruction, bool status, string author, string argument);


int main(int argc, char **argv)
{
    string filename = argv[1];
    string pass = argv[2];
    string test_name =argv[3];
    string name = argv[4];
    string ref = argv[5];
    string type = argv[6];
    string argument;

    if(type =="-1") argument = "No errors";
    
    else if(type == "-10") argument = "Arithmetic errror";
    
    else if(type == "-11") argument =  "Memory Exception";
    
    else if(type == "-12") argument =  "Invalid Instruction";
    
    else if(type == "-20") argument =  "Internal Error";
    
    else argument = "Unkown error";
      
        
    bool passed = false;

    if (pass == "pass") passed = true;

    fstream csv_out;

    csv_out.open(filename.c_str(), ios::out);

    if (!csv_out.is_open())
    {
      exit(EXIT_FAILURE);
    }

    string tmp;

    csv_out>>tmp;

    if (tmp != "TestId") csv_out<<"TestId , Instruction , Status , Author, Message"<<endl;

    write_csv(csv_out, ref, test_name, passed, name, argument);

    csv_out.close();

    return 0;
}


void write_csv(fstream &outfile, string id, string instruction, bool status, string author, string argument)
{
  if (status)
  {
    outfile<<id<<", "<<instruction<<", "<<"Pass"<<", "<<author<<", "<<argument<<", "; 
    return;
  }

  outfile<<id<<", "<<instruction<<", "<<"Fail"<<", "<<author<<", "<<argument<<", "; 
}