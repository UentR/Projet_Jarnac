#include "test-dico.hpp"

int main() {
  ForDict *Dict1 = new ForDict;
  ForDict *Dict2 = new ForDict;
  ForDict *Dict3 = new ForDict;
  CreateHelper(Dict1, "Text/DictionnairePurified.txt", 1);
  CreateHelper(Dict2, "Text/DictionnairePurified.txt", 2);
  CreateHelper(Dict3, "Text/DictionnairePurified.txt", 3);

  clock_t Deb;
  clock_t Fin;
  string Word;
  int Idx;
  int Len;
  srand(time(NULL));
  int Nbr = 0;
  while (true) {
    if (rand() % 5 == 0) {
      Len = rand() % 6;
      Len += 3;
      Word = "";
      for (int i = 0; i < Len; i++) {
        Word += (char)(rand() % 26 + 65);
      }
    } else {
      Idx = rand() % 169080;
      Word = Dict1->Mots[Idx];
    }
    writeToDebugFile("Word N°" + to_string(Nbr) + " : " + Word);
    Deb = clock();
    trouve(Word, Dict1);
    Fin = clock();
    writeToDebugFile("Dict1 : " +
                     to_string((float)(Fin - Deb) / CLOCKS_PER_SEC * 1000));
    Deb = clock();
    trouve(Word, Dict2);
    Fin = clock();
    writeToDebugFile("Dict2 : " +
                     to_string((float)(Fin - Deb) / CLOCKS_PER_SEC * 1000));
    Deb = clock();
    trouve(Word, Dict3);
    Fin = clock();
    writeToDebugFile("Dict3 : " +
                     to_string((float)(Fin - Deb) / CLOCKS_PER_SEC * 1000));
    Deb = clock();
    for (int i = 0; i < 169080; i++) {
      if (Dict1->Mots[i] == Word) {
        break;
      }
    }
    Fin = clock();
    writeToDebugFile("All Dict : " +
                     to_string((float)(Fin - Deb) / CLOCKS_PER_SEC * 1000));
    writeToDebugFile("");
    Nbr++;
  }
}