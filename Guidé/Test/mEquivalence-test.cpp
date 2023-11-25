#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

#include "mPoseMotJarnac-correction.hpp"
#include "mEquivalence.hpp"
/***Infrastructure minimale de test **/

#define CHECK(test) \
      if (!(test))  \
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

// travail sur l'équivalence, une specificaion, un test, une illustration
// le test couvre t-il tout les cas de l'illustration, l'illustration est-elle coherente?
// proposez un enrichissement du test
// proposez une implantation de la fonction.
// votre implantation passe t-elle le test?

void testEquivalence()
{
      cout << "testEquivalence" << endl;
      // tout est de bonne longueur et identique
      CHECK(equivautA({"lepidopteriste", "myrtil", "citron", "vulcain"}, {"lepidopteriste", "myrtil", "citron", "vulcain"}, 8, 9));

      CHECK(not equivautA({"lepidopteriste", "myrtil", "citron", "vulcain"}, {"chasseur", "louve", "ourse", "laie"}, 9, 8));

      // vrac melangee
      CHECK(equivautA({"abc", "epidopterologie", "edelweiss", "campanule"}, {"bac", "epidopterologie", "edelweiss", "campanule"}, 10, 4));

      CHECK(not equivautA({"abc", "epidopterologie", "edelweiss", "campanule"}, {"abcabc", "epidopterologie", "edelweiss", "campanule"}, 10, 4));

      // passage en majuscule
      CHECK(equivautA({"ABCDEFGHIJK", "RIEN"}, {"abcdefghijk", "rien"}, 5, 5));

      CHECK(equivautA({"ABCDEFGHIJK", "rien"}, {"ABCDEFGHIJK", "RIEN"}, 5, 5));

      // même mots valides dans le même ordre à la même position mais mots trop longs ignorés

      CHECK(equivautA({"ABC", "epidopterologie", "edelweiss", "campanule"}, {"ABC", "lepidoptériste", "edelweiss", "campanule"}, 10, 4));

      CHECK(not equivautA({"ABC", "epidopterologie", "campanule", "edelweiss"}, {"ABC", "lepidoptériste", "edelweiss", "campanule"}, 10, 4));

      // même mots valides dans le même ordre à la même position mais mots trop longs ignorés
      CHECK(equivautA({"ABC", "", "edelweiss", "campanule"}, {"ABC", "lepidopteriste", "edelweiss", "campanule"}, 10, 4));

      CHECK(not equivautA({"ABC", "edelweiss", "campanule"}, {"ABC", "lepidopteriste", "edelweiss", "campanule"}, 10, 4));
}

int main()
{
      testEquivalence();
}
