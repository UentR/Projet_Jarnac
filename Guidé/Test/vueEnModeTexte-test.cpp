#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

#include "vueEnModeTexte.hpp"

//pour le debug
#include "mCreeJeuJarnac-correction.hpp"

/***Infrastructure minimale de test **/


#define CHECK(test) if (!(test)) cerr << "Test failed in file " << __FILE__ << " line " << __LINE__ << ": " #test << endl


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

void test_traduitEnEntier() {
    cout << "test_traduitEnEntier" << endl;

    CHECK ( traduitEnEntier( vector<string>({"nu"}) ) == vector<int>({13}) );

    CHECK ( traduitEnEntier( vector<string>({"alpha", "beta", "gamma", "delta", "epsilon", "dzeta", "eta", "theta", "iota", "kappa", "lambda", "mu", "nu", "xi", "omicron", "pi", "rho", "sigma", "tau", "upsilon", "phi", "khi", "psi", "omega" } ) )
            == vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24} ) );
    CHECK ( traduitEnEntier( vector<string>({"eta", "xi", "mu", "mu", "mu", "beta"}))
            == vector<int>({7, 14, 12, 12, 12, 2}) );
}

void test_traduitEnLettreGrecque() {
    cout << "test_traduitEnLettreGrecque" << endl;
    CHECK ( traduitEnLettreGrecque( vector<int>({7, 14, 12, 12, 12, 2})) ==
            vector<string>({"eta", "xi", "mu", "mu", "mu", "beta"}) ) ;
    CHECK ( traduitEnLettreGrecque( vector<int>({11, 12, 13, 14, 15, 16})) ==
            vector<string>({"lambda", "mu", "nu", "xi", "omicron", "pi"}) ) ;
    CHECK ( traduitEnLettreGrecque(
                vector<int>(
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24} ) )
    ==
    vector<string>(
    {"alpha", "beta", "gamma", "delta", "epsilon", "dzeta", "eta", "theta", "iota", "kappa", "lambda", "mu", "nu", "xi", "omicron", "pi", "rho", "sigma", "tau", "upsilon", "phi", "khi", "psi", "omega" }
    )
          );
    CHECK ( traduitEnLettreGrecque(
                vector<int>({24}) ) == vector<string>({"omega"}));
}

int main() {
    test_traduitEnEntier();
    test_traduitEnLettreGrecque();
}

