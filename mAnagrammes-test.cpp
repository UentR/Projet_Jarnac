
#include <vector>
#include <string>


using namespace std;

#include "mAnagrammes.hpp"
#include "mDictionnaire.hpp"

using namespace std;






/***Infrastructure minimale de test **/


#define CHECK(test) if (!(test)) cerr << "Test failed in file " << __FILE__ << " line " << __LINE__ << ": " #test << endl


int main() {


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

    CHECK ( retire("bonjour", 'o') == "bnjour");

    CHECK ( retire("bonjour", 'p') == "-");


    CHECK ( est_anagramme("rien", "rien") );
    CHECK ( est_anagramme("nier", "rein") );

    //CHECK (listeDesAnagrammesDuDico("rien","dictionnaire.txt") == {"rien", "nier","rein"} );
    //afficheListedeMots( listeDesAnagrammesDuDico("rien","dictionnaire.txt"));

    CHECK( retire("abcd", "bac")   == "d");
    CHECK( retire("ab", "bac")     == "-");
    CHECK( retire("abc", "bac")    == "");
    CHECK( retire("famille", "m") == "faille");
    CHECK( retire("garder", "regard") == "");
    CHECK( (retire("brossent", "sorbet") == "ns")
           or (retire("brossent", "sorbet") == "sn")
         ) ;

}















