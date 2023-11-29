#include <vector>
#include <string>
using namespace std;


#include "mAnagrammes.hpp"
#include "mDictionnaire.hpp"




/*******************************************************
 *
 * M    M    OOO    TTTTT   EEEEE   U   U    RRRR
 * MM  MM   O   O     T     E       U   U    R   R
 * M MM M   O   O     T     EEE     U   U    RRR
 * M    M   O   O     T     E       U   U    R  R
 * M    M    OOO      T     EEEEE    UUU     R   R
 ***/

/********************************
 * Deux plateaux sont équivalents si ils ont la même représentation pour le joueur
 * c'est à dire leur vrac est le même à permutation près
 * et ils presentent les même mots dans le même ordre
 * entree: Deux tableaux de chaine de caractères de taille strictement supérieur à 0 dont le premier terme de chacun représente le vrac
 * et les termes suivant représentent des mots
 * deux entiers largeur et hauteur qui représentent respectivement la largeur de la représentation visuelle du tableau ainsi que la longueur maximale des mots à considérer
 * et la hauteur de la représentation visuelle du tableau ainsi que le nombre de mots à considerer.
 */
bool equivautA(vector<string> plateauEtalon,
               vector<string> plateauCompare,
               int largeur, int hauteur);
