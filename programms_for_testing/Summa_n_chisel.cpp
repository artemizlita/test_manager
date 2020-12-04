#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

int main()
{
    long int sum = 0;
    string str;
    ifstream fin;
    fin.open("in.txt");
    while (getline(fin, str)) {
        sum += atoi(str.c_str());
    }
    fin.close();

    ofstream fout;
    fout.open("out.txt");
    fout << sum;
    fout.close();
    return 0;
}

