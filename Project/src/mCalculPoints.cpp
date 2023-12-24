#include "mCalculPoints.hpp"

int* calculPoints(char* Board, int nbMaxLettres, int nbFinalMots) {
  int Points[2] = {0, 0};
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < 8; i++) {
      int Len = 0;
      for (int k = 0; k < 9; k++) {
        if (Board[j * 8 * 10 + i * 10 + k] != '\0') {
          Len++;
        }
      }
      Points[j] += Len * Len;
    }
  }

  return Points;
}
