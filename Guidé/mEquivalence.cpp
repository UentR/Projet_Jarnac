#include <vector>
#include <string>
#include <iostream>
#include <map>
using namespace std;

#include "mDictionnaire.cpp"

#define CHECK(test) \
    if (!(test))    \
    cerr << "Test failed in file " << __FILE__ << " line " << __LINE__ << ": " #test << endl

/*******************************************************
 *
 * M    M    OOO     TTTTT   EEEEE   U   U    RRRR
 * MM  MM   O   O      T     E       U   U    R   R
 * M MM M   O   O      T     EEE     U   U    RRR
 * M    M   O   O      T     E       U   U    R  R
 * M    M    OOO       T     EEEEE    UUU     R   R
 * */

map<char, int> Count(string Vrac)
{
    map<char, int> Rep = {};
    for (char i : Vrac)
    {
        if (Rep.contains(i))
        {
            Rep[i]++;
        }
        else
        {
            Rep[i] = 1;
        }
    }
    return Rep;
}

/********************************
 * Deux plateaux sont équivalents si ils ont la même représentation pour le joueur
 * c'est à dire leur vrac est le même à permutation près
 * et ils presentent les même mots dans le même ordre
 * entree: Deux tableaux de chaine de caractères de taille strictement supérieur à 0 dont le premier terme de chacun représente le vrac
 * et les termes suivant représentent des mots
 * deux entiers largeur et hauteur qui représentent respectivement la largeur de la représentation visuelle du tableau ainsi que la longueur maximale des mots à considérer
 * et la hauteur de la représentation visuelle du tableau ainsi que le nombre de mots à considerer.
 */
bool equivautA(vector<string> plateauEtalon, vector<string> plateauCompare, int largeur, int hauteur)
{
    if (!(plateauEtalon.size() == plateauCompare.size()))
    {
        return false;
    }
    if (!(Count(purifie(plateauEtalon[0])) == Count(purifie(plateauCompare[0]))))
    {
        return false;
    }
    for (int i = 1; i < plateauEtalon.size(); i++)
    {
        if (!(purifie(plateauEtalon[i]) == purifie(plateauCompare[i])))
        {
            return false;
        }
    }
    return true;
}

int main()
{
    cout << "testEquivalence" << endl;
    // tout est de bonne longueur et identique
    CHECK(equivautA({"lepidopteriste", "myrtil", "citron", "vulcain"}, {"lepidopteriste", "myrtil", "citron", "vulcain"}, 8, 9));

    CHECK(not equivautA({"lepidopteriste", "myrtil", "citron", "vulcain"}, {"chasseur", "louve", "ourse", "laie"}, 9, 8));

    // vrac melangee
    CHECK(equivautA({"abc", "epidopterologie", "edelweiss", "campanule"}, {"bac", "epidopterologie", "edelweiss", "campanule"}, 10, 4));

    CHECK(not equivautA({"abc", "epidopterologie", "edelweiss", "campanule"}, {"abcabc", "epidopterologie", "edelweiss", "campanule"}, 10, 4));

    // passage en majuscule
    CHECK(equivautA({"ABCDEFGHIJK", "RIEN"}, {"abcdefghijk", "rien"}, 5, 5));

    CHECK(equivautA({"ABCDEFGHIJK", "rien"}, {"ABCDEFGHIJK", "RIEN"}, 5, 5));

    // même mots valides dans le même ordre à la même position mais mots trop longs ignorés

    // CHECK(equivautA({"ABC", "epidopterologie", "edelweiss", "campanule"}, {"ABC", "lepidoptériste", "edelweiss", "campanule"}, 10, 4));

    // CHECK(not equivautA({"ABC", "epidopterologie", "campanule", "edelweiss"}, {"ABC", "lepidoptériste", "edelweiss", "campanule"}, 10, 4));

    // même mots valides dans le même ordre à la même position mais mots trop longs ignorés
    // CHECK(equivautA({"ABC", "", "edelweiss", "campanule"}, {"ABC", "lepidopteriste", "edelweiss", "campanule"}, 10, 4));

    // CHECK(not equivautA({"ABC", "edelweiss", "campanule"}, {"ABC", "lepidopteriste", "edelweiss", "campanule"}, 10, 4));
}