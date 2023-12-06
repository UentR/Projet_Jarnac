// un bibliotheque pour jouer avec les anagrammes
#include <string>
#include <set>
#include <vector>

using namespace std;

// #include "mDictionnaire.hpp"

/**
 * retire une lettre
 * @param string vrac: une suite de lettres
 * @param char lettre: une lettre
 * @return string: '-' si la lettre n'apparaît dans `vrac` et sinon
 * `vrac` après avoir y retiré la première occurence de la lettre
 */
string retire(string mot, char lettre);

/**
 * retire un mot
 * @param string vrac: une suite de lettres
 * @param string mot: un mot
 * renvoie vrac après lui avoir retiré une occurence de chaque lettre du mot
 * et la chaîne "-" si l'opération est impossible.
 */
string retire(string vrac, string mot);

/**
 * est anagramme
 * @param string mot1: un mot
 * @param string mot2: un mot
 * renvoie vrai si les deux mots sont des anagrammes l'un de l'autre et faux sinon
 */
bool est_anagramme(string mot1, string mot2);

/**
 * calcule la liste d'anagramme d'un mot
 */
set<string> listeDesAnagrammes(string mot);

/**
 * calcul la liste des anagrammes d'un mot qui figurent dans le dico
 */
set<string> listeDesAnagrammesDuDico(string mot, vector<string> dico);
