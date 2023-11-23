#include <vector>
#include <string>
using namespace std;


/*******************************************************
 *
 * M    M    OOO    TTTTT   EEEEE   U   U    RRRR
 * MM  MM   O   O     T     E       U   U    R   R
 * M MM M   O   O     T     EEE     U   U    RRR
 * M    M   O   O     T     E       U   U    R  R
 * M    M    OOO      T     EEEEE    UUU     R   R
************************************/

/**************************
 * calculPoints
 * @param vector<string> plateauJoueur: un plateau de jeu
 * @return le nombre de points rapportés par l'ensemble des mots du plateau
*/

int calculPoints(vector<string> plateauJoueur, int nbMaxLettres,
                 int nbFinalMots);

void testCalculeLesPoints();
