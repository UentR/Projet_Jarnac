#include <vector>
#include <string>
using namespace std;

int calculPoints(vector<string> plateauJoueur, int nbMaxLettres, int nbFinalMots)
{
    int res = 0;
    int Len;
    string Word;
    int Size = plateauJoueur.size();
    for (int i = 1; i < min(Size, nbFinalMots + 1); i++)
    {
        Word = plateauJoueur[i];
        Len = Word.length();
        if (Len <= nbMaxLettres && Len >= 3)
        {
            res += Len * Len;
        }
    }
    return res;
}
