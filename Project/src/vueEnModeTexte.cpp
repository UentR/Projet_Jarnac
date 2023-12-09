#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

// #include "vueEnModeTexte.hpp"
#include "mCalculPoints.hpp"
#include "mDictionnaire.hpp"

/*****************************************************
******************************************************
******************************************************
*       affichage en mode texte
*
*
*  ********  *******   *      *   *******  *******
*     *      *          *    *       *     *
*     *      ****        ****        *     ****
*     *      *          *    *       *     *
*     *      *******   *      *      *     *******
*
******************************************************
*/

int size(string s) {
  if ((int)s[0] != 27) {
    return s.length();
  } else {
    int Idx = 0;
    int Count = 0;
    while ((int)s[Idx] != 109) {
      Idx++;
    }
    Idx++;
    while ((int)s[Idx] != 27) {
      Count++;
      Idx++;
    }
    return Count;
  }
}

void afficheSymbolesRepete(int combien, string symboles) {
  for (int nbSymboles = 0; nbSymboles < combien; nbSymboles++) {
    cout << symboles;
  }
}

/**********************************
 * affichage grille de points
 */
void afficheGrillePoints(int nbMaxLettres, int largeurCase) {
  for (int carre = 1; carre <= nbMaxLettres; carre++) {
    cout << "|";
    if (carre < 3) {
      afficheSymbolesRepete(largeurCase - 1, " ");
    } else {
      afficheSymbolesRepete(largeurCase - 4, " ");
      int nbAffiche = carre * carre;
      if (nbAffiche < 10) {
        afficheSymbolesRepete(1, " ");
      }
      if (nbAffiche < 100) {
        afficheSymbolesRepete(1, " ");
      }
      cout << "\033[1;97m";
      cout << nbAffiche;
      cout << "\033[0m";
    }
  }
}

/**
 * affiche une ligne séparatrice en hauteur
 */
void afficheSeparateur(int nbMaxLettres, int largeurCase) {
  for (int carre = 1; carre <= nbMaxLettres; carre++) {
    cout << "|";
    afficheSymbolesRepete(largeurCase - 1, "_");
  }
}

/***
 * Affiche chaque lettre du mots dans les case de la bonne largeur
 */
void afficheMots(string mot, int nbMaxLettres, int largeurCase) {
  for (int lettre = 0; lettre < nbMaxLettres; lettre++) {
    cout << "|";
    afficheSymbolesRepete((largeurCase - 1) / 2, " ");
    if (lettre < mot.size()) {
      cout << majuscule(mot[lettre]);
    } else {
      cout << " ";
    }
    afficheSymbolesRepete(largeurCase - 2 - ((largeurCase - 1) / 2), " ");
  }
}

/***
 * efface les mots trop long
 */
string filtre(string mot, int nbMaxLettres) {
  string motFiltre = "";
  if (mot.size() <= nbMaxLettres) {
    motFiltre = mot;
  }
  return motFiltre;
}

/*******************************
 * affiche 2 plateaux de jeu de nbMots de nbMaxLettres maximun
 * ainsi que le titre du jeu et les prenom des joueurs. Les prenoms des joueurs
 * doivent être de longueur inférieur à 7 pour un affichage optimal.
 *
 */
