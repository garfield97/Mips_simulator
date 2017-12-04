#include <iostream>
#include <fstream>
#include "father.hpp"
#include <bitset>
#include <cinttypes>


using namespace std;


void write_csv(fstream &outfile, int id, string instruction, bool status, string author, string message);
string int2string(const int binary);


int main()
{
  fstream csv_out;
  int test_size;
  string in_name;
  string author_name;
  bool pass = false;
  string message;
  int result = 0;
  string instr_name;
  uint32_t binary;

  cout<<"Enter the number of test files:"<<endl;
  cin>>test_size;

  cout<<"Enter author name:"<<endl;
  cin>>author_name;

  csv_out.open("results.csv", ios::app);

  if (!(csv_out.is_open())) 
  {
    cout<<"Cannot open output file"<<endl; 
    exit(EXIT_FAILURE);
  }

  csv_out<<"TestId , Instruction , Status , Author , Message"<<endl;

  for (int i = 0; i < test_size; i++)
  {
    cout<<"Enter the name of file "<<i<<":";
    cin>>in_name;

    result = simulator(in_name, instr_name, binary);

    if (result == -1) pass = true;

    else 
    {
      pass = false;
      message = "Failed on " + instr_name + " with binary 0x" + int2string(binary);
    }

    write_csv(csv_out, i, inst, pass, author_name, message); //Print result in csv file and move on to next test
    message.clear();
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


string int2string(const int binary)
{
  int tmp;
  char hex[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

  string output;

  output.push_back(hex[(binary>>28)&0xF]);
  output.push_back(hex[(binary>>24)&0xF]);
  output.push_back(hex[(binary>>20)&0xF]);
  output.push_back(hex[(binary>>16)&0xF]);
  output.push_back(hex[(binary>>12)&0xF]);
  output.push_back(hex[(binary>>8)&0xF]);
  output.push_back(hex[(binary>>4)&0xF]);
  output.push_back(hex[binary&0xF]);

  return output;
}