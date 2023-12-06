// un bibliotheque pour jouer avec les anagrammes
#include <string>
#include <algorithm>
#include <set>
#include <vector>

using namespace std;

#include "mDictionnaire.hpp"

struct ForDict;

string retire(string mot, char lettre)
{
    string Rep = "";
    char Current;
    int Count = 0;
    for (char Current : mot)
    {
        if (Current == lettre)
        {
            break;
        }
        Rep += Current;
        Count++;
    }
    if (Count == mot.length())
        return "-";
    for (int i = Count + 1; i < mot.size(); i++)
    {
        Current = mot[i];
        Rep += Current;
    }
    return Rep;
}

string retire(string vrac, string mot)
{
    string Rep = vrac;
    for (char i : mot)
    {
        Rep = retire(Rep, i);
    }
    return Rep;
}

bool est_anagramme(string mot1, string mot2)
{
    return retire(mot1, mot2) != "-";
}

set<string> listeDesAnagrammes(string mot)
{
    set<string> Temp = {};
    sort(mot.begin(), mot.end());
    do
    {
        Temp.insert(mot);
    } while (next_permutation(mot.begin(), mot.end()));
    return Temp;
}

set<string> listeDesAnagrammesDuDico(string mot, ForDict *DictHelper)
{
    set<string> Rep = {};
    set<string> Ana = listeDesAnagrammes(mot);
    for (auto i : Ana)
    {
        if (trouve(i, DictHelper))
        {
            Rep.insert(i);
        }
    }
    return Rep;
}

// int main()
// {

//     CHECK(retire("bonjour", 'o') == "bnjour");

//     CHECK(retire("bonjour", 'p') == "-");

//     CHECK(est_anagramme("rien", "rien"));
//     CHECK(est_anagramme("nier", "rein"));

//     vector<string> Dico = importeDico("Guidé/DictionnairePurified.txt");
//     vector<long> BORNES = CreateBorne(Dico);

//     CHECK((listeDesAnagrammesDuDico("rien", Dico) == set<string>{"rien", "nier", "rein"}));
//     //  afficheListedeMots( listeDesAnagrammesDuDico("rien","dictionnaire.txt"));

//     CHECK(retire("abcd", "bac") == "d");
//     CHECK(retire("ab", "bac") == "-");
//     CHECK(retire("abc", "bac") == "");
//     CHECK(retire("famille", "m") == "faille");
//     CHECK(retire("garder", "regard") == "");
//     CHECK(retire("garder", "regarde") == "-");
//     CHECK((retire("brossent", "sorbet") == "ns") or (retire("brossent", "sorbet") == "sn"));

//     listeDesAnagrammes("TEST");
// }