// ajouter espace dans le vrac
void affichePlateaux(vector<string> plateauJoueurA,
                     vector<string> plateauJoueurB, int nbMots,
                     int nbMaxLettres, string titreJeu, string prenomA,
                     string prenomB, int Joueur, bool Jarnac) {
  system("clear");

  vector<string> Perso = {" ", " "};
  string End = "";
  string Titre = "";
  string SCORE = "";
  string VRAC = "";
  string NBR = "";
  string J = "";
  if (true) {
    J = "\033[91;1m";
    if (Jarnac) {
      J = "\033[36;4;1m";
    }
    Perso = {J, "\033[32;1m"};
    End = "\033[0m";
    prenomA = Perso[1 - Joueur] + prenomA + End;
    prenomB = Perso[Joueur] + prenomB + End;

    titreJeu = "\033[1;93m" + titreJeu + End;
    SCORE = "\033[1;35m";
    VRAC = "\033[1;96m";
    NBR = "\033[1;97m";
  }

  int largeurDecor = 16;
  int largeurCase = 4;
  if (nbMaxLettres * nbMaxLettres > 100) {
    largeurCase = 5;
  }
  // vector<string> grec = vector<string>{"alpha", "beta", "gamma", "delta",
  // "epsilon", "dzeta", "eta", "theta", "iota", "kappa", "lambda", "mu", "nu",
  // "xi", "omicron", "pi", "rho", "sigma", "tau", "upsilon", "phi", "khi",
  // "psi", "omega"};
  vector<string> grec =
      vector<string>{"1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",
                     "10", "11", "12", "13", "14", "15", "16", "17", "18",
                     "19", "20", "21", "22", "23", "24", "25", "26"};

  cout << endl;
  cout << endl;

  // affichage graphique de l'entete du jeu si 9 lettres
  // cout<< "
  // =======================================================================================
  // "<<endl; cout<< "|                                         titreJeu
  // |"<<endl; cout<<
  // "|=======================================================================================|"<<endl;
  // cout<< "|                                   |               | |"<<endl;
  // cout<< "| JoueurA"                          |               | |"<<endl;
  // cout<< "|                                   |               | |"<<endl;
  // cout<< "|===================================|
  // |===================================|"<<endl; cout<<
  // "|___________________________________|
  // |___________________________________|"<<endl; cout<< "|   |   | 9 | 16| 25|
  // 36| 49| 64| 81|               |   |   | 9 | 16| 25| 36| 49| 64| 81|"<<endl;
  // cout<< "|___|___|___|___|___|___|___|___|___|
  // |___|___|___|___|___|___|___|___|___|"<<endl;
  int largeurInterne = (2 * largeurCase * nbMaxLettres) + largeurDecor;
  // affichage logique de l'entete du jeu
  cout << "|";
  afficheSymbolesRepete(largeurInterne, "=");
  cout << "|";
  cout << endl;

  cout << "|";
  afficheSymbolesRepete((largeurInterne - size(titreJeu)) / 2, " ");
  afficheSymbolesRepete((largeurInterne - size(titreJeu)) % 2, " ");
  cout << titreJeu;
  afficheSymbolesRepete((largeurInterne - size(titreJeu)) / 2, " ");
  cout << "|";
  cout << endl;

  cout << "|";
  afficheSymbolesRepete(largeurInterne, "=");
  cout << "|";
  cout << endl;

  // entete des joueurs
  cout << "|";
  cout << prenomA;
  int nbAffiche = calculPoints(plateauJoueurA, nbMaxLettres, nbMots);
  afficheSymbolesRepete(
      largeurCase * nbMaxLettres - (size(prenomA) + size(" points ") + 4), " ");
  if (nbAffiche < 10) {
    cout << " ";
  }
  if (nbAffiche < 100) {
    cout << " ";
  }
  cout << SCORE;
  cout << nbAffiche << " point";

  if (nbAffiche > 1) {
    cout << "s";
  } else {
    cout << " ";
  }
  cout << End;
  cout << " |";
  afficheSymbolesRepete(largeurDecor, " ");
  cout << "|";

  cout << prenomB;
  nbAffiche = calculPoints(plateauJoueurB, nbMaxLettres, nbMots);
  afficheSymbolesRepete(
      largeurCase * nbMaxLettres - (size(prenomB) + size(" points ") + 4), " ");
  if (nbAffiche < 10) {
    cout << " ";
  }
  if (nbAffiche < 100) {
    cout << " ";
  }
  cout << SCORE;
  cout << nbAffiche << " point";

  if (nbAffiche > 1) {
    cout << "s";
  } else {
    cout << " ";
  }
  cout << End;
  cout << " |";
  cout << endl;

  cout << "|";
  afficheSymbolesRepete(largeurCase * nbMaxLettres - 1, "_");
  cout << "|";
  afficheSymbolesRepete(largeurDecor, " ");
  cout << "|";
  afficheSymbolesRepete(largeurCase * nbMaxLettres - 1, "_");
  cout << "|";
  cout << endl;

  afficheGrillePoints(nbMaxLettres, largeurCase);
  cout << "|";
  afficheSymbolesRepete(largeurDecor, " ");
  afficheGrillePoints(nbMaxLettres, largeurCase);
  cout << "|";
  cout << endl;

  afficheSeparateur(nbMaxLettres, largeurCase);
  cout << "|";

  for (int espace = 0; espace < largeurDecor; espace++) {
    cout << " ";
  }
  afficheSeparateur(nbMaxLettres, largeurCase);
  cout << "|";
  cout << endl;

  // affichage des mots
  string motA = " ";
  string motB = " ";
  for (int ligne = 1; ligne <= nbMots; ligne++) {
    if (ligne < plateauJoueurA.size()) {
      motA = filtre(plateauJoueurA[ligne], nbMaxLettres);
    } else {
      motA = "";
    }
    if (ligne < plateauJoueurB.size()) {
      motB = filtre(plateauJoueurB[ligne], nbMaxLettres);
    } else {
      motB = "";
    }
    afficheMots(motA, nbMaxLettres, largeurCase);
    cout << "|";

    afficheSymbolesRepete((largeurDecor - grec[ligne - 1].size()) / 2, " ");
    cout << NBR;
    cout << grec[ligne - 1];
    cout << End;
    afficheSymbolesRepete(largeurDecor - grec[ligne - 1].size() -
                              ((largeurDecor - grec[ligne - 1].size()) / 2),
                          " ");

    afficheMots(motB, nbMaxLettres, largeurCase);
    cout << "|";
    cout << endl;

    afficheSeparateur(nbMaxLettres, largeurCase);
    cout << "|";
    afficheSymbolesRepete(largeurDecor, " ");
    afficheSeparateur(nbMaxLettres, largeurCase);
    cout << "|";
    cout << endl;
  }
  cout << "|";
  afficheSymbolesRepete(largeurCase * nbMaxLettres - 1, " ");
  cout << "|";
  afficheSymbolesRepete(largeurDecor, " ");
  cout << "|";
  afficheSymbolesRepete(largeurCase * nbMaxLettres - 1, " ");
  cout << "|";
  cout << endl;

  // Ecrire le vrac sur la bonne largeur et peut etre plusieurs lignes;
  cout << "|";
  afficheSymbolesRepete(largeurCase * nbMaxLettres - 1, " ");
  cout << "|";
  afficheSymbolesRepete(largeurDecor, " ");
  cout << "|";
  afficheSymbolesRepete(largeurCase * nbMaxLettres - 1, " ");
  cout << "|";
  cout << endl;

  cout << "|";

  if (plateauJoueurA[0].size() < largeurCase * nbMaxLettres) {
    afficheSymbolesRepete(
        (largeurCase * nbMaxLettres - plateauJoueurA[0].size()) / 2, " ");
    cout << VRAC;
    cout << plateauJoueurA[0];
    cout << End;
    afficheSymbolesRepete(
        largeurCase * nbMaxLettres - plateauJoueurA[0].size() - 1 -
            (largeurCase * nbMaxLettres - plateauJoueurA[0].size()) / 2,
        " ");
  }
  cout << "|";
  afficheSymbolesRepete(largeurDecor, " ");
  cout << "|";

  if (plateauJoueurB[0].size() < largeurCase * nbMaxLettres) {
    afficheSymbolesRepete(
        (largeurCase * nbMaxLettres - plateauJoueurB[0].size()) / 2, " ");
    cout << VRAC;
    cout << plateauJoueurB[0];
    cout << End;
    afficheSymbolesRepete(
        largeurCase * nbMaxLettres - plateauJoueurB[0].size() - 1 -
            (largeurCase * nbMaxLettres - plateauJoueurB[0].size()) / 2,
        " ");
  }
  cout << "|";
  cout << endl;

  cout << "|";
  afficheSymbolesRepete(largeurCase * nbMaxLettres - 1, " ");
  cout << "|";
  afficheSymbolesRepete(largeurDecor, " ");
  cout << "|";
  afficheSymbolesRepete(largeurCase * nbMaxLettres - 1, " ");
  cout << "|";
  cout << endl;

  cout << "|";
  afficheSymbolesRepete(2 * (largeurCase * nbMaxLettres) + largeurDecor, "_");
  cout << "|";
  cout << endl;
}

