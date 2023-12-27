#include "controleurChoixAction.hpp"

struct ForDict;
struct Play;
struct IA;
struct Names;
struct StorePlayers;
struct StoreLetters;

string Sort(string Mot) {
  string Sorted = Mot;
  sort(Sorted.begin(), Sorted.end());
  return Sorted;
}

int getLine(char *Board, char *Word, int Joueur) {
  bool Same = true;
  for (int ligne = 0; ligne < 8; ligne++) {
    char *BoardLine = Board + Joueur * 80 + ligne * 10;
    Same = true;
    for (int i = 0; i < strlen(Word); i++) {
      if (BoardLine[i] != Word[i]) {
        Same = false;
        break;
      }
    }
    if (Same) {
      return ligne;
    }
  }
  return -1;
}

bool isVowel(char Letter) {
  writeToDebugFile("isVowel", INFO_DETAIL);
  return (Letter == 'A' or Letter == 'E' or Letter == 'I' or Letter == 'O' or
          Letter == 'U' or Letter == 'Y');
}

bool CheckVowel(char *Letters, int Start, int End) {
  writeToDebugFile("CheckVowel", INFO_DETAIL);
  for (int i = Start; i > End; i--) {
    if (isVowel(Letters[i])) {
      return true;
    }
  }
  return false;
}

char *createLetters() {
  writeToDebugFile("createLetters", INFO_DETAIL);
  // Toutes les lettres de base du jeu
  // Les mettre dans un fichier texte ?
  char *Letters = new char[145];
  strcpy(
      Letters,
      "AAAAAAAAAAAAAABBBBCCCCCCCDDDDDEEEEEEEEEEEEEEEEEEEFFGGGGHHIIIIIIIIIIIJKLL"
      "LLLLMMMMMNNNNNNNNNOOOOOOOOPPPPQRRRRRRRRRRSSSSSSSTTTTTTTTTUUUUUUUUVVWXYZ"
      "Z");

  srand(time(NULL));
  bool Vowel1 = false;
  bool Vowel2 = false;
  int Size = strlen(Letters);
  do {
    // Shuffle Letters
    random_shuffle(Letters, Letters + strlen(Letters));

    // Verifier s'il y a bien une voyelle dans le vrac de chaque joueur
    Vowel1 = CheckVowel(Letters, Size, Size - TAILLE_VRAC);
    Vowel2 =
        CheckVowel(Letters, Size - 1 - TAILLE_VRAC, Size - 2 * TAILLE_VRAC);
  } while ((!Vowel1) || (!Vowel2));
  writeToDebugFile("createLetters end", INFO_DETAIL);

  return Letters;
}

void Exchange(char *LETTERS, Adresses *Adrr, int Joueur) {
  writeToDebugFile("Exchange", INFO_DETAIL);
  for (int i = 0; i < 3; i++) {
    char Letter = piocheLettre(LETTERS);
    strcat(Adrr->Board->Vracs[Joueur], &Letter);
  }
  // Remove Adrr->ExchangeLetters from Adrr->Board->Vracs[Joueur]
  int Remove[3] = {-1, -1, -1};
  int Idx = 0;
  for (int i = 0; i < strlen(Adrr->Board->Vracs[Joueur]); i++) {
    for (int j = 0; j < 3; j++) {
      if (Adrr->Board->Vracs[Joueur][i] == Adrr->ExchangeLetters[j]) {
        Remove[Idx] = i;
        Idx++;
        break;
      }
    }
  }
  if (Remove[2] == -1) {
    cout << "Erreur dans le retrait des lettres" << endl;
    exit(1);
  }

  for (int i = 0; i < 3; i++) {
    memmove(&Adrr->Board->Vracs[Joueur][Remove[i]],
            &Adrr->Board->Vracs[Joueur][Remove[i] + 1],
            144 - Remove[i] - 1);  // Remove index
  }

  strcat(LETTERS, Adrr->ExchangeLetters);
  // On mélange le sac
  random_shuffle(LETTERS, LETTERS + strlen(LETTERS) - 1);
  writeToDebugFile("Exchange end", INFO_DETAIL);
}

