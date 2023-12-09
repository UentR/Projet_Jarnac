#include <math.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**************************************
 *
 * DDDD    IIIII     CCC    OOO
 *  D  D     I      C      O   O
 *  D  D     I     C       O   O
 *  D  D     I      C      O   O
 * DDDD    IIIII     CCC    OOO
 *
 ***************************************/

/**
 * @brief Structure contenant le dictionnaire
 * @param vector<string> Mots - Les mots du dictionnaire
 * @param vector<long> Bornes - Les bornes de chaque couple de NBR premières
 * @param int NBR - Le nombre de lettres à prendre en compte
 * @param int SHIFT - Le nombre de bits de décalage
 */
struct ForDict {
  vector<string> Mots;
  vector<long> Bornes;
  int NBR;
  int SHIFT;
};

/**
 * @brief Convertit en majuscule
 * @param char lettre - La lettre à convertir
 * @return char - La lettre en majuscule, ou '-' si ce n'est pas une
 * lettre
 */
char majuscule(char lettre);

/**
 * @brief Convertit en majuscule
 * @param string Lettre - La lettre à convertir
 * @return string - La lettre en majuscule, ou '-' si ce n'est pas une
 * lettre
 */
string majuscule(string Lettre);

/**
 * @brief «Purifie» un mot en retirant espaces et caractères spéciaux et en le
 * mettant en majuscules
 * @param string mot - Le mot à purifier
 * @return string - Le mot purifié
 */
string purifie(string mot);

/**
 * @brief «Purifie» un nombre en retirant espaces et autres caractères
 * @param string Nbr - Le nombre à purifier
 * @return string - Le nombre purifié
 */
string purifieNbr(string Nbr);

/**
 * @brief Trouve un mot dans un dictionnaire
 * @param string mot - Le mot a trouver
 * @param ForDict* DictHelper - La structure contenant le dictionnaire
 * @return bool - Le résultat de la recherche
 */
bool trouve(string mot, ForDict *DictHelper);

/**
 * @brief Calcule l'indexe d'un couple de NBR lettre
 * @note Convertisseur de la base 26 de l'alphabet en base 10
 * @param string Word - le mot dont on cherche l'indexe
 * @return int - l'indexe correspondant
 */
int CalcIdx(string Word, int NBR);

/**
 * @brief Crée les bornes de chaque couple de NBR première lettres
 * @param ForDict* DictHelper - La structure contenant le dictionnaire
 */
void CreateBorne(ForDict *DictHelper);

/**
 * @brief Importe un dictionnaire
 * @param string adresseDico - L'adresse du dictionnaire
 * @param ForDict* DictHelper - La structure contenant le dictionnaire
 */
void importeDico(string adresseDico, ForDict *DictHelper);

/**
 * @brief Charge la structure contenant le dictionnaire
 * @param ForDict* DictHelper - La structure contenant le dictionnaire
 * @param string adresseDico - L'adresse du dictionnaire
 * @param int NBR - Le nombre de lettres à prendre en compte
 */
void CreateHelper(ForDict *DictHelper, string adresseDico, int NBR);
