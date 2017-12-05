#include <iostream>
#include <fstream>
#include <cinttypes>

using namespace std;

void write_csv(fstream &outfile, int id, string instruction, bool status, string author, string message);


int main(int argc, char **argv)
{
    string file_name;
    int number_of_files = 1;
    string author_name;
    fstream csv_out;
    int test_size;
    string in_name;
    bool pass = false;
    string inst;
    string message;
    int result = 0;
    string instr_name;


    cout<<"Enter author name:"<<endl;
    cin>>author_name;

    csv_out.open("results.csv", ios::app);

    if (!csv_out.is_open())
    {
     cout<<"Cannot open output file"<<endl; 
     exit(EXIT_FAILURE);
    }

    string tmp;

    csv_out>>tmp;

    if (tmp != "TestId") csv_out<<"TestId , Instruction , Status , Author"<<endl;

    string path = argv[1];
    string test_file = argv[2];
    string execute = path + " " + test_file;

    result = system(execute.c_str()); //Calls the simulator with test file location and binary test

    if (result == 65280) 
    { write_csv(csv_out, 1, inst, true, author_name, message); 
      cout<<"Success code "<<(signed)result<<endl;
    }
    else 
    {write_csv(csv_out, 1, inst, false, author_name, message);
    cout<<"failure "<<(signed)result<<endl;
    }
    csv_out.close();

    return 0;
}


void write_csv(fstream &outfile, int id, string instruction, bool status, string author, string message)
{
  if (status)
  {
    outfile<<id<<", "<<instruction<<", "<<"Pass"<<", "<<author<<", "<<message<<", "<<endl; 
    return;
  }

  outfile<<id<<", "<<instruction<<", "<<"Fail"<<", "<<author<<", "<<message<<", "<<endl;
}