char piocheLettre(char *LETTERS) {
  writeToDebugFile("piocheLettre", INFO_DETAIL);
  int Size = strlen(LETTERS) - 1;
  if (Size >= 0) {
    // Si le sac n'est pas vide
    char Letter = LETTERS[Size];
    LETTERS[Size] = '\0';
    writeToDebugFile("piocheLettre end letter", INFO_DETAIL);
    return Letter;
  }
  cout << "Le sac est vide " << endl;
  cout << "Appuyez sur 'enter' pour continuer" << endl;
  // getchar();
  writeToDebugFile("piocheLettre end no letter", INFO_DETAIL);
  return '\0';
}

void ActionPossible() {
  writeToDebugFile("ActionPossible", INFO_DETAIL);
  cout << "Au cours du jeu il est possible de taper certaines lettres pour "
          "piloter la manche."
       << endl;
  cout << "    A : Abandonner la partie" << endl;
  cout << "    C : Continuer en choisissant un mot" << endl;
  cout << "    F : Finir son tour" << endl;
  cout << "    J : crier Jarnac" << endl;
  cout << "    M : voir ce Menu" << endl;
  cout << "    P : voir le Plateau" << endl;
  cout << endl;
  writeToDebugFile("ActionPossible end", INFO_DETAIL);
}

string retire(string mot, char lettre) {
  string Rep = "";
  char Current;
  int Count = 0;
  for (char Current : mot) {
    if (Current == lettre) {
      break;
    }
    Rep += Current;
    Count++;
  }
  if (Count == mot.length()) return "-";
  for (int i = Count + 1; i < mot.size(); i++) {
    Current = mot[i];
    Rep += Current;
  }
  return Rep;
}

string retire(string vrac, string mot) {
  string Rep = vrac;
  for (char i : mot) {
    Rep = retire(Rep, i);
  }
  return Rep;
}

void retire(char *Vrac, string mot) {
  for (char i : mot) {
    for (int j = 0; j < strlen(Vrac); j++) {
      if (Vrac[j] == i) {
        memmove(&Vrac[j], &Vrac[j + 1], 144 - j);
        break;
      }
    }
  }
}

void Place(char *Board, string Temp, int Ligne, int Joueur) {
  // Convert string to char *
  char *Word = new char[10];
  strcpy(Word, Temp.c_str());
  strcat(Word, "         ");
  strncpy(Board + Joueur * 80 + Ligne * 10, Word, 9);
}

