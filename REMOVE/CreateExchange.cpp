#include <fstream>
#include <iostream>
#include <map>

using namespace std;

int main() {
  ifstream File("Text/WordsIA.txt");
  string Word;
  string Vrac;
  map<char, double> Words;
  map<char, double> Vracs;
  int Count = 0;
  int CountVrac = 0;
  int CountWord = 0;
  while (File >> Word) {
    Count++;
    cout << Count << endl;
    if (Word == "Vrac:") {
      for (int i = 0; i < 2; i++) {
        File >> Vrac;
        for (char c : Vrac) {
          Vracs[c]++;
          CountVrac++;
        }
      }
    } else {
      for (char c : Word) {
        Words[c]++;
        CountWord++;
      }
    }
  }

  ofstream File2("Text/LettersIA.txt");
  File2 << "Words:" << endl;
  for (auto const &x : Words) {
    File2 << x.first << " " << x.second / CountWord * 100 << endl;
  }
  File2 << endl << "Vracs:" << endl;
  for (auto const &x : Vracs) {
    File2 << x.first << " " << x.second / CountVrac * 100 << endl;
  }
  return 0;
}