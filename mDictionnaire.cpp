#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

#define CHECK(test) if (!(test)) cerr << "Test failed in file " << __FILE__ << " line " << __LINE__ << ": " #test << endl


/** Convertit en majuscule
 * @param char lettre
 * @return lettre en majuscule, ou '-' si lettre n'est pas une lettre dans 'a-z, A-Z'.
 *
*/
char majuscule(char lettre) {
    if (lettre >= 65 and lettre <= 90) return lettre;
    if (lettre >= 97 and lettre <= 122) return lettre - 32;
    return '-';
}


/** «Purifie» un mot en retirant espaces et caractères spéciaux
 *  et en le mettant en majuscules
 * @param string mot
 * @return string: le mot purifié
*/
string purifie(string mot) {
    string Rep = "";
    for (char i : mot) {
        if (i >= 65 and i <= 90) Rep += i;
        if (i >= 97 and i <= 122) Rep += i - 32;
    }
    return Rep;
}

/**importeDico
* @param string l'adresse d'un fichier contenant une liste de mots correctes.
* @return vector<string> un tableau de chaines de caracteres contenant les memes mots.
**/

vector<string> importeDico(string adresseDico) {
    ifstream File;
    File.open(adresseDico);
    string Word;
    vector<string> Dico = {};
    while (File >> Word) {
        Dico.push_back(Word);
    }
    return Dico;
}


/** Teste si un mot est dans le dictionnaire
 *  Note: on comparera deux mots en testant si leurs purifications sont égales
 * @param string mot: le mot recherché
 * @param vector<string> dico: un dictionnaire sous forme de tableau
 * @return bool: vrai si le mot est dans le dictionnaire, faux sinon
*/
bool trouve(string mot, vector<string> dico) {
    string Word = purifie(mot);
    for (auto i : dico) {
        if (i == Word) return true;
    }
    return false;
}



int main() {
    vector<string> Dico = {"TEST", "OUI", "NOM", "BONJOUR"};

    CHECK( majuscule('a') == 'A' );
    CHECK( majuscule('b') == 'B' );
    CHECK( majuscule('D') == 'D' );
    CHECK( majuscule('^') == '-' );

    CHECK( purifie(" Ou i¹") == "OUI");
    CHECK( purifie("NOM?") == "NOM");

    CHECK( trouve("ZYTHUMS", importeDico("dictionnaire.txt")) );
    CHECK( !trouve("non", Dico) );
    
    return 0;
}
