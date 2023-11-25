#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <math.h>
using namespace std;

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
    int Nbr = 0;
    while (File >> Word and Nbr < pow(2, 18))
    {
        Nbr++;
        Dico.push_back(Word);
    }
    return Dico;
}

vector<string> Dico = importeDico("Guidé/DictionnairePurified.txt");
int SHIFT = ceil(log(Dico.size()) / log(2));
int NBR = 3;

/** Calcul l'indexe d'un couple de NBR lettre
 *  Note: Convertisseur de la base 26 de l'alphabet en base 10
 * @param string Word: le mot dont on cherche l'indexe
 * @return int Idx: l'indexe correspondant
 */
int CalcIdx(string Word)
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

/** Crée les bornes de chaque couple de NBR première lettres
 *  Note: l'explication du stockage de la valeur des bornes est dans "..."
 * @param vector<string> Dico: le dictionnaire a partionner
 * @return vector<long> Borne: la liste de toutes les bornes pour chaque couple
 */
vector<long> CreateBorne(vector<string> Dico)
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
            Idx = CalcIdx(First);
            BorneSup = i;
            Borne[Idx] = (BorneSup << SHIFT) + (BorneInf); // Se référer à la doc
            BorneInf = i;
            First = Word.substr(0, NBR);
        }
    }
    Idx = CalcIdx(First);
    BorneSup = Dico.size();
    Borne[Idx] = (BorneSup << SHIFT) + (BorneInf); // Se référer à la doc
    return Borne;
}

vector<long> BORNES = CreateBorne(Dico);

#define CHECK(test) \
    if (!(test))    \
    cerr << "Test failed in file " << __FILE__ << " line " << __LINE__ << ": " #test << endl

/** Convertit en majuscule
 * @param char lettre
 * @return lettre en majuscule, ou '-' si lettre n'est pas une lettre dans 'a-z, A-Z'.
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
    if (Word == "")
    {
        return false;
    }
    int Idx = CalcIdx(Word);
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
    CHECK(trouve("AALENIEN", Dico));
    CHECK(trouve("ABACA", Dico));
    CHECK(trouve("REABONNAI", Dico));
    CHECK(trouve("REABONNA", Dico));
    CHECK(trouve("REABONNAS", Dico));

    CHECK(!trouve("ZZZ", Dico));
    CHECK(!trouve("???", Dico));
    CHECK(!trouve("Nomination", Dico));

    return 0;
}
