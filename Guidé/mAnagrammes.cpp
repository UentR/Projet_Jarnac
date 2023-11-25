// un bibliotheque pour jouer avec les anagrammes
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
using namespace std;

/**
 * retire une lettre
 * @param string vrac: une suite de lettres
 * @param char lettre: une lettre
 * @return string: '-' si la lettre n'apparaît dans `vrac` et sinon
 * `vrac` après avoir y retiré la première occurence de la lettre
 */
string retire(string mot, char lettre) {
    string Rep = "";
    char Current;
    int Count = 0;
    for (char Current : mot) {
        if (Current == lettre) {
            break;
        }
        Rep += Current;
        Count++;
    }
    if (Count == mot.length()) return "-";
    for (int i=Count+1; i<mot.size(); i++) {
        Current = mot[i];
        Rep += Current;
    }
    return Rep;
}

/**
 * retire un mot
 * @param string vrac: une suite de lettres
 * @param string mot: un mot
 * renvoie vrac après lui avoir retiré une occurence de chaque lettre du mot
 * et la chaîne "-" si l'opération est impossible.
 */
string retire(string vrac, string mot) {
    string Rep = vrac;
    for (char i : mot) {
        Rep = retire(Rep, i);
    }
    return Rep;
}

/**
 * est anagramme
 * @param string mot1: un mot
 * @param string mot2: un mot
 * renvoie vrai si les deux mots sont des anagrammes l'un de l'autre et faux sinon
*/
bool est_anagramme(string mot1, string mot2) {
    return retire(mot1, mot2) != "-";      
}


/**
 * calcule la liste d'anagramme d'un mot
 */
#include <set>
vector<string> listeDesAnagrammes(string mot) {
    set<string> Temp = {};
    
}


/**
 * calcul la liste des anagrammes d'un mot qui figurent dans le dico
 */
vector<string> listeDesAnagrammesDuDico(string mot, string dico) {
    vector<string> Rep = {};
    return Rep;
}



#define CHECK(test) if (!(test)) cerr << "Test failed in file " << __FILE__ << " line " << __LINE__ << ": " #test << endl


int main() {

    CHECK ( retire("bonjour", 'o') == "bnjour");

    CHECK ( retire("bonjour", 'p') == "-");


    CHECK ( est_anagramme("rien", "rien") );
    CHECK ( est_anagramme("nier", "rein") );

    //CHECK ( listeDesAnagrammesDuDico("rien","dictionnaire.txt") == {"rien", "nier","rein"} );
    //afficheListedeMots( listeDesAnagrammesDuDico("rien","dictionnaire.txt"));

    CHECK( retire("abcd", "bac")   == "d");
    CHECK( retire("ab", "bac")     == "-");
    CHECK( retire("abc", "bac")    == "");
    CHECK( retire("famille", "m") == "faille");
    CHECK( retire("garder", "regard") == "");
    CHECK( (retire("brossent", "sorbet") == "ns")
           or (retire("brossent", "sorbet") == "sn")
         ) ;

}

