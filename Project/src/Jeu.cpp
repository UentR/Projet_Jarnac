#include "controleurChoixAction.hpp"

int main() {
  flushDebug();
  writeToDebugFile("main", INFO_DETAIL);

  lanceLeJeu("1", "2", "Jarnac", true, true);

  string j1, j2;
  cout << "Nom du premier joueur : ";
  // cin >> j1;
  cout << "Nom du deuxième joueur : ";
  // cin >> j2;

  j1 = "Joueur 1";
  j2 = "Joueur 2";

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
  // cin >> ChoixUtilisateur;
  ChoixUtilisateur = "A";
  ChoixUtilisateur = purifie(ChoixUtilisateur);

  if (ChoixUtilisateur == "A") {
    lanceLeJeu(j1, j2, "Jarnac", false, true);
  } else if (ChoixUtilisateur == "P") {
    lanceLeJeu(j1, j2, "Jarnac", false, false);
  } else {
    lanceLeJeu(j1, j2, "Jarnac", true, true);
  }
  writeToDebugFile("main end", INFO_DETAIL);
  return 0;
}

int main2() {
  Adresses *Adrr = new Adresses;

  int keyAdrr = shmget(ftok("Jarnac", 1), sizeof(Keys), 0666 | IPC_CREAT);
  if (keyAdrr < 0) Error("Erreur de shmget keyAdrr");
  Keys *key = (Keys *)shmat(keyAdrr, NULL, 0);

  cout << "keyAdrr: " << keyAdrr << endl;

  // Loading key to tell if players are connected
  int keyConnected1 = shmget(ftok("Jarnac", 2), sizeof(bool), 0666 | IPC_CREAT);
  if (keyConnected1 < 0) Error("Erreur de shmget Connected1");
  key->keyConnected[0] = keyConnected1;
  int keyConnected2 = shmget(ftok("Jarnac", 3), sizeof(bool), 0666 | IPC_CREAT);
  if (keyConnected2 < 0) Error("Erreur de shmget Connected2");
  key->keyConnected[1] = keyConnected2;

  // Load adrress of said variable
  Adrr->Connected[0] = (bool *)shmat(keyConnected1, 0, 0);
  Adrr->Connected[1] = (bool *)shmat(keyConnected2, 0, 0);
  *(Adrr->Connected[0]) = false;
  *(Adrr->Connected[1]) = false;

  // Load key to tell which player it is to play
  int keyPlay0 = shmget(ftok("Jarnac", 4), sizeof(bool), 0666 | IPC_CREAT);
  if (keyPlay0 < 0) Error("Erreur de shmget keyPlay 1");
  key->keyPlay[0] = keyPlay0;
  int keyPlay1 = shmget(ftok("Jarnac", 6), sizeof(bool *), 0666 | IPC_CREAT);
  if (keyPlay1 < 0) Error("Erreur de shmget keyPlay 2");
  key->keyPlay[1] = keyPlay1;

  // Load/Set the variables
  Adrr->CanPlay[1] = (bool *)shmat(keyPlay1, 0, 0);
  *(Adrr->CanPlay[1]) = false;
  Adrr->CanPlay[0] = (bool *)shmat(keyPlay0, 0, 0);
  *(Adrr->CanPlay[0]) = false;

  // Load key to tell if players can exchange letters
  int keyExchange0 =
      shmget(ftok("Jarnac", 5), sizeof(bool *), 0666 | IPC_CREAT);
  if (keyExchange0 < 0) Error("Erreur de shmget keyExchange 1");
  key->keyExchange[0] = keyExchange0;
  int keyExchange1 =
      shmget(ftok("Jarnac", 7), sizeof(bool *), 0666 | IPC_CREAT);
  if (keyExchange1 < 0) {
    cout << "Erreur de shmget keyExchange 2" << endl;
    exit(1);
  }
  key->keyExchange[1] = keyExchange1;

  // Load/Set
  Adrr->CanExchange[1] = (bool *)shmat(keyExchange1, 0, 0);
  *(Adrr->CanExchange[1]) = false;
  Adrr->CanExchange[0] = (bool *)shmat(keyExchange0, 0, 0);
  *(Adrr->CanExchange[0]) = false;

  // Load key to tell players if Jarnac is possible
  int keyJarnac = shmget(ftok("Jarnac", 8), sizeof(bool *), 0666 | IPC_CREAT);
  if (keyJarnac < 0) Error("Erreur de shmget keyJarnac");
  key->keyJarnac = keyJarnac;
  // Load/Set
  Adrr->CanJarnac = (bool *)shmat(keyJarnac, 0, 0);
  *(Adrr->CanJarnac) = false;

  // Load key to get choice of player when new turn
  int keyLetters = shmget(ftok("Jarnac", 9), 3, 0666 | IPC_CREAT);
  if (keyLetters < 0) Error("Erreur de shmget keyLetters");
  key->keyLetters = keyLetters;
  // Load/Set
  Adrr->ExchangeLetters = (char *)shmat(keyLetters, NULL, 0);
  strcpy(Adrr->ExchangeLetters, "   ");

  // Load key to get choice of player when placing words
  int keyPlayStruct =
      shmget(ftok("Jarnac", 10), sizeof(Play), 0666 | IPC_CREAT);
  if (keyPlayStruct < 0) Error("Erreur de shmget keyPlayStruct");
  key->keyPlayMove = keyPlayStruct;
  // Load
  Adrr->Move = (Play *)shmat(keyPlayStruct, NULL, 0);

  // Init of the Board
  BOARD *Board = new BOARD;
  Board->Board;
  Board->Vracs[0];
  Board->Vracs[1];

  // Load key for the board
  int keyBoard = shmget(ftok("Jarnac", 11), sizeof(BOARD), 0666 | IPC_CREAT);
  if (keyBoard < 0) Error("Erreur de shmget keyBoard");
  key->keyBoard = keyBoard;
  // Load/Set
  Adrr->Board = (BOARD *)shmat(keyBoard, NULL, 0);
  strcpy(Adrr->Board->Board,
         "                                                                 "
         "    "
         "                                                                 "
         "    "
         "                      ");
  cout << "Waiting for AIs" << endl;
  while (!(*(Adrr->Connected[0])) or !(*(Adrr->Connected[1]))) {
    usleep(100000);
  }
  cout << "AIs ready" << endl;

  cout << Adrr->Board->Board[130] << endl;

  shmctl(keyAdrr, IPC_RMID, 0);
  shmctl(key->keyPlay[0], IPC_RMID, 0);
  shmctl(key->keyPlay[1], IPC_RMID, 0);
  shmctl(key->keyExchange[0], IPC_RMID, 0);
  shmctl(key->keyExchange[1], IPC_RMID, 0);
  shmctl(key->keyJarnac, IPC_RMID, 0);
  shmctl(key->keyLetters, IPC_RMID, 0);
  shmctl(key->keyPlayMove, IPC_RMID, 0);
  shmctl(key->keyConnected[0], IPC_RMID, 0);
  shmctl(key->keyConnected[1], IPC_RMID, 0);
  shmctl(key->keyBoard, IPC_RMID, 0);

  return 0;
}