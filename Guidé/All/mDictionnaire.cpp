#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <math.h>
using namespace std;

vector<string> importeDico(string adresseDico)
{
    ifstream File;
    File.open(adresseDico);
    string Word;
    vector<string> Dico = {};
    int Nbr = 0;
    while (File >> Word and Nbr < pow(2, 18))
    {
        Nbr++;
        Dico.push_back(Word);
    }
    return Dico;
}

int CalcIdx(string Word, int NBR)
{
    int Idx = 0;
    int Mult = 1;
    for (int j = NBR - 1; j >= 0; j--)
    {
        Idx += (Word[j] - 65) * Mult;
        Mult *= 26;
    }
    return Idx;
}

vector<long> CreateBorne(vector<string> Dico, int NBR, int SHIFT)
{
    if (SHIFT > 32)
    {
        cerr << "Dictionnaire trop grand, overflow" << endl;
        exit(136);
    }
    double Size = pow(26, NBR);
    vector<long> Borne = vector<long>(Size, 0);
    string First = Dico[0].substr(0, NBR);
    long BorneInf = 0;
    long BorneSup = 0;
    int Idx;
    string Word;
    for (int i = 0; i < Dico.size(); i++) // parcours du dico
    {
        Word = Dico[i];                // lecture dico
        if (Word.rfind(First, 0) != 0) // rupture sur les NBR premiers caractères ?
        {                              // OUI
            Idx = CalcIdx(First, NBR);
            BorneSup = i;
            Borne[Idx] = (BorneSup << SHIFT) + (BorneInf); // Se référer à la doc
            BorneInf = i;
            First = Word.substr(0, NBR);
        }
    }
    Idx = CalcIdx(First, NBR);
    BorneSup = Dico.size();
    Borne[Idx] = (BorneSup << SHIFT) + (BorneInf); // Se référer à la doc
    return Borne;
}

char majuscule(char lettre)
{
    if (lettre >= 65 and lettre <= 90)
        return lettre;
    if (lettre >= 97 and lettre <= 122)
        return lettre - 32;
    return '-';
}

string purifie(string mot)
{
    string Rep = "";
    for (char i : mot)
    {
        if (i >= 65 and i <= 90)
            Rep += i;
        if (i >= 97 and i <= 122)
            Rep += i - 32;
    }
    return Rep;
}

bool trouve(string mot, vector<string> dico, vector<long> BORNES, int SHIFT, int NBR)
{
    string Word = purifie(mot);
    if (Word == "")
    {
        return false;
    }
    int Idx = CalcIdx(Word, NBR);
    long Borne = BORNES[Idx];
    int BorneInf = (Borne & (int)(pow(2, SHIFT) - 1));
    int BorneSup = Borne >> SHIFT;

    for (int i = BorneInf; i < BorneSup; i++)
    {
        if (dico[i] == Word)
        {
            return true;
        }
    }

    return false;
}

// int main()
// {
// #define CHECK(test) \
//     if (!(test))    \
//     cerr << "Test failed in file " << __FILE__ << " line " << __LINE__ << ": " #test << endl
//     vector<string> Dico = importeDico("Guidé/DictionnairePurified.txt");
//     int SHIFT = ceil(log(Dico.size()) / log(2));
//     int NBR = 3;
//     vector<long> BORNES = CreateBorne(Dico, NBR, SHIFT);
//     CHECK(majuscule('a') == 'A');
//     CHECK(majuscule('b') == 'B');
//     CHECK(majuscule('D') == 'D');
//     CHECK(majuscule('^') == '-');

//     CHECK(purifie(" Ou i¹") == "OUI");
//     CHECK(purifie("NOM?") == "NOM");

//     CHECK(trouve("ZYTHUMS", Dico, BORNES, SHIFT, NBR));
//     CHECK(trouve("non", Dico, BORNES, SHIFT, NBR));
//     CHECK(trouve("ABACAS", Dico, BORNES, SHIFT, NBR));
//     CHECK(trouve("AAS", Dico, BORNES, SHIFT, NBR));
//     CHECK(trouve("AALENIEN", Dico, BORNES, SHIFT, NBR));
//     CHECK(trouve("ABACA", Dico, BORNES, SHIFT, NBR));
//     CHECK(trouve("REABONNAI", Dico, BORNES, SHIFT, NBR));
//     CHECK(trouve("REABONNA", Dico, BORNES, SHIFT, NBR));
//     CHECK(trouve("REABONNAS", Dico, BORNES, SHIFT, NBR));

//     CHECK(!trouve("ZZZ", Dico, BORNES, SHIFT, NBR));
//     CHECK(!trouve("???", Dico, BORNES, SHIFT, NBR));
//     CHECK(!trouve("Nomination", Dico, BORNES, SHIFT, NBR));

//     return 0;
// }
