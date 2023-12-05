#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <tuple>
#include <iostream>
#include <unistd.h>

using namespace std;

struct Node
{
    string Ana;
    map<char, Node *> Children;
};

string Sort(string Mot)
{
    string Sorted = Mot;
    sort(Sorted.begin(), Sorted.end());
    return Sorted;
}

string fRetire(string Word, char Letter)
{
    string newString = "";
    for (char character : Word)
    {
        if (character != Letter)
        {
            newString += character;
        }
    }
    return newString;
}

tuple<Node *, string> Analyze(string Vrac, vector<Node *> PlayerWords)
{
    set<tuple<Node *, string, string, Node *>> toAnalyze = {};
    for (Node *Word : PlayerWords)
    {
        toAnalyze.insert(make_tuple(Word, Vrac, "", Word));
    }

    set<tuple<Node *, string>> End = {};

    Node *Word;
    string tVrac;
    string L;
    bool Child;
    string Path;
    Node *Origin;
    while (!toAnalyze.empty())
    {
        tie(Word, tVrac, Path, Origin) = *toAnalyze.begin();
        toAnalyze.erase(toAnalyze.begin());

        Child = false;
        for (char Letter : tVrac)
        {
            if (Word->Children.find(Letter) != Word->Children.end())
            {
                Child = true;
                toAnalyze.insert(make_tuple(Word->Children[Letter], fRetire(tVrac, Letter), Path + Letter, Origin));
            }
        }
        if (!Child)
        {
            End.insert(make_tuple(Origin, Path));
        }
    }

    int Longest = 0;
    string BestPath;
    Node *BestStart;
    for (tuple<Node *, string> EndWord : End)
    {
        tie(Word, Path) = EndWord;
        if (Path.length() > Longest)
        {
            Longest = Path.length();
            BestPath = Path;
            BestStart = Word;
        }
    }
    return make_tuple(BestStart, BestPath);
}


string Create3LettersWord(string Vrac, set<string> ShortWords) {
    
}



/**
 * Loads a tree data structure from a text file.
 *
 * The text file contains words, with child letters separated by spaces,
 * and different words separated by semicolons.
 *
 * This builds up a tree mapping prefixes to child nodes.
 * It creates Node objects to represent each node in the tree.
 *
 * It uses a helper Sort function to sort the letters in each word,
 * so that anagrams map to the same node.
 */
map<string, Node *> LoadTree(string FileName)
{
    ifstream File;
    Node *T;
    map<string, Node *> Tree;
    string letter;

    File.open(FileName);
    T = new Node;

    while (File >> T->Ana)
    {

        File >> letter;

        while (letter != ";")
        {
            T->Children[letter[0]] = Tree[Sort((T->Ana + letter))];
            File >> letter;
        }

        Tree[Sort(T->Ana)] = T;
        T = new Node;
    }
    File.close();
    return Tree;
}

int main()
{
    map<string, Node *> Tree = LoadTree("Text/Arbre.txt");
    cout << "Tree loaded" << endl;
    vector<string> WORDS = {
        "OUT",
        "OSA",
        "TON",
        "ZEN",
        "NON",
        "MEO",
        "SKI",
        "RIE"};
    vector<Node *> PlayerWords = {};
    for (string Word : WORDS)
    {
        PlayerWords.push_back(Tree[Sort(Word)]);
    }

    cout << "Player words loaded" << endl;

    // 5s pause
    for (int i = 0; i < 5; i++)
    {
        cout << i << endl;
        sleep(1);
    }

    string Vrac = "UGYFT";
    Node *Origin;
    string Path;
    tie(Origin, Path) = Analyze(Vrac, PlayerWords);
    cout << "Longest path: " << Path << endl;
    cout << "Longest path length: " << Path.length() << endl;
    cout << "Origin: " << Origin->Ana << endl;

    cout << "To place:" << endl;
    Node *Temp = Origin;
    cout << Temp->Ana << endl;
    for (char Letter : Path)
    {
        Temp = Temp->Children[Letter];
        cout << Temp->Ana << endl;
    }
}
