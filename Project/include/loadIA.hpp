#include <map>
#include <set>
#include <string>
#include <vector>

#include "mAnagrammes.hpp"

using namespace std;

using BOARD = vector<vector<string> >;

// Structure de données d'arbre
struct Node {
  string Ana;
  map<char, Node *> Children;
};

struct AI {
  map<string, string> Dict;
  map<string, Node *> NodeDict;
};

struct Play {
  string Word;
  string DLetter;
  int Ligne;
  int Origin;
  bool Jarnac;
  bool End;
};

/**
 * Génère toutes les permutations possibles d'une chaîne de caractères.
 * @param {string} Vrac - La chaîne de caractères à permuter.
 * @param {int} i - La longueur des permutations à générer.
 * @return {vector<string>} Un vecteur contenant toutes les permutations
 * possibles.
 */
set<string> DictPermutations(string Vrac, int i);

/**
 * Trie une chaîne de caractères.
 * @param {string} Word - La chaîne de caractères à trier.
 * @return {string} La chaîne de caractères triée.
 */
string Sort(string Word);

/**
 * Recherche des mots dans un dictionnaire.
 * @param {map<string, string>} Words - Le dictionnaire dans lequel rechercher.
 * @param {string} Vrac - La chaîne de caractères à rechercher.
 * @return {set<string>} Un ensemble de mots trouvés.
 */
set<string> SearchWords(map<string, string> Words, string Vrac);

/**
 * Charge un dictionnaire à partir d'un fichier. Chaque mot du fichier est trié
 * et ajouté au dictionnaire avec la version triée du mot comme clé et le mot
 * lui-même comme valeur.
 * @param {string} FileName - Le nom du fichier à charger.
 * @return {map<string, string>} Un dictionnaire avec les mots triés comme clés
 * et les mots originaux comme valeurs.
 */
void LoadDict(AI *AIHelper, string FileName);

/**
 * Charge une structure de données d'arbre à partir d'un fichier texte.
 * @param {string} FileName - Le nom du fichier à charger.
 * @return {Tree} L'arbre chargé à partir du fichier.
 */
void LoadTree(AI *AIHelper, string FileName);

set<string> FindWords(string Vrac, map<string, string> Words);

int findWord(BOARD Board, string Word, int Joueur);

void StartUpAI(AI *AIHelper);

Play *BestMove(BOARD Board, int Joueur, bool Jarnac, AI *AIHelper);