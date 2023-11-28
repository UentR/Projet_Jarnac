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

/** Crée les bornes de chaque couple de NBR première lettres
 *  Note: l'explication du stockage de la valeur des bornes est dans "..."
 * @param vector<string> Dico: le dictionnaire a partionner
 * @return vector<long> Borne: la liste de toutes les bornes pour chaque couple
 */
vector<long> CreateBorne(vector<string> Dico, int NBR, int SHIFT);

/** Calcul l'indexe d'un couple de NBR lettre
 *  Note: Convertisseur de la base 26 de l'alphabet en base 10
 * @param string Word: le mot dont on cherche l'indexe
 * @return int Idx: l'indexe correspondant
 */
int CalcIdx(string Word, int NBR);

/** Teste si un mot est dans le dictionnaire
 *  Note: on comparera deux mots en testant si leurs purifications sont égales
 * @param string mot: le mot recherché
 * @param vector<string> dico: un dictionnaire sous forme de tableau
 * @return bool: vrai si le mot est dans le dictionnaire, faux sinon
 */
bool trouve(string mot, vector<string> dico, vector<long> BORNES, int SHIFT, int NBR);
