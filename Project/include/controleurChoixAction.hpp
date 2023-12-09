#include <iostream>
#include <string>
#include <vector>

using namespace std;

// #include "vueEnModeTexte.hpp"
// #include "mPoseMotJarnac-correction.hpp"

/***********************************************************************
 *
 *  CCC     OOO   N   N  TTTTT RRRR    OOO   L      EEEEE  U    U  RRRR
 * C       O   O  NN  N    T   R   R  O   O  L      E      U    U  R   R
 * C       O   O  N N N    T   RRRR   O   O  L      EEEE   U    U  RRRR
 * C       O   O  N  NN    T   R  R   O   O  L      E      U    U  R  R
 *  CCC     OOO   N   N    T   R   R   OOO   LLLLL  EEEEE   UUUU   R   R
 *
 * **********************************************************************/

/// BEGIN SOLUTION

string choisirDictionnaire();

string piocher6lettres();

/// END SOLUTION

/************************
 * choisit une action parmi les actions possibles proposées dans possibilités
 * @param possibilites une liste de chaine de caractères
 * @param nomJoueur
 * @return une des chaines de caractères de possibilités,
 * laquelle aura été choisie par définition par le joueur nommé nomJoueur.
 **/
string choisirAction(vector<string> possibilites, string nomJoueur);

/************************
 * choisit un mot parmis les mots du diconnaire dans le fichier dico
 * @param plateau un plateau de jeu (voir définition:
 * le premier mot represente des lettres en vrac, les suivants des mots.)
 * @param nomJoueur le nom du joueur concerné par le choix
 * @param l'adresse du dictionnaire à consulter
 * @return la chaine de caractère "-" si le joueur (n'a pas voulu choisir)
 * ou un des mots du dictionnaire
 * lequel aura été choisi en principe par le joueur nommé nomJoueur.
 **/
string choisirMot(vector<string> plateau, string nomJoueur, string dico);

/*****************
 * Permet à deux joueurs de s'affronter
 * au Jarnac de l'installation à la notification du gagnant,
 * ils auront au cour du jeu à choisir
 *  - leur dictionnaire commun
 *  - leur interface et leur mode de jeu
 *  - leur actions et leurs coups respectifs
 * @param les prenoms de chacun deux joueurs
 * @retour vrai si le jeu s'est terminé normalement
 * avec un vainqueur (supériorité en nombre de points lors de la pose du 8eme mot)
 * et un perdant notifiés
 * (en cas d'égalité au points
 * c'est le joueur qui a fini
 * le tour qui gagne
 * de ce fait il y a toujours
 * un gagnant qui est notifé lors que le jeu va jusqu'au bout),
 * faux si le jeu a fait l'objet d'un abandon.
 */
bool lanceLeJeu(string joueur0, string joueur1);
