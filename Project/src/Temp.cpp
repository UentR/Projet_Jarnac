#include <string.h>
#include <sys/shm.h>

#include <iostream>
#include <map>

using namespace std;

struct Keys {
  int keyPlay[2];
  int keyExchange[2];
  int keyJarnac;
  int keyLetters;
  int keyPlayMove;
  int keyConnected[2];
  int keyBoard;
};

#include <unistd.h>

#include "loadIA.hpp"

struct Adresses {
  bool *CanPlay[2];
  bool *CanExchange[2];
  bool *CanJarnac;
  char *ExchangeLetters;
  Play *Move;
  bool *Connected[2];
  BOARD *Board;
};

// int WaitForPlay(Adresses *Adrr, int Joueur, AI *AIHelper) {
//   while (*Adrr->Connected[Joueur]) {
//     if (*Adrr->CanPlay[Joueur]) {
//       Play *Move = new Play;
//       BestMove(Adrr->Board, Joueur, *Adrr->CanJarnac, AIHelper);
//       Adrr->Move = Move;
//       *Adrr->CanPlay[Joueur] = false;
//     }
//     usleep(100000);
//   }
//   return 0;
// }

int main() {
  AI *AIDemo = new AI;
  StartUpAI(AIDemo);
}

int main2(int argc, char const *argv[]) {
  int keyAdrr = atoi(argv[1]);
  int Joueur = atoi(argv[2]);
  Keys *key = (Keys *)shmat(keyAdrr, NULL, 0);

  Play *play = (Play *)shmat(key->keyPlayMove, NULL, 0);
  bool *MyTurn = (bool *)shmat(key->keyPlay[Joueur], NULL, 0);
  bool *MyExchange = (bool *)shmat(key->keyExchange[Joueur], NULL, 0);
  bool *Jarnac = (bool *)shmat(key->keyJarnac, NULL, 0);
  char *Letters = (char *)shmat(key->keyLetters, NULL, 0);
  bool *Connected = (bool *)shmat(key->keyConnected[Joueur], NULL, 0);
  BOARD *Board = (BOARD *)shmat(key->keyBoard, NULL, 0);

  cout << "Joueur " << Joueur << endl;
  cout << "MyTurn: " << *MyTurn << endl;
  cout << "MyExchange: " << *MyExchange << endl;
  cout << "Jarnac: " << *Jarnac << endl;
  cout << "Letters: " << *Letters << endl;

  cout << "Connected: " << *Connected << endl;

  *Jarnac = true;

  *MyExchange = true;
  *MyTurn = true;

  int Ligne = 1;
  // int Idx = 0;
  // char *CHAR = "BEST     ";
  // for (auto *t = Board + Joueur * 8 * 9 + Ligne * 9;
  //      t < Board + Joueur * 8 * 9 + (Ligne + 1) * 9; t++) {
  //   *t = CHAR[Idx];
  //   Idx++;
  // }
  string Word = string(argv[3]) + "         ";
  strncpy(Board->Board + Joueur * 8 * 10 + (Ligne - 1) * 10, Word.c_str(), 9);

  cout << "Board: " << endl;

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 2; j++) {
      cout << "| ";
      for (int k = 0; k < 9; k++) {
        cout << Board->Board[j * 8 * 10 + i * 10 + k] << " | ";
      }
      cout << "|||";
    }
    cout << endl;
  }

  for (int j = 0; j < 2; j++) {
    cout << "Vrac " << j << ": ";
    // Go through the char * array
    for (char *t = Board->Vracs[j]; *t != '\0'; t++) {
      cout << *t << " ";
    }
    cout << " Oui ";

    cout << endl;
  }

  *Connected = true;

  return 0;
}