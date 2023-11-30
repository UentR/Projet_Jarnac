#include <fstream>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

struct Node
{
    string Ana;
    map<string, Node *> Children;
};

string Sort(string Mot)
{
    string Sorted = Mot;
    sort(Sorted.begin(), Sorted.end());
    return Sorted;
}

int main()
{
    ifstream File;
    Node *T;
    map<string, Node *> Tree;
    string letter;

    File.open("Text/Arbre.txt");
    T = new Node;

    while (File >> T->Ana)
    {

        File >> letter;

        while (letter != ";")
        {
            T->Children[letter] = Tree[Sort((T->Ana + letter))];
            File >> letter;
        }

        Tree[Sort(T->Ana)] = T;
        T = new Node;
    }
    File.close();
}
