#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

#include "vueEnModeTexte.hpp"


// attention : aucune lettre accentuée dans les chaines de caractères car sinon la fonction .size() n'est plus cohérente avec l'affichage
// D'ou l'importance d'utiliser la fonction .length sur les chaines de caractères...

int main() {

    cout << "________________________ Illustration sur l equivalence des plateaux  _______________________________"
         << endl;
    cout << "======================>Les  equivalents" << endl;

    affichePlateaux( {"lepidopteriste", "myrtil", "citron", "vulcain"},
    {"lepidopteriste", "myrtil", "citron", "vulcain"},
    9, 8, "Illustration : plateaux equivalents car identiques", "Joueur A",
    "Joueur B"
                   );


    affichePlateaux( {"abc", "epidopterologie", "edelweiss", "campanule"},
    {"bac", "epidopterologie", "edelweiss", "campanule"},
    9, 6, "Illustration permutation du vrac : plateaux equivalents", "Joueur A",
    "Joueur B"
                   );

    affichePlateaux(  {"ABCDEFGHIJK", "RIEN"},
    {"abcdefghijk", "rien"},
    6, 6, "Illustration vrac en majuscule : plateaux equivalents", "Joueur A",
    "Joueur B"

                   );

    affichePlateaux( {"ABC", "epidopterologie", "edelweiss", "campanule"},
    {"ABC", "lepidoptériste",  "edelweiss", "campanule"},
    2, 10, "Illustration premier mot ignore : plateaux equivalents", "Joueur A",
    "Joueur B"
                   );


    affichePlateaux(       {"ABC", "epidopterologie", "campanule", "edelweiss"},
    {"ABC", "lepidopteriste",  "edelweiss", "campanule"},
    4, 7, "Illustration tous mots ignores : plateaux equivalents", "Joueur A",
    "Joueur B"
                   );

    affichePlateaux(   {"ABC", "", "edelweiss", "campanule"},
    {"ABC", "lepidopteriste",  "edelweiss", "campanule"},
    3, 10, "Illustration mot vide : plateaux equivalents", "Joueur A", "Joueur B"
                   );

    affichePlateaux(    { "regard", "arbre" },
    { "garder", "arbre", "racine" },
    1, 6, "Illustration trop de mots : plateaux equivalents", "Joueur A",
    "Joueur B"
                   );

    affichePlateaux(    { "regard", "chene", "bouleau", "pin", "racine", "gland" },
    { "regard", "chene", "bouleau", "pin", "regal", "ecureuil"},
    3, 10, "Illustration beaucoup trop de mots : plateaux equivalents",
    "Joueur A", "Joueur B"
                   );

    affichePlateaux(     { "ABCDeFG", "rien" },
    { "ABCDEFG", "RIEN" },
    1, 6, "Illustration minuscule : plateaux equivalents", "Joueur A", "Joueur B"
                   ) ;


    affichePlateaux(     { "ABCDEFG", "rien" },
    { "ABCDEFG", "RIEN" },
    2, 6, "Illustration mot en majuscule : plateaux equivalents", "Joueur A",
    "Joueur B"
                   ) ;


    cout << "======================>Les NON equivalents" << endl;

    cout << "mot de char 32";
    affichePlateaux( {"MOT"},  {"", "MOT", "   "}, 2, 6,
                     "Illustration : plateaux non equivalents", "Joueur A", "Joueur B");

    cout << "à venir";
    affichePlateaux(   { "regarder",   "regal" },
    { "regard", "regaler" },
    6, 8, "Illustration : plateaux non equivalents", "Joueur A", "Joueur B"
                   );


    cout << "vrac trop long";
    affichePlateaux(   { "regard",   "regal" },
    { "regarder", "regal" },
    6, 8, "Illustration : plateaux non equivalents", "Joueur A", "Joueur B"
                   );

    cout << "vrac trop long bis";
    affichePlateaux(   { "regard",   "regal" },
    { "regarder", "regal" },
    8, 6, "Illustration : plateaux non equivalents", "Joueur A", "Joueur B"
                   );

    cout << "anagramme";
    affichePlateaux(    { "regard", "regard" },
    { "regard", "garder" },
    1, 6, "Illustration : plateaux non equivalents", "Joueur A", "Joueur B"
                   );

    cout << "mot intercalaire";
    affichePlateaux(  {"ABC", "edelweiss", "campanule"},
    {"ABC", "edelweiss", "lepidopteriste",  "campanule"},
    6, 10, "Illustration : plateaux non equivalents", "Joueur A", "Joueur B"
                   );

    cout << "tous différents";
    affichePlateaux(    {"lepidopteriste", "myrtil", "citron", "vulcain"},
    {"chasseur",      "louve", "ourse", "laie"},
    5, 8, "Illustration : plateaux non equivalents", "Joueur A", "Joueur B"
                   );

    cout << "vrac trop long ter";
    affichePlateaux(    {"abc", "epidopterologie", "edelweiss", "campanule"},
    {"abcabc", "epidopterologie", "edelweiss", "campanule"},
    3, 6, "Illustration : plateaux non equivalents ", "Joueur A", "Joueur B"

                   );

    cout << "Vrac non vide, vide";
    affichePlateaux( {"MOT"},  {"", "MOT", "BONS"}, 5, 6,
                     "Illustration : plateaux non equivalents", "Joueur A", "Joueur B");

    cout << "________________________ fin de l'illustration _______________________________"
         << endl;
    cout << endl;

    cout << "*************************** DEBUT DU PROGRAMMME *******************"
         << endl << endl;
    affichePlateaux( {"rien"}, {"CouCou", "Les", "Poteaux"}, 4, 12,
                     "illsutration : aspect du jeu", "Ali", "Baba");

    cout << endl <<
         "======> Exemple de deux plateaux avec le même nombre de points. " << endl;
    affichePlateaux( {"essai", "tentative", "mots", "affichable"}, {"tte", "marcheras", "bien", "mien", "mien", "tien", "lien", "finale"},
                     15, 11, "ilustration : nombre de points identique", "Casimodo", "Dartagnan" );

    cout << endl;
    affichePlateaux ({"essai", "Jeu", "mots", "affichable", "Normal"}, {"tte", "marcheras", "bien", "mien", "mien", "tien", "lien", "finale"},
                     9, 8, "JARNAC", "Casimodo", "Dartagnan" );


}














