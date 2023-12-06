#include <stdexcept>
#include <iostream>
#include <vector>
using namespace std;

// #include "mDictionnaire.hpp"
/***Infrastructure minimale de test **/

#define CHECK(test) \
    if (!(test))    \
    cerr << "Test failed in file " << __FILE__ << " line " << __LINE__ << ": " #test << endl

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

/*******************************
 * affiche 2 plateaux de jeu de nbMots de nbMaxLettres maximun
 * ainsi que le titre du jeu et les prenom des joueurs.
 * Les prenoms des joueurs doivent être de longueur inférieur à 7
 * pour un affichage optimal.
 *
 */
void affichePlateaux(vector<string> plateauJoueurA,
                     vector<string> plateauJoueurB, int nbMots, int nbMaxLettres, string titreJeu,
                     string prenomA, string prenomB, int Joueur, bool Jarnac);

/**
 *
 */
void afficheLaDemandeDePrenom(int numJoueur);

/**
 *
 */
void afficheLaDemandeDeReponseParmi(vector<string> possibilites,
                                    string NomJoueur);

/**
 *
 */
void afficheLaDemandeDeReponseParmi(string possibilites, string NomJoueur);

/**
 *
 */
void afficheLaDemandeDeChoisir3lettres(string nomJoueur);

/**
 *
 */
void afficheLaDemandeDeTaperUnMot(string nomJoueur);

/**
 *
 */
void signaleMotNonValide(string mot);

/**
 *
 */
void signaleGagnant(string nomJoueur);

/// BEGIN SOLUTION
/** affiche plusieurs fois les mêmes symboles à l'écran
 * @param combien : le nb de fois
 * @param symboles : les symboles a repeter pluieurs fois
 */
void afficheSymbolesRepete(int combien, string symboles);

/**********************************
 * affichage grille de points
 * fabrique le décor des nombres de points
 */
void afficheGrillePoints(int nbMaxLettres, int largeurCase);

/**
 * affiche une ligne séparatrice en hauteur
 */
void afficheSeparateur(int nbMaxLettres, int largeurCase);

/***
 * Affiche chaque lettre du mots dans les case de la bonne largeur
 */
void afficheMots(string mot, int nbMaxLettres, int largeurCase);

/***
 * efface les mots trop long
 */
string filtre(string mot, int nbMaxLettres);

/// END SOLUTION

/// BEGIN SOLUTION
vector<int> traduitEnEntier(vector<string> liste);
vector<string> traduitEnLettreGrecque(vector<int> origine);
/// END SOLUTION