TentativeMotPlace JouerMot(BOARD *Board, int Joueur, Play *Current,
                           Names *NamesHelper, ForDict *DictHelper,
                           char *LETTERS) {
  writeToDebugFile("JouerMot", INFO_DETAIL);
  if (Current->End) {
    writeToDebugFile("JouerMot end avec FinTourIA", INFO_DETAIL);
    // Si l'IA a fini son tour
    return FinTourIA;
  }

  // Debut check si mot possible
  writeToDebugFile("Debut test", ERROR);
  if (Current->DLetter == "-") {
    // Si aucune lettre n'est ajoutée
    affichePlateaux(Board, NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                    NamesHelper->NameGame, NamesHelper->Name1,
                    NamesHelper->Name2, Joueur, false);
    cout << "Vous devez ajouter au moins une lettre. Taper 'R' pour "
            "choisir une autre ligne. Taper 'Q' pour changer d'action."
         << endl;
    writeToDebugFile("JouerMot end avec aucune lettre ajoutée", INFO_DETAIL);
    return EchecPlacementMot;
  }
  for (auto i : Current->DLetter) {
    // Si la lettre n'est pas dans le vrac
    int DJ = Joueur;
    if (Current->Jarnac) {
      DJ = 1 - Joueur;
    }
    bool in = false;
    for (int j = 0; j < strlen(Board->Vracs[DJ]); j++) {
      if (Board->Vracs[DJ][j] == i) {
        in = true;
        break;
      }
    }
    if (!in) {
      affichePlateaux(Board, NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                      NamesHelper->NameGame, NamesHelper->Name1,
                      NamesHelper->Name2, Joueur, false);
      cout << "Lettre impossible à jouer. Taper 'R' pour choisir une autre "
              "ligne. Taper 'Q' pour changer d'action."
           << endl;
      writeToDebugFile("JouerMot end avec lettre impossible à jouer",
                       INFO_DETAIL);
      return EchecPlacementMot;
    }
  }
  if (!trouve(Current->Word, DictHelper)) {
    // Si le mot n'est pas dans le dictionnaire
    affichePlateaux(Board, NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                    NamesHelper->NameGame, NamesHelper->Name1,
                    NamesHelper->Name2, Joueur, false);
    cout << "Mot inexistant. Taper 'R' pour choisir une autre "
            "ligne. "
            "Taper 'Q' pour changer d'action."
         << endl;
    writeToDebugFile("Mot joué : " + Current->Word, INFO);
    writeToDebugFile("JouerMot end avec mot inexistant", INFO_DETAIL);
    return EchecPlacementMot;
  }
  writeToDebugFile("Fin test", ERROR);
  // Fin check si mot possible

  // Ici mot possible
  writeToDebugFile("Jarnac : " + to_string(Current->Jarnac), ERROR);
  writeToDebugFile("Origin : " + to_string(Current->Origin), ERROR);
  writeToStatsFile(Current->Word);
  if (!(Current->Jarnac)) {
    writeToDebugFile("Jarnac false", ERROR);
    writeToDebugFile("Ligne : " + to_string(Current->Ligne), ERROR);
    writeToDebugFile("Current->Word : " + Current->Word, ERROR);
    // Retire du vrac - Ajoute au plateau - Ajoute une lettre au vrac
    retire(Board->Vracs[Joueur], Current->DLetter);
    Place(Board->Board, Current->Word, Current->Ligne, Joueur);
    char Letter = piocheLettre(LETTERS);
    strcat(Board->Vracs[Joueur], &Letter);
  } else {
    // Retire du vrac de l'adversaire - (Retire du plateau adverse) - Ajoute
    // à notre plateau
    retire(Board->Vracs[1 - Joueur], Current->DLetter);
    if (Current->Origin > 0) {
      Place(Board->Board, "", Current->Ligne, 1 - Joueur);
    }
    Place(Board->Board, Current->Word, Current->Ligne, Joueur);
  }

  affichePlateaux(Board, NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                  NamesHelper->NameGame, NamesHelper->Name1, NamesHelper->Name2,
                  Joueur, false);
  writeToDebugFile("JouerMot end avec mot joué : " + Current->Word,
                   INFO_DETAIL);
  return ReussitePlacementMot;
}

string Round(Adresses *Adrr, int Joueur, ForDict *DictHelper,
             Names *NamesHelper, int Tour, char *LETTERS) {
  Play *IAMove = new Play;
  TentativeMotPlace state = EchecPlacementMot;

  if (Tour > 0) {
    *Adrr->CanJarnac = true;
    do {
      *Adrr->CanPlay[Joueur] = true;
      while (*Adrr->CanPlay[Joueur]) {
        usleep(100000);
      }
      // IAMove = BestMove(Board, Joueur, true, PlayerHelper->AIS[Joueur]);
      state = JouerMot(Adrr->Board, Joueur, Adrr->Move, NamesHelper, DictHelper,
                       LETTERS);
    } while (state == ReussitePlacementMot);
  }

  if (Tour >= 2) {
    *Adrr->CanExchange[Joueur] = true;
    while (*Adrr->CanExchange[Joueur]) {
      usleep(100000);
    }
    // On ne pioche qu'à partir du 2e tour
    // string LettreIA = PiocheEchange(Board, Joueur);
    if (strlen(Adrr->ExchangeLetters) > 0) {
      char Letter = piocheLettre(LETTERS);
      strcat(Adrr->Board->Vracs[Joueur], &Letter);
    } else {
      Exchange(LETTERS, Adrr, Joueur);
    }
  }
  *Adrr->CanJarnac = false;
  do {
    // affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU,
    // TAILLE_MAX_MOT,
    //                 NamesHelper->NameGame, NamesHelper->Name1,
    //                 NamesHelper->Name2, Joueur, false);

    *Adrr->CanPlay[Joueur] = true;
    while (*Adrr->CanPlay[Joueur]) {
      usleep(100000);
    }
    // IAMove = BestMove(Board, Joueur, true, PlayerHelper->AIS[Joueur]);
    state = JouerMot(Adrr->Board, Joueur, Adrr->Move, NamesHelper, DictHelper,
                     LETTERS);

    // Si l'IA a joué un mot impossible
    if (state == EchecPlacementMot) {
      affichePlateaux(Adrr->Board, NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                      NamesHelper->NameGame, NamesHelper->Name1,
                      NamesHelper->Name2, Joueur, false);
      writeToDebugFile("Mot joué : " + IAMove->Word, ERROR);
      writeToDebugFile("Lettres jouées : " + IAMove->DLetter, ERROR);
      writeToDebugFile("Ligne jouée : " + to_string(IAMove->Ligne), ERROR);
      writeToDebugFile("Ligne d'origine : " + to_string(IAMove->Origin), ERROR);
      writeToDebugFile("Jarnac : " + to_string(IAMove->Jarnac), ERROR);
      writeToDebugFile("Fin du jeu IA a joué mot impossible", ERROR);
      throw invalid_argument("Tentative de placement: ECHEC.");
    }
  } while (state == ReussitePlacementMot);
  writeToDebugFile("Round end", INFO_DETAIL);
  return "";
}

