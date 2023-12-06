#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <math.h>

using namespace std;

// struct ForDict
// {
//     vector<string> Mots;
//     vector<long> Bornes;
//     int NBR;
//     int SHIFT;
// };

#include "mDictionnaire.hpp"

struct ForDict;


void importeDico(string adresseDico, ForDict *DictHelper)
{
    
    ifstream File;
    File.open(adresseDico);
    string Word;
    int Nbr = 0;
    while (File >> Word and Nbr < pow(2, 18))
    {
        Nbr++;
        DictHelper->Mots.push_back(Word);
    }
    DictHelper->SHIFT = ceil(log(Nbr) / log(2));
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

void CreateBorne(ForDict *DictHelper)
{
    if (DictHelper->SHIFT > 32)
    {
        cerr << "Dictionnaire trop grand, overflow" << endl;
        exit(136);
    }
    double Size = pow(26, DictHelper->NBR);

    DictHelper->Bornes = vector<long>(Size, 0);

    string First = DictHelper->Mots[0].substr(0, DictHelper->NBR);
    long BorneInf = 0;
    long BorneSup = 0;
    int Idx;
    string Word;
    long Value;
    for (int i = 0; i < DictHelper->Mots.size(); i++) // parcours du dico
    {
        Word = DictHelper->Mots[i];                // lecture dico
        if (Word.rfind(First, 0) != 0) // rupture sur les NBR premiers caractères ?
        {                              // OUI
            Idx = CalcIdx(First, DictHelper->NBR);
            BorneSup = i;
            DictHelper->Bornes[Idx] = (BorneSup << DictHelper->SHIFT) + (BorneInf); // Se référer à la doc
            BorneInf = i;
            First = Word.substr(0, DictHelper->NBR);
        }
    }
    Idx = CalcIdx(First, DictHelper->NBR);
    BorneSup = DictHelper->Mots.size();
    DictHelper->Bornes[Idx] = (BorneSup << DictHelper->SHIFT) + (BorneInf); // Se référer à la doc
}

char majuscule(char lettre)
{
    if (lettre >= 65 and lettre <= 90)
        return lettre;
    if (lettre >= 97 and lettre <= 122)
        return lettre - 32;
    return '-';
}

string majuscule(string lettre)
{
    char Lettre = lettre[0];
    string Rep = "";
    Rep += majuscule(Lettre);
    return Rep;
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

bool trouve(string mot, ForDict *DictHelper)
{
    string Word = purifie(mot);
    if (Word == "")
    {
        return false;
    }
    int Idx = CalcIdx(Word, DictHelper->NBR);
    long Borne = DictHelper->Bornes[Idx];
    int BorneInf = (Borne & (int)(pow(2, DictHelper->SHIFT) - 1));
    int BorneSup = Borne >> DictHelper->SHIFT;

    for (int i = BorneInf; i < BorneSup; i++)
    {
        if (DictHelper->Mots[i] == Word)
        {
            return true;
        }
    }

    return false;
}

void CreateHelper(ForDict *DictHelper, string adresseDico, int NBR) {
    DictHelper->NBR = 3;
    importeDico(adresseDico, DictHelper);
    CreateBorne(DictHelper);
}

#define CHECK(test) \
    if (!(test))    \
    cerr << "Test failed in file " << __FILE__ << " line " << __LINE__ << ": " #test << endl

// int main()
// {
//     ForDict *DictHelper = new ForDict;
//     CreateHelper(DictHelper, "Text/DictionnairePurified.txt", 3);
//     // CHECK(majuscule('a') == 'A');
//     // CHECK(majuscule('b') == 'B');
//     // CHECK(majuscule('D') == 'D');
//     // CHECK(majuscule('^') == '-');

//     // CHECK(purifie(" Ou i¹") == "OUI");
//     // CHECK(purifie("NOM?") == "NOM");

//     CHECK(trouve("ZYTHUMS", DictHelper));
//     CHECK(trouve("non", DictHelper));
//     CHECK(trouve("ABACAS", DictHelper));
//     CHECK(trouve("AAS", DictHelper));
//     CHECK(trouve("AALENIEN", DictHelper));
//     CHECK(trouve("ABACA", DictHelper));
//     CHECK(trouve("REABONNAI", DictHelper));
//     CHECK(trouve("REABONNA", DictHelper));
//     CHECK(trouve("BITA", DictHelper));

//     CHECK(!trouve("ZZZ", DictHelper));
//     CHECK(!trouve("???", DictHelper));
//     CHECK(!trouve("Nomination", DictHelper));

//     return 0;
// }
