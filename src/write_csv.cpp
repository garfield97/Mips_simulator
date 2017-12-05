#include <iostream>
#include <fstream>

using namespace std;


void write_csv(fstream &outfile, string id, string instruction, bool status, string author);


int main(int argc, char **argv)
{
    string filename = argv[1];
    string pass = argv[2];
    string test_name =argv[3];
    string name = argv[4];
    string ref = argv[5];
    string type = argv[6];
    string tmp;
    string argument;


    bool passed = false;

    if (pass == "pass") passed = true;

    fstream csv_out;

    csv_out.open(filename.c_str(), ios::out);

    if (!csv_out.is_open())
    {
      cout<<"Failed"<<endl;
      exit(EXIT_FAILURE);
    }

        fflush(stdin);
    getchar();
    cout<<"schmei"<<endl;

    csv_out<<"TestId , Instruction , Status , Author"<<endl;

    write_csv(csv_out, ref, test_name, passed, name);

    csv_out.close();

    return 0;
}


void write_csv(fstream &outfile, string id, string instruction, bool status, string author)
{
  if (status)
  {
    outfile<<id<<", "<<instruction<<", "<<"Pass"<<", "<<author<<", "<<endl; 
    return;
  }

  outfile<<id<<", "<<instruction<<", "<<"Fail"<<", "<<author<<", "<<endl;
}