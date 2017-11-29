#include <iostream>
#include <fstream>


using namespace std;


int main()
{
  fstream csv_out;
  ofstream tst_in;
  int test_size;
  string in_base = "tst";
  string in_name;

  cout<<"Enter the number of test files"<<endl;
  cin>>test_size;

  csv_out.open("results.csv");

  csv_out<<"TestId , Instruction , Status , Author [, Message]"<<endl;

  for (int i = 0; i < test_size; i++)
  {
    in_name = in_base + (char)i + ".bin";

    tst_in.open(in_name.c_str());

    // Do testing and get output

    write_csv();
  }

}


void write_csv()
{

}