bool endGame(char *Board, int Joueur) {
  writeToDebugFile("endGame", INFO_DETAIL);
  // Si toutes les lignes sont commencées et aucun jarnac n'est possible
  // alors fin de la partie
  for (int j = 0; j < 8; j++) {
    if (Board[Joueur * 80 + j * 10] == ' ') {
      writeToDebugFile("endGame end avec false", INFO_DETAIL);
      return false;
    }
  }
  writeToDebugFile("endGame end avec true", INFO_DETAIL);
}

string choisirDictionnaire() {
  writeToDebugFile("choisirDictionnaire", INFO_DETAIL);
  string choix;
  cout << "Quel dictionnaire souhaitez-vous choisir" << endl;
  cin >> choix;
  writeToDebugFile("Choix utilisateur " + choix, ALL_LOG);
  writeToDebugFile("choisirDictionnaire end", INFO_DETAIL);
  return choix;
}

void Error(char *msg) {
  cout << msg << endl;
  exit(1);
}

void initGame(Adresses *Adrr) {
  writeToDebugFile("initGame", INFO_DETAIL);
  int keyAdrr = shmget(ftok("Jarnac", 1), sizeof(Keys), 0666 | IPC_CREAT);
  if (keyAdrr < 0) Error("Erreur de shmget keyAdrr");
  Keys *key = (Keys *)shmat(keyAdrr, NULL, 0);

  cout << "keyAdrr : " << keyAdrr << endl;

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

  // Load key for the board
  int keyBoard = shmget(ftok("Jarnac", 11), sizeof(BOARD), 0666 | IPC_CREAT);
  if (keyBoard < 0) Error("Erreur de shmget keyBoard");
  key->keyBoard = keyBoard;
  // Load/Set
  Adrr->Board = (BOARD *)shmat(keyBoard, NULL, 0);
  // Board->Board = new char[2 * 8 * 10 + 1];
  // Board->Vracs[0] = new char[145];
  // Board->Vracs[1] = new char[145];
  strcpy(Adrr->Board->Board,
         "                                                                 "
         "    "
         "                                                                 "
         "    "
         "                      ");
  writeToDebugFile("initGame end", INFO_DETAIL);
}

void SetupVracs(Adresses *Adrr, char *Letters) {
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < 6; i++) {
      char Letter = piocheLettre(Letters);
      int Size = strlen(Adrr->Board->Vracs[j]);
      Adrr->Board->Vracs[j][Size] = Letter;
      Adrr->Board->Vracs[j][Size + 1] = '\0';
    }
  }
}