void afficheLaDemandeDeReponseParmi(vector<string> possibilites,
                                    string joueur) {
  cout << "Au cours du jeu il est possible de taper certaines lettres pour "
          "piloter la manche."
       << endl;
  cout << "    A : Abandonner la Partie" << endl;
  cout << "    C : Continuer en choisissant un mot" << endl;
  cout << "    E : Echanger trois Lettres" << endl;
  cout << "    J : crier Jarnac" << endl;
  cout << "    L : Laisser l'ordinateur jouer à ma place" << endl;
  cout << "    P : Passer son tour" << endl;
  cout << "    T : Tirer une lettre" << endl;
  cout << "    V : Voir les coups possibles (aide de l'IA)" << endl;
  cout << joueur
       << " ------>Veuillez choisir une action parmis celle possibles:" << endl;
  for (string mot : possibilites) {
    cout << " " << mot;
  }
  cout << endl;
}

void afficheLaDemandeDeChoisir3lettres(string nomJoueur) {
  cout << nomJoueur
       << ", veuillez taper les trois lettres de votre vrac dont vous "
          "souhaitez vous débarasser:";
}

void afficheLaDemandeDeTaperUnMot(string nomJoueur) {
  cout << nomJoueur
       << ", veuillez taper votre nouveau mot ou seulement la lettre P pour "
          "passer votre tour:";
}

void afficheLaDemandeDePrenom(int joueur) {
  cout << "joueur " << joueur << " Veuillez taper votre prénom:";
}

void signaleMotNonValide(string mot) {
  cout << "... " << mot
       << " ... ----------> Ce mot entre les points de suspension est non "
          "valide "
       << endl;
}

void signaleGagnant(string nomJoueur) {
  cout << "Bravo, " << nomJoueur
       << " remporte la partie et a gagné un sourire! " << endl;
}

// Ecrire votre code ici
