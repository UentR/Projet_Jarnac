#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <vector>

using namespace std;

struct Node
{
    string Ana;
    map<string, int *> Children;
};

int main()
{
	
		Node * Test = new Node;
		Test->Ana = "Test";
	int *Temp = new int(10);
	Test->Children = {{"a", Temp}};
	cout << Test << endl;
	cout << Test->Children["a"] << endl;

    /*char *Nom3 = new char[9];

    string Nom2 = "Test";

    for (int i = 0; i < Nom2.length(); i++)
    {
        Nom[i] = Nom2[i];
    }

    *Nom3 = *Nom;

    Nom2 = "Test2";

    for (int i = 0; i < Nom2.length(); i++)
    {
        Nom[i] = Nom2[i];
    }

    cout << Nom << endl;
    cout << Nom3 << endl;
    */
    // ifstream File;
    // File.open("Text/After2.json");
    // map<string, Node> MAP;
    // string letter;
    // vector<int *>
    // for (int i = 0; i < 124492; i++)
    // {
    //     string Nom;
    //     int Nbr;
    //     int *Pointer;

    //     Nbr = 0;
    //     File >> Nom;
    //     File >> letter;
    //     Nbr = 0
    //     while (letter != "]") {

    //     }
    // }
}
