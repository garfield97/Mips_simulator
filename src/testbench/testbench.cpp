#include <iostream>
#include <fstream>


using namespace std;


void write_csv(fstream &outfile, int id, string instruction, bool status, string author, string message);


int main()
{
  fstream csv_out;
  ofstream tst_in;
  int test_size;
  string in_base = "tst";
  string in_name;
  string author_name;
  bool pass = false;
  string inst;
  string message;

  cout<<"Enter the number of test files:"<<endl;
  cin>>test_size;

  cout<<"Enter author name:"<<endl;
  cin>>author_name;

  csv_out.open("results.csv");

  if (!csv_out.is_open()) cout<<"Cannot open output file<<endl"<<endl; exit(EXIT_FAILURE);

  csv_out<<"TestId , Instruction , Status , Author [, Message]"<<endl;

  for (int i = 0; i < test_size; i++)
  {
    in_name = in_base + (char)i + ".bin";

    tst_in.open(in_name.c_str());

    // Do testing and get result

    write_csv(csv_out, i, inst, pass, author_name, message); //Print result in csv file and move on to next test

    tst_in.close();
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
