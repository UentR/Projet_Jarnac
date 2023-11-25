#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

/**************************************
 *
 * DDDD    IIIII     CCC    OOO
 *  D  D     I      C      O   O
 *  D  D     I     C       O   O
 *  D  D     I      C      O   O
 * DDDD    IIIII     CCC    OOO
 *
 ***************************************/

/** Convertit en majuscule
 * @param char lettre
 * @return lettre en majuscule, ou '-' si lettre n'est pas une lettre dans 'a-z, A-Z'.
 *
 */
char majuscule(char lettre);

/** «Purifie» un mot en retirant espaces et caractères spéciaux
 *  et en le mettant en majuscules
 * @param string mot
 * @return string: le mot purifié
 */
string purifie(string mot);

/**importeDico
 * @param string l'adresse d'un fichier contenant une liste de mots correctes.
 * @return vector<string> un tableau de chaines de caracteres contenant les memes mots.
 **/

vector<string> importeDico(string dico);

void CalcIdx(int &Idx, int &Mult, std::string &First);

/** Teste si un mot est dans le dictionnaire
 *  Note: on comparera deux mots en testant si leurs purifications sont égales
 * @param string mot: le mot recherché
 * @param vector<string> dico: un dictionnaire sous forme de tableau
 * @return bool: vrai si le mot est dans le dictionnaire, faux sinon
 */
bool trouve(string mot, vector<string> dico);
