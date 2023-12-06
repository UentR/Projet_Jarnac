#include <iostream>

using namespace std;

int main()
{
    string Temp = "Test";
    string Deb = "\033[31;1m";
    cout << Deb << Temp << "\033[0m" << endl;
}