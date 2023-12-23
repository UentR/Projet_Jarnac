#include <string.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <tuple>
#include <vector>

#include "Debug.hpp"
// #include "loadIA.hpp"
#include "mCalculPoints.hpp"
#include "mDictionnaire.hpp"
#include "vueEnModeTexte.hpp"

using namespace std;

using BOARD = vector<vector<string>>;

// Debut definition des constantes
#define NB_LIGNES_PLATEAU 8
#define TAILLE_MAX_MOT 9
#define NBJOUEUR 2
#define TAILLE_VRAC 6
#define TAILLES_CLEES_DICO 3
#define NB_LETTRES_ECHANGE 3
// Fin definition des constantes

/**
 * @brief Structure de données pour les noms à stocker
 *
 * @param string Name1 - Nom du joueur 1
 * @param string Name2 - Nom du joueur 2
 * @param string NameGame - Nom du jeu
 */
struct Names {
  string Name1;
  string Name2;
  string NameGame;
};

/**
 * @brief Structure de données pour un mot à jouer
 *
 * @param bool isAI[2] - Si les joueurs sont des IA
 * @param AI* AIS[2] - Les IA à charger
 * @param bool* AIPlay[2] - Si les IA doivent jouer
 * @param Play* IAMove - Le mot à jouer pour l'IA
 */

struct Play {
  string Word;
  string DLetter;
  int Ligne;
  int Origin;
  bool Jarnac;
  bool End;
};

struct StorePlayers {
  bool isAI[2] = {false, false};
  // AI *AIS[2];

  bool AIPlay[2];
  bool AIExchange[2];
  Play *IAMove;
  char *AILetters;
  bool AIJarnac;
};

struct StoreLetters {
  vector<string> Letters;
  int Len;
};

struct Keys {
  int keyPlay[2];
  int keyExchange[2];
  int keyJarnac;
  int keyLetters;
  int keyPlayMove;
  int keyReady[2];
  int keyBoard;
  int keyVrac[2];
};

struct Adresses {
  bool *CanPlay[2];
  bool *CanExchange[2];
  bool *CanJarnac;
  char *ExchangeLetters;
  Play *Move;
  bool *Ready[2];
  char *Board;
  char *Vrac[2];
};

enum TentativeMotPlace {
  // Etat du mot joué
  EchecPlacementMot,
  ReussitePlacementMot,
  FinTourIA,
};

enum ChoixPossible {
  // Etat du choix de l'action
  PlacerUnMot = 'C',
  TentativeJarnac = 'J',
  FinTour = 'F',
  VoirPlateau = 'P',
  VoirMenu = 'M',
  Abandonner = 'A',
};

/**
 * @brief Trie une chaîne de caractères.
 * @param string Word - La chaîne de caractères à trier.
 * @return string La chaîne de caractères triée.
 */
string Sort(string Mot);

/**
 * @brief Retourne la ligne sur laquelle se trouve un mot.
 * @param BOARD Board - Le plateau de jeu.
 * @param string Word - Le mot à rechercher.
 * @param int Joueur - Le joueur qui joue.
 * @return int La ligne sur laquelle se trouve le mot.
 */
int getLine(BOARD Board, string Word, int Joueur);

/**
 * @brief Renvoie si une lettre est une voyelle ou non
 *
 * @param string Letter - Lettre à vérifier
 * @return bool - Si la lettre est une voyelle
 */
bool isVowel(string Letter);

/**
 * @brief Permet de vérifier s'il y a une voyelle dans la pioche d'un joueur
 * entre deux bornes
 *
 * @param vector<string> vectorLetter - Lettres à vérifier
 * @param int Start - Début des bornes
 * @param int End - Fin des bornes
 * @return bool - S'il y a une voyelle entre les bornes
 */
bool CheckVowel(vector<string> vectorLetter, int Start, int End);

/**
 * @brief Initialisation d'un vecteur avec toutes les lettres disponibles
 * avec le bon nombre d'occurrence
 *
 * @return vector<string> - Sac de lettres
 */
vector<string> createLetters();

/**
 * @brief Permet de mélanger le sac de lettres
 *
 * @param StoreLetters* LETTERS - Contenu du sac de lettre
 * @param string Rep - Lettres à remettre dans le sac lors d'un échange
 */
void shuffleBag(StoreLetters *LETTERS, string Rep);

/**
 * @brief Permet de piocher une lettre dans le sac
 *
 * @param StoreLetters* LETTERS - Contenu du sac de lettre
 * @return string - Lettre piochée
 */
string piocheLettre(StoreLetters *LETTERS);

/**
 * @brief Permet d'afficher l'ensemble des actions possibles pour un joueur
 */
void ActionPossible();

/**
 * @brief Permet de choisir 3 lettres à échanger
 *
 * @param BOARD Board - Plateau de jeu
 * @param int Joueur - Joueur actuel
 * @param StoreLetters* Letters - Contenu du sac de lettre
 * @return tuple<BOARD,bool> - Plateau de jeu avec les lettres échangées et si
 * l'échange a été effectué
 */
