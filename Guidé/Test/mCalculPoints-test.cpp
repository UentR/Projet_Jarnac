#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

#include "mCalculPoints.hpp"

/***Infrastructure minimale de test **/

#define CHECK(test) \
    if (!(test))    \
    cerr << "Test failed in file " << __FILE__ << " line " << __LINE__ << ": " #test << endl

/******************************************************************
 *  TTTTTTT   EEEEEE    SSS    TTTTTTT    SSS
 *     T      E        S   S      T      S   S
 *     T      E         S         T       S
 *     T      EEEEE      S        T        S
 *     T      E           S       T         S
 *     T      E        S   S      T      S   S
 *     T      EEEEEE    SSS       T       SSS
 *
 ******************************************************************
 */

int main()
{

    CHECK(calculPoints({"essai", "tentative", "mots", "affichable"},
                       8, 9) == 16);
    CHECK(calculPoints({"essai", "c", "est", "trop", "court"},
                       8, 9) == 50);
    CHECK(calculPoints({"essai", "tentative", "mots", "affichable"},
                       12, 6) == 197);
    CHECK(calculPoints({"essai", "recursivement", "enumerable"},
                       12, 6) == 100);
    CHECK(calculPoints({"essai", "tentative", "mots", "affichable"},
                       15, 3) == 197);
    CHECK(calculPoints({"essai", "tentative", "mots", "affichable"},
                       15, 1) == 81);
    CHECK(calculPoints({"abcdefghijklmnopqrstuvwxyz", "affichable"},
                       10, 8) == 100);
    CHECK(calculPoints({"abcdefghijklmnopqrstuvwxyz", "trop", "de", "mots"},
                       20, 2) == 16);
    CHECK(calculPoints({"abcdefghijklmnopqrstuvwxyz", "mots", "trop", "long", "oui"},
                       3, 10) == 9);
}
