#include <ctime>

#include "Debug.hpp"
#include "mDictionnaire.hpp"

int main() {
  ForDict *Dict1 = new ForDict;
  ForDict *Dict2 = new ForDict;
  ForDict *Dict3 = new ForDict;
  CreateHelper(Dict1, "Text/DictionnairePurified.txt", 1);
  CreateHelper(Dict2, "Text/DictionnairePurified.txt", 2);
  CreateHelper(Dict3, "Text/DictionnairePurified.txt", 3);

  time_t Deb;
  time_t Fin;
  string Word;
  while (true) {
  }
}