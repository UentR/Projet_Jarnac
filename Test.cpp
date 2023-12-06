#include <iostream>
#include <vector>

using namespace std;

struct Temp
{
    vector<long> TT;
};


void modif(Temp *Test) {
    Test->TT.push_back(1);
    // return Test;
}


int main()
{
    Temp *Test = new Temp;
    modif(Test);
    Test->TT.push_back(524288);
    cout << Test->TT[1] << endl;
}