bool lanceLeJeu(string joueur0, string joueur1, string Name, bool IA1,
                bool IA2) {
  writeToDebugFile("lanceLeJeu", INFO_DETAIL);
  ForDict *DictHelper = new ForDict;
  // Demander à l'utilisateur de choisir le PATH vers dico (pas pratique)
  // string nomDico = choisirDictionnaire();
  // CreateHelper(DictHelper, nomDico, TAILLES_CLEES_DICO);
  CreateHelper(DictHelper, "Text/DictionnairePurified.txt", TAILLES_CLEES_DICO);

  // Loading bag of letters
  char *LETTERS = createLetters();

  // Stockage des noms
  Names *NamesHelper = new Names;
  NamesHelper->Name1 = joueur0;
  NamesHelper->Name2 = joueur1;
  NamesHelper->NameGame = Name;

  // Loading players variables
  Adresses *Adrr = new Adresses;
  initGame(Adrr);

  SetupVracs(Adrr, LETTERS);

  // Waiting for players to connect
  cout << "Waiting for AIs..." << endl;
  while (!(*Adrr->Connected[0]) or !(*Adrr->Connected[1])) {
    usleep(100000);
  }
  cout << "AIs ready" << endl;

  int Joueur;
  int Tour;
  string mot;

  // writeToDebugFile("Nouvelle partie N°" + to_string(NbPartie));

  // Affiche règle si pas IA
  writeToDebugFile("Affichage des règles", ALL_LOG);
  affichePlateaux(Adrr->Board, NB_LIGNES_PLATEAU, TAILLE_MAX_MOT, Name, joueur0,
                  joueur1, 0, false);
  ActionPossible();
  cout << "Appuyez sur 'enter' pour continuer" << endl;
  getchar();

  Joueur = 0;
  Tour = 0;
  mot = Round(Adrr, Joueur, DictHelper, NamesHelper, Tour, LETTERS);
  exit(0);
  Tour++;
  if (mot == "-STOP-") {
    cout << "Fin du jeu, " << NamesHelper->Name2 << " a gagné." << endl;
    writeToDebugFile("Fin du jeu, " + NamesHelper->Name2 + " a gagné.",
                     INFO_DETAIL);
    return true;
  }

  Joueur = 1 - Joueur;

  mot = Round(Adrr, Joueur, DictHelper, NamesHelper, Tour, LETTERS);
  Tour++;
  if (mot == "-STOP-") {
    cout << "Fin du jeu, " << NamesHelper->Name1 << " a gagné." << endl;
    writeToDebugFile("Fin du jeu, " + NamesHelper->Name1 + " a gagné.",
                     INFO_DETAIL);
    return true;
  }
  writeToDebugFile("Fin du premier tour", ALL_LOG);
  while (!endGame(Adrr->Board->Board, Joueur) and mot != "-STOP-") {
    Joueur = 1 - Joueur;
    affichePlateaux(Adrr->Board, NB_LIGNES_PLATEAU, TAILLE_MAX_MOT, Name,
                    joueur0, joueur1, Joueur, false);
    writeToDebugFile("Début du tour : " + to_string(Tour), ALL_LOG);
    mot = Round(Adrr, Joueur, DictHelper, NamesHelper, Tour, LETTERS);
    Tour++;
    writeToDebugFile("Fin du tour : " + to_string(Tour), ALL_LOG);
  }
  affichePlateaux(Adrr->Board, NB_LIGNES_PLATEAU, TAILLE_MAX_MOT, Name, joueur0,
                  joueur1, 0, false);

  // writeToStatsFile("Vrac: " + Adrr->Board->Vracs[0] + " " +
  //                  Adrr->Board->Vracs[1]);

  writeToDebugFile("Letters : ", ALL_LOG);
  for (auto *i = LETTERS; *i != '\0'; i++) {
    writeToDebugFile(string(1, *i), ALL_LOG);
  }

  writeToDebugFile("Letters size : " + strlen(LETTERS), ALL_LOG);
  writeToDebugFile("Board : ", ALL_LOG);
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 2; j++) {
      cout << "| ";
      for (int k = 0; k < 9; k++) {
        cout << Adrr->Board->Board[j * 8 * 10 + i * 10 + k] << " | ";
      }
      cout << "|||";
    }
    cout << endl;
  }

  int *Points;
  Points = calculPoints(Adrr->Board->Board, TAILLE_MAX_MOT, NB_LIGNES_PLATEAU);

  if (Points[0] == Points[1]) {
    cout << "Fin du jeu, égalité" << endl;
  } else if (Points[0] > Points[1]) {
    cout << "Fin du jeu, " << NamesHelper->Name1 << " a gagné." << endl;
  } else {
    cout << "Fin du jeu, " << NamesHelper->Name2 << " a gagné." << endl;
  }
  writeToDebugFile("lanceLeJeu end", INFO_DETAIL);
  return true;
}
