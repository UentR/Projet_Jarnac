#include "controleurChoixAction.hpp"

int main() {
  flushDebug();
  writeToDebugFile("main", INFO_DETAIL);
  string j1, j2;
  cout << "Nom du premier joueur : ";
  cin >> j1;
  cout << "Nom du deuxième joueur : ";
  cin >> j2;

  srand(time(NULL));
  bool shuffle = false;
  if (rand() % 2) {
    shuffle = true;
    string Temp = j1;
    j1 = j2;
    j2 = Temp;
  }

  cout << "Quel mode de jeu voulez-vous :" << endl
       << "    A: Player versus IA" << endl
       << "    P: Player versus Player" << endl
       << "    D: Demo des IA" << endl;
  string ChoixUtilisateur;
  cin >> ChoixUtilisateur;
  ChoixUtilisateur = purifie(ChoixUtilisateur);

  if (ChoixUtilisateur == "A") {
    lanceLeJeu(j1, j2, "Jarnac", shuffle, !shuffle);
  } else if (ChoixUtilisateur == "P") {
    lanceLeJeu(j1, j2, "Jarnac", false, false);
  } else {
    lanceLeJeu(j1, j2, "Jarnac", true, true);
  }
  writeToDebugFile("main end", INFO_DETAIL);
  return 0;
}