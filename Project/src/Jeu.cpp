#include "controleurChoixAction.hpp"

int main2() {
  flushDebug();
  writeToDebugFile("main", INFO_DETAIL);
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

int main() {
  int keyAdrr = shmget(0x1240, sizeof(Keys), 0666 | IPC_CREAT);
  if (keyAdrr < 0) {
    cout << "Erreur de shmget" << endl;
    exit(1);
  }
  Keys *key = (Keys *)shmat(keyAdrr, NULL, 0);
  Adresses *Adrr = new Adresses;

  cout << "keyAdrr: " << keyAdrr << endl;

  int keyReady1 = shmget(0x1242, sizeof(bool), 0666 | IPC_CREAT);
  if (keyReady1 < 0) {
    cout << "Erreur de shmget Ready1" << endl;
    exit(1);
  }
  key->keyReady[0] = keyReady1;
  int keyReady2 = shmget(0x1243, sizeof(bool), 0666 | IPC_CREAT);
  if (keyReady2 < 0) {
    cout << "Erreur de shmget Ready2" << endl;
    exit(1);
  }
  key->keyReady[1] = keyReady2;

  Adrr->Ready[0] = (bool *)shmat(keyReady1, 0, 0);
  Adrr->Ready[1] = (bool *)shmat(keyReady2, 0, 0);

  *(Adrr->Ready[0]) = true;
  *(Adrr->Ready[1]) = true;

  cout << "here" << endl;

  StorePlayers *PlayersHelper = new StorePlayers;
  if (false) {
    writeToDebugFile("IA1 loading", ALL_LOG);
    // PlayersHelper->AIS[0] = new AI;
    // StartUpAI(PlayersHelper->AIS[0]);
    PlayersHelper->isAI[0] = true;
    int keyPlay = shmget(0x1244, sizeof(bool), 0666 | IPC_CREAT);
    if (keyPlay < 0) {
      cout << "Erreur de shmget keyPlay 1" << endl;
      exit(1);
    }
    key->keyPlay[0] = keyPlay;
    Adrr->CanPlay[0] = (bool *)shmat(keyPlay, 0, 0);
    *(Adrr->CanPlay[0]) = false;
    int keyExchange = shmget(0x1245, sizeof(bool *), 0666 | IPC_CREAT);
    if (keyExchange < 0) {
      cout << "Erreur de shmget keyExchange 1" << endl;
      exit(1);
    }
    key->keyExchange[0] = keyExchange;
    Adrr->CanExchange[0] = (bool *)shmat(keyExchange, 0, 0);
    *(Adrr->CanExchange[0]) = false;
    *(Adrr->Ready[0]) = false;
  }
  if (true) {
    writeToDebugFile("IA2 loading", ALL_LOG);
    // PlayersHelper->AIS[1] = new AI;
    // StartUpAI(PlayersHelper->AIS[1]);
    PlayersHelper->isAI[1] = true;
    int keyPlay = shmget(0x1246, sizeof(bool *), 0666 | IPC_CREAT);
    if (keyPlay < 0) {
      cout << "Erreur de shmget keyPlay 2" << endl;
      exit(1);
    }
    key->keyPlay[1] = keyPlay;
    Adrr->CanPlay[1] = (bool *)shmat(keyPlay, 0, 0);
    *(Adrr->CanPlay[1]) = false;
    int keyExchange = shmget(0x1247, sizeof(bool *), 0666 | IPC_CREAT);
    if (keyExchange < 0) {
      cout << "Erreur de shmget keyExchange 2" << endl;
      exit(1);
    }
    key->keyExchange[1] = keyExchange;
    Adrr->CanExchange[1] = (bool *)shmat(keyExchange, 0, 0);
    *(Adrr->CanExchange[1]) = false;
    *(Adrr->Ready[1]) = false;
  }
  if (false or true) {
    int keyJarnac = shmget(0x1248, sizeof(bool *), 0666 | IPC_CREAT);
    if (keyJarnac < 0) {
      cout << "Erreur de shmget keyJarnac" << endl;
      exit(1);
    }
    key->keyJarnac = keyJarnac;
    Adrr->CanJarnac = (bool *)shmat(keyJarnac, 0, 0);
    *(Adrr->CanJarnac) = false;
    int keyLetters = shmget(0x1249, 3, 0666 | IPC_CREAT);
    if (keyLetters < 0) {
      cout << "Erreur de shmget keyLetters" << endl;
      exit(1);
    }
    key->keyLetters = keyLetters;
    Adrr->ExchangeLetters = (char *)shmat(keyLetters, NULL, 0);
    strcpy(Adrr->ExchangeLetters, "   ");
    int keyPlayStruct = shmget(0x1250, sizeof(Play), 0666 | IPC_CREAT);
    if (keyPlayStruct < 0) {
      cout << "Erreur de shmget keyPlayStruct" << endl;
      exit(1);
    }
    key->keyPlayMove = keyPlayStruct;
    Adrr->Move = (Play *)shmat(keyPlayStruct, NULL, 0);
  }

  int keyBoard = shmget(0x1251, 8 * 9 * 2 + 1, 0666 | IPC_CREAT);
  if (keyBoard < 0) {
    cout << "Erreur de shmget keyBoard" << endl;
    exit(1);
  }
  key->keyBoard = keyBoard;
  Adrr->Board = (char *)shmat(keyBoard, NULL, 0);
  strcpy(Adrr->Board,
         "                                                                     "
         "                                                                     "
         "      ");

  int keyVrac1 = shmget(0x1252, 145, 0666 | IPC_CREAT);
  if (keyVrac1 < 0) {
    cout << "Erreur de shmget keyVrac 1" << endl;
    exit(1);
  }
  key->keyVrac[0] = keyVrac1;
  Adrr->Vrac[0] = (char *)shmat(keyVrac1, NULL, 0);
  strcpy(Adrr->Vrac[0],
         "                                                                     "
         "                                                                     "
         "      ");

  int keyVrac2 = shmget(0x1253, 145, 0666 | IPC_CREAT);
  if (keyVrac2 < 0) {
    cout << "Erreur de shmget keyVrac 2" << endl;
    exit(1);
  }
  key->keyVrac[1] = keyVrac2;
  Adrr->Vrac[1] = (char *)shmat(keyVrac2, NULL, 0);
  strcpy(Adrr->Vrac[1],
         "                                                                     "
         "                                                                     "
         "      ");
  cout << "Waiting for AIs..." << endl;
  cout << "Ready 1 : " << *(Adrr->Ready[0]) << endl;
  cout << "Ready 2 : " << *(Adrr->Ready[1]) << endl;
  while (!(*(Adrr->Ready[0])) or !(*(Adrr->Ready[1]))) {
    usleep(100000);
  }
  cout << "AIs ready" << endl;

  cout << Adrr->Board[130] << endl;

  shmctl(keyAdrr, IPC_RMID, 0);
  shmctl(key->keyPlay[0], IPC_RMID, 0);
  shmctl(key->keyPlay[1], IPC_RMID, 0);
  shmctl(key->keyExchange[0], IPC_RMID, 0);
  shmctl(key->keyExchange[1], IPC_RMID, 0);
  shmctl(key->keyJarnac, IPC_RMID, 0);
  shmctl(key->keyLetters, IPC_RMID, 0);
  shmctl(key->keyPlayMove, IPC_RMID, 0);
  shmctl(key->keyReady[0], IPC_RMID, 0);
  shmctl(key->keyReady[1], IPC_RMID, 0);
  shmctl(key->keyBoard, IPC_RMID, 0);
  shmctl(key->keyVrac[0], IPC_RMID, 0);
  shmctl(key->keyVrac[1], IPC_RMID, 0);

  return 0;
}