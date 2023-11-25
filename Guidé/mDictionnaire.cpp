#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <math.h>
#include <fstream>
using namespace std;

int NBR = 2;

vector<long> CreateBorne(vector<string> Dico)
{
    vector<long> Borne = vector<long>(pow(26, NBR), 0);
    string First = Dico[0].substr(0, NBR);
    long BorneInf = 0;
    long BorneSup = 0;
    int Idx;
    string Word;
    for (int i = 0; i < Dico.size(); i++) // parcours du dico
    {
        Word = Dico[i];                // lecture dico
        if (Word.rfind(First, 0) != 0) // rupture sur les 2 premiers caractères ?
        {                              // OUI
            Idx = (First[0] - 65) * 26 + First[1] - 65;
            BorneSup = i;
            Borne[Idx] = (BorneSup << 19) + (BorneInf << 1) + 1;
            BorneInf = i;
            First = Word.substr(0, NBR);
        }
    }
    Idx = (First[0] - 65) * 26 + First[1] - 65;
    BorneSup = Dico.size() - 1;
    Borne[Idx] = (BorneSup << 19) + (BorneInf << 1) + 1;
    return Borne;
}

/**importeDico
 * @param string l'adresse d'un fichier contenant une liste de mots correctes.
 * @return vector<string> un tableau de chaines de caracteres contenant les memes mots.
 **/
vector<string> importeDico(string adresseDico)
{
    ifstream File;
    File.open(adresseDico);
    string Word;
    vector<string> Dico = {};
    while (File >> Word)
    {
        Dico.push_back(Word);
    }
    return Dico;
}

vector<string> Dico = importeDico("DictionnairePurified.txt");
vector<long> BORNES = CreateBorne(Dico);

#define CHECK(test) \
    if (!(test))    \
    cerr << "Test failed in file " << __FILE__ << " line " << __LINE__ << ": " #test << endl

/** Convertit en majuscule
 * @param char lettre
 * @return lettre en majuscule, ou '-' si lettre n'est pas une lettre dans 'a-z, A-Z'.
 *
 */
char majuscule(char lettre)
{
    if (lettre >= 65 and lettre <= 90)
        return lettre;
    if (lettre >= 97 and lettre <= 122)
        return lettre - 32;
    return '-';
}

/** «Purifie» un mot en retirant espaces et caractères spéciaux
 *  et en le mettant en majuscules
 * @param string mot
 * @return string: le mot purifié
 */
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

/** Teste si un mot est dans le dictionnaire
 *  Note: on comparera deux mots en testant si leurs purifications sont égales
 * @param string mot: le mot recherché
 * @param vector<string> dico: un dictionnaire sous forme de tableau
 * @return bool: vrai si le mot est dans le dictionnaire, faux sinon
 */
bool trouve(string mot, vector<string> dico)
{
    string Word = purifie(mot);
    int Idx = 0;
    int Mult = 1;
    for (int i = 0; i < NBR; i++)
    {
        Idx += (Word[NBR - i - 1] - 65) * Mult;
        Mult *= 26;
    }
    long Borne = BORNES[Idx];
    if (!(Borne % 2))
    {
        return false;
    }
    int BorneInf = ((Borne >> 1) & int(pow(2, 18) - 1));
    int BorneSup = Borne >> 19;

    for (int i = BorneInf; i <= BorneSup; i++)
    {
        if (dico[i] == Word)
        {
            return true;
        }
    }

    return false;
}

int main()
{
    CHECK(majuscule('a') == 'A');
    CHECK(majuscule('b') == 'B');
    CHECK(majuscule('D') == 'D');
    CHECK(majuscule('^') == '-');

    CHECK(purifie(" Ou i¹") == "OUI");
    CHECK(purifie("NOM?") == "NOM");

    CHECK(trouve("ZYTHUMS", Dico));
    CHECK(trouve("non", Dico));
    CHECK(trouve("ABACAS", Dico));
    CHECK(trouve("AAS", Dico));

    return 0;
}
