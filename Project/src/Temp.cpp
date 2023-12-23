#include <sys/shm.h>

#include <iostream>

using namespace std;

struct Keys {
  int keyPlay[2];
  int keyExchange[2];
  int keyJarnac;
  int keyLetters;
  int keyPlayMove;
  int keyReady[2];
  int keyBoard;
  int keyVrac[2];
};

struct Play {
  string Word;
  string DLetter;
  int Ligne;
  int Origin;
  bool Jarnac;
  bool End;
};

int main(int argc, char const *argv[]) {
  int keyAdrr = atoi(argv[1]);
  int Joueur = atoi(argv[2]);
  Keys *key = (Keys *)shmat(keyAdrr, NULL, 0);

  Play *play = (Play *)shmat(key->keyPlayMove, NULL, 0);
  bool *MyTurn = (bool *)shmat(key->keyPlay[Joueur], NULL, 0);
  bool *MyExchange = (bool *)shmat(key->keyExchange[Joueur], NULL, 0);
  bool *Jarnac = (bool *)shmat(key->keyJarnac, NULL, 0);
  char *Letters = (char *)shmat(key->keyLetters, NULL, 0);
  bool *PlayEnd = (bool *)shmat(key->keyReady[Joueur], NULL, 0);
  char *Board = (char *)shmat(key->keyBoard, NULL, 0);
  char *Vracs[2];
  Vracs[0] = (char *)shmat(key->keyVrac[Joueur], NULL, 0);
  Vracs[1] = (char *)shmat(key->keyVrac[1 - Joueur], NULL, 0);

  cout << "Joueur " << Joueur << endl;
  cout << "MyTurn: " << *MyTurn << endl;
  cout << "MyExchange: " << *MyExchange << endl;
  cout << "Jarnac: " << *Jarnac << endl;
  cout << "Letters: " << *Letters << endl;

  cout << "PlayEnd: " << *PlayEnd << endl;

  *Jarnac = true;

  *MyExchange = true;
  *MyTurn = true;

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 2; j++) {
      cout << "| ";
      for (int k = 0; k < 9; k++) {
        cout << Board[j * 8 * 9 + i * 8 + k] << " | ";
      }
      cout << " |||";
    }
    cout << endl;
  }

  Board[130] = '3';

  for (int j = 0; j < 2; j++) {
    cout << "Vrac " << j << ": ";
    for (int i = 0; i < 8; i++) {
      cout << Vracs[j][i] << " ";
    }
    cout << endl;
  }

  *PlayEnd = true;

  return 0;
}