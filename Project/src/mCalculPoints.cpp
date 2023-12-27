#include "mCalculPoints.hpp"

#include <iostream>

int* calculPoints(char* Board, int nbMaxLettres, int nbFinalMots) {
  int* Points = new int[2];
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < 8; i++) {
      int Len = 0;
      for (int k = 0; k < 9; k++) {
        if (*(Board + j * 8 * 10 + i * 10 + k) != '\0') {
          Len++;
        }
      }
      Points[j] += Len * Len;
    }
  }

  return Points;
}
