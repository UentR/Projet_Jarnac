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
    File.open("Text/Arbre.txt");
    Node *T;
    map<string, Node *> Tree;
    string letter;
    int Size;
    for (int i = 0; i < 124492; i++)
    {
        T = new Node;
        File >> T->Ana;

        File >> letter;
        File >> letter;

        while (letter != "]")
        {
            T->Children[letter] = Tree[Sort((T->Ana + letter))];
            File >> letter;
        }

        Tree[Sort(T->Ana)] = T;
    }
}
