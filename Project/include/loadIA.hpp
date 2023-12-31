#include <string.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <string>
#include <tuple>

#include "Debug.hpp"
#include "mDictionnaire.hpp"

using namespace std;

struct BOARD {
  char Board[160];
  char Vracs[2][145] = {"", ""};
};
/**
 * @brief Structure de données contenant les informations d'un noeud d'arbre.
 * @param string Ana - Le mot correspondant au noeud.
 * @param map<char,Node*> Children - Les noeuds enfants du noeud.
 */
struct Node {
  char *Ana;
  map<char, Node *> Children;
};

/**
 * @brief Structure de données contenant des informations pour l'IA.
 * @param map<string,string> Dict - Un dictionnaire contenant des mots triés
 *  en entrée et les mots correspondant en sortie.
 * @param map<string,Node*> NodeDict - Un dictionnaire contenant des mots en
 * entrée et les noeuds correspondant en sortie.
 */
struct AI {
  map<char *, char *> Dict;
  map<char *, Node *> NodeDict;
};

/**
 * @brief Structure de données contenant les informations d'un coup à jouer.
 * @param string Word - Le mot à jouer.
 * @param string DLetter - La lettre à utiliser.
 * @param int Ligne - La ligne sur laquelle jouer.
 * @param int Origin - L'origine du Jarnac.
 * @param bool Jarnac - Si le mode Jarnac est activé.
 * @param bool End - Si le coup est le dernier.
 */
struct Play {
  string Word;
  string DLetter;
  int Ligne;
  int Origin;
  bool Jarnac;
  bool End;
};

/**
 * @brief Trie une chaîne de caractères.
 * @param string Word - La chaîne de caractères à trier.
 * @return string La chaîne de caractères triée.
 */
string Sort(string Mot);

/**
 * @brief retire une lettre
 *
 * @param string vrac: une suite de lettres
 * @param char lettre: une lettre
 * @return string: '-' si la lettre n'apparaît dans `vrac` et sinon
 * `vrac` après avoir y retiré la première occurrence de la lettre
 */
string retire(string mot, char lettre);

/**
 * @brief retire un mot
 *
 * @param string vrac: une suite de lettres
 * @param string mot: un mot
 * @return string après lui avoir retiré une occurrence de chaque lettre du mot
 * et la chaîne "-" si l'opération est impossible.
 */
string retire(string vrac, string mot);

/**
 * @brief Retire une chaîne de caractères d'une chaîne de caractères.
 * @param string Word - La chaîne de caractères à modifier.
 * @param string Duplicate - La chaîne à retirer.
 * @return string La chaîne de caractères modifiée.
 */
string Retire(string Word, string Duplicate);

/**
 * @brief Génère toutes les permutations possibles d'une chaîne de
 * caractères.
 * @param string Vrac - La chaîne de caractères à permuter.
 * @param int i - La longueur des permutations à générer.
 * @return vector<string> Un vecteur contenant toutes les permutations
 * possibles.
 */
set<char *> DictPermutations(char *Vrac, int Length);

/**
 * @brief Recherche des mots dans un dictionnaire.
 * @param string Vrac - La chaîne de caractères à rechercher.
 * @param map<string,string> Words - Le dictionnaire dans lequel rechercher.
 * @param bool Jarnac - Si le mode Jarnac est activé.
 * @return set<string> Un ensemble de mots trouvés.
 */
tuple<Node *, char *> Analyze(char *Vrac, vector<Node *> PlayerWords);

/**
 * @brief Analyse une chaîne de caractères et retourne le mot le plus long
 * possible ainsi que la lettre double qui le compose.
 * @param string Vrac - L'ensemble de lettre qu'il est possible d'ajouter.
 * @param vector<Node*> PlayerWords - Les mots déjà présents dans le jeu.
 * @return tuple<Node*,string> Un tuple contenant le noeud de l'arbre
 * correspondant au mot qui permet d'ajouter le plus de lettre et la suite
 * de lettres que l'on peut ajouter.
 */
tuple<Node *, string> Analyze(string Vrac, vector<Node *> PlayerWords);

/**
 * @brief Laisse l'IA choisir si elle veut piocher ou échanger 3 lettres
 * @param BOARD Board - Le plateau de jeu.
 * @param int Joueur - Le joueur qui joue.
 * @return true
 */
string PiocheEchange(BOARD Board, int Joueur);

/**
 * @brief Retourne la ligne sur laquelle se trouve un mot.
 * @param BOARD Board - Le plateau de jeu.
 * @param string Word - Le mot à rechercher.
 * @param int Joueur - Le joueur qui joue.
 * @return int La ligne sur laquelle se trouve le mot.
 */
int getLine(char *Board, char *Word, int Joueur);

/**
 * @brief Retourne le meilleur coup possible à jouer.
 * @param BOARD Board - Le plateau de jeu.
 * @param int Joueur - Le joueur qui joue.
 * @param bool Jarnac - Si le mode Jarnac est activé.
 * @param AI* AIHelper - L'IA qui aide à trouver le meilleur coup.
 * @return Play* Le meilleur coup possible.
 */
Play *BestMove(BOARD *Board, int Joueur, bool Jarnac, AI *AIHelper);

/**
 * @brief Charge un dictionnaire à partir d'un fichier. Chaque mot du fichier
 * est trié et ajouté au dictionnaire avec la version triée du mot comme clé et
 * le mot lui-même comme valeur.
 * @param AI* AIHelper - L'IA dans laquelle charger le dictionnaire.
 * @param string FileName - Le nom du fichier à charger.
 */
void LoadDict(AI *AIHelper, string FileName);

/**
 * @brief Charge une structure de données d'arbre à partir d'un fichier texte.
 * @param AI* AIHelper - L'IA dans laquelle charger l'arbre.
 * @param string FileName - Le nom du fichier à charger.
 */
void LoadTree(AI *AIHelper, string FileName);

/**
 * @brief Charge un dictionnaire et une structure de données d'arbre à partir de
 * fichiers texte.
 * @param AI* AIHelper - L'IA dans laquelle charger le dictionnaire et
 * l'arbre.
 */
void StartUpAI(AI *AIHelper);