tuple<BOARD, bool> echangeLettre(BOARD Board, int Joueur,
                                 StoreLetters *Letters);

/**
 * @brief Permet à un humain de choisir entre piocher une lettre ou échanger des
 * lettres
 *
 * @param BOARD Board - Plateau de jeu
 * @param int Joueur - Joueur actuel
 * @param StoreLetters* Letters - Contenu du sac de lettre
 * @return BOARD - Plateau de jeu avec la lettre piochée/les lettres échangées
 */
BOARD piocheOuEchange(BOARD Board, int Joueur, StoreLetters *Letters);

/**
 * @brief Permet de jouer un mot en suivant la structure Play
 *
 * @param BOARD Board - Plateau de jeu
 * @param int Joueur - Joueur actuel
 * @param Play* Current - Structure de données du mot à jouer
 * @param Names* NamesHelper - Noms des joueurs
 * @param ForDict* DictHelper - Structure de données du dictionnaire
 * @param StoreLetters* Letters - Contenu du sac de lettre
 * @return tuple<BOARD,TentativeMotPlace> - Plateau de jeu et état du mot joué
 */
tuple<BOARD, TentativeMotPlace> JouerMot(BOARD Board, int Joueur, Play *Current,
                                         Names *NamesHelper,
                                         ForDict *DictHelper,
                                         StoreLetters *Letters);

/**
 * @brief Permet à un humain de choisir un mot à jouer en suivant la structure
 * Play
 *
 * @param BOARD Board - Plateau de jeu
 * @param int Joueur - Joueur actuel
 * @param ForDict* DictHelper - Structure de données du dictionnaire
 * @param bool isJarnac - Si le jeu est en mode Jarnac
 * @param Names* NamesHelper - Noms des joueurs
 * @param StoreLetters* Letters - Contenu du sac de lettre
 * @return BOARD - Le plateau de jeu avec le mot joué
 */
BOARD PlaceMot(BOARD Board, int Joueur, ForDict *DictHelper, bool isJarnac,
               Names *NamesHelper, StoreLetters *Letters);

/**
 * @brief Permet à un humain de choisir une action
 *
 * @param BOARD Board - Plateau de jeu
 * @param int Joueur - Joueur actuel
 * @param ForDict* DictHelper - Structure de données du dictionnaire
 * @param Names* NamesHelper - Noms des joueurs
 * @param int Tour - Tour actuel
 * @param StoreLetters* Letters - Contenu du sac de lettre
 * @return tuple<BOARD,string> - Plateau de jeu et état du jeu
 */
tuple<BOARD, string> choixAction(BOARD Board, int Joueur, ForDict *DictHelper,
                                 Names *NamesHelper, int Tour,
                                 StoreLetters *Letters);

/**
 * @brief Permet de jouer un tour, que ce soit un tour d'IA ou de joueur
 *
 * @param BOARD Board - Plateau de jeu
 * @param int Joueur - Joueur actuel
 * @param ForDict* DictHelper - Structure de données du dictionnaire
 * @param Names* NamesHelper - Noms des joueurs
 * @param StorePlayers* PlayerHelper - Structure de données pour l'IA
 * @param int Tour - Tour actuel
 * @param StoreLetters* Letters - Contenu du sac de lettre
 * @return tuple<BOARD,string> - Plateau de jeu et état du jeu
 */
tuple<BOARD, string> Round(BOARD Board, int Joueur, ForDict *DictHelper,
                           Names *NamesHelper, StorePlayers *PlayerHelper,
                           int Tour, StoreLetters *Letters);

/**
 * @brief Check si la partie doit se terminer
 *
 * @param BOARD Board - Plateau de jeu
 * @param int Joueur - Joueur actuel
 * @return bool - Si la partie doit se terminer
 */
bool endGame(BOARD Board, int Joueur);

/**
 * @brief Permet de créer le plateau de jeu
 *
 * @param StoreLetters* Letters - Contenu du sac de lettre
 * @param int W - Largeur du plateau
 * @param int H - Hauteur du plateau
 * @return BOARD - Plateau de jeu
 */
BOARD initBoard(StoreLetters *Letters, int W, int H);

/**
 * @brief Permet de choisir le dictionnaire
 *
 * @return string - Nom du dictionnaire
 */
string choisirDictionnaire();

/**
 * @brief Permet de commencer le jeu
 *
 * @param string joueur0 - Nom du joueur 1
 * @param string joueur1 - Nom du joueur 2
 * @param string Name - Nom du jeu
 * @param bool IA1 - Si le joueur 1 est une IA
 * @param bool IA2 - Si le joueur 2 est une IA
 * @return true
 */
bool lanceLeJeu(string joueur0, string joueur1, string Name, bool IA1,
                bool IA2);
