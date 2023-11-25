#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
using namespace std;

#define CHECK(test) if (!(test)) cerr << "Test failed in file " << __FILE__ << " line " << __LINE__ << ": " #test << endl

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
int calculPoints(vector<string> plateauJoueur, int nbMaxLettres, int nbFinalMots) {
    int res = 0;
    int Len;
    string Word;
    int Size = plateauJoueur.size();
    for (int i=1; i<min(Size, nbFinalMots+1); i++) {
        Word = plateauJoueur[i];
        Len = Word.length();
        if (Len <= nbMaxLettres && Len >= 3) {
            res += Len * Len;
        }
    }
    return res;
}



int main() {

    CHECK ( calculPoints( {"essai", "tentative", "mots", "affichable"},
                          8, 9) ==   16  );
    CHECK ( calculPoints( {"essai", "c", "est", "trop", "court"},
                          8, 9) ==   50  );
    CHECK ( calculPoints( {"essai", "tentative", "mots", "affichable"},
                          12, 6) ==   197 );
    CHECK ( calculPoints( {"essai", "recursivement", "enumerable"},
                          12, 6) ==   100 );
    CHECK ( calculPoints( {"essai", "tentative", "mots", "affichable"},
                          15, 3) ==   197 );
    CHECK ( calculPoints( {"essai", "tentative", "mots", "affichable"},
                          15, 1) ==   81  );
    CHECK ( calculPoints( {"abcdefghijklmnopqrstuvwxyz", "affichable"},
                          10, 8) ==   100 );
    CHECK ( calculPoints( {"abcdefghijklmnopqrstuvwxyz", "trop", "de", "mots"},
                          20, 2) ==   16  );
    CHECK ( calculPoints( {"abcdefghijklmnopqrstuvwxyz", "mots", "trop", "long", "oui"},
                          3, 10) ==   9   );
}

