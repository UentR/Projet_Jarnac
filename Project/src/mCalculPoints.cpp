#include "mCalculPoints.hpp"

int calculPoints(vector<string> plateauJoueur, int nbMaxLettres,
                 int nbFinalMots) {
  int Points = 0;
  int Len;
  string Word;
  int Size = plateauJoueur.size();
  for (int i = 1; i < min(Size, nbFinalMots + 1); i++) {
    Word = plateauJoueur[i];
    Len = Word.length();
    if (Len <= nbMaxLettres && Len >= 3) {
      Points += Len * Len;
    }
  }
  return Points;
}
