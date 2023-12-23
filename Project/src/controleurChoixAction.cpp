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

int getLine(BOARD Board, string Word, int Joueur) {
  for (int i = 1; i < Board[Joueur].size(); i++) {
    if (Sort(Board[Joueur][i]) == Sort(Word)) {
      return i;
    }
  }
  return -1;
}

bool isVowel(string Letter) {
  writeToDebugFile("isVowel", INFO_DETAIL);
  return (Letter == "A" or Letter == "E" or Letter == "I" or Letter == "O" or
          Letter == "U" or Letter == "Y");
}

bool CheckVowel(vector<string> vectorLetter, int Start, int End) {
  writeToDebugFile("CheckVowel", INFO_DETAIL);
  for (int i = Start; i > End; i--) {
    if (isVowel(vectorLetter[i])) {
      return true;
    }
  }
  return false;
}

vector<string> createLetters() {
  writeToDebugFile("createLetters", INFO_DETAIL);
  // Toutes les lettres de base du jeu
  // Les mettre dans un fichier texte ?
  map<string, int> usableLetter = {
      {"A", 14}, {"B", 4},  {"C", 7}, {"D", 5},  {"E", 19}, {"F", 2}, {"G", 4},
      {"H", 2},  {"I", 11}, {"J", 1}, {"K", 1},  {"L", 6},  {"M", 5}, {"N", 9},
      {"O", 8},  {"P", 4},  {"Q", 1}, {"R", 10}, {"S", 7},  {"T", 9}, {"U", 8},
      {"V", 2},  {"W", 1},  {"X", 1}, {"Y", 1},  {"Z", 2}};

  vector<string> vectorLetter = {};
  for (auto &iter : usableLetter) {
    for (int i = 0; i < iter.second; i++) {
      vectorLetter.push_back(iter.first);
    }
  }

  srand(time(NULL));
  bool Vowel1 = false;
  bool Vowel2 = false;
  int Size = vectorLetter.size() - 1;

  while (!Vowel1 || !Vowel2) {
    random_shuffle(vectorLetter.begin(), vectorLetter.end());

    // Verifier s'il y a bien une voyelle dans le vrac de chaque joueur
    Vowel1 = CheckVowel(vectorLetter, Size, Size - TAILLE_VRAC);
    Vowel2 = CheckVowel(vectorLetter, Size - 1 - TAILLE_VRAC,
                        Size - 2 * TAILLE_VRAC);
  }
  writeToDebugFile("createLetters end", INFO_DETAIL);
  return vectorLetter;
}

void shuffleBag(StoreLetters *LETTERS, string Rep) {
  writeToDebugFile("shuffleBag", INFO_DETAIL);
  for (auto i : Rep) {
    // On remet les lettres dans le sac
    LETTERS->Letters.push_back(string(1, i));
    LETTERS->Len++;
  }
  // On mélange le sac
  random_shuffle(LETTERS->Letters.begin(), LETTERS->Letters.end());
  writeToDebugFile("shuffleBag end", INFO_DETAIL);
}

string piocheLettre(StoreLetters *LETTERS) {
  writeToDebugFile("piocheLettre", INFO_DETAIL);
  if (LETTERS->Len != 0) {
    // Si le sac n'est pas vide
    LETTERS->Len--;
    LETTERS->Letters.erase(LETTERS->Letters.begin() + LETTERS->Len);
    writeToDebugFile("piocheLettre end letter", INFO_DETAIL);
    return LETTERS->Letters[LETTERS->Len];
  }
  cout << "Le sac est vide " << endl;
  cout << "Appuyez sur 'enter' pour continuer" << endl;
  // getchar();
  writeToDebugFile("piocheLettre end no letter", INFO_DETAIL);
  return "";
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

tuple<BOARD, bool> echangeLettre(BOARD Board, int Joueur,
                                 StoreLetters *LETTERS) {
  writeToDebugFile("echangeLettre", INFO_DETAIL);
  if (Board[Joueur][0].size() < NB_LETTRES_ECHANGE) {
    cout << "Vous n'avez pas assez de lettre pour en échanger." << endl;
    return make_tuple(Board, false);
  }

  string LettresAEchanger = "";
  string nouveauVrac = "";
  while (LettresAEchanger == "") {
    cout << "Votre vrac est actuellement : " << Board[Joueur][0] << endl
         << "De quelles lettres voulez-vous vous séparer ?" << endl;
    cin >> LettresAEchanger;
    LettresAEchanger = purifie(LettresAEchanger);
    if (LettresAEchanger.size() != NB_LETTRES_ECHANGE) {
      cout << "Vous devez choisir exactement " << NB_LETTRES_ECHANGE
           << " lettre(s)." << endl;
      LettresAEchanger = "";
      writeToDebugFile("LettresAEchanger.size() != NB_LETTRES_ECHANGE",
                       ALL_LOG);
    }

    nouveauVrac = retire(Board[Joueur][0], LettresAEchanger);
    if (nouveauVrac == "-") {
      cout << "Vous ne disposez pas d'au moins une de ces lettres." << endl;
      LettresAEchanger = "";
      writeToDebugFile("Choix de lettres invalides", ALL_LOG);
    }
  }

  Board[Joueur][0] = nouveauVrac;
  for (int i = 0; i < NB_LETTRES_ECHANGE; i++) {
    Board[Joueur][0] += piocheLettre(LETTERS);
  }
  shuffleBag(LETTERS, LettresAEchanger);
  writeToDebugFile("echangeLettre end", INFO_DETAIL);
  return make_tuple(Board, true);
}

BOARD piocheOuEchange(BOARD Board, int Joueur, StoreLetters *LETTERS) {
  writeToDebugFile("piocheOuEchange", INFO_DETAIL);
  string RepJoueur;
  bool EchangeReussi;
  while (true) {
    cout << "Voulez-vous piocher une lettre ou échanger " << NB_LETTRES_ECHANGE
         << " lettres ?" << endl;
    cout << "    P : Piocher une lettre" << endl;
    cout << "    E : Echanger " << NB_LETTRES_ECHANGE << " lettres" << endl;
    cin >> RepJoueur;
    RepJoueur = purifie(RepJoueur);
    if (RepJoueur == "P") {
      Board[Joueur][0] += piocheLettre(LETTERS);
      writeToDebugFile("piocheOuEchange end avec pioche", INFO_DETAIL);
      return Board;
    } else if (RepJoueur == "E") {
      tie(Board, EchangeReussi) = echangeLettre(Board, Joueur, LETTERS);
      if (EchangeReussi) {
        writeToDebugFile("piocheOuEchange end avec echange", INFO_DETAIL);
        return Board;
      } else {
        writeToDebugFile("Echange raté", ALL_LOG);
      }
    } else {
      cout << "Choix invalide." << endl;
      writeToDebugFile("Choix invalide", ALL_LOG);
    }
  }
}

tuple<BOARD, TentativeMotPlace> JouerMot(BOARD Board, int Joueur, Play *Current,
                                         Names *NamesHelper,
                                         ForDict *DictHelper,
                                         StoreLetters *LETTERS) {
  writeToDebugFile("JouerMot", INFO_DETAIL);
  if (Current->End) {
    writeToDebugFile("JouerMot end avec FinTourIA", INFO_DETAIL);
    // Si l'IA a fini son tour
    return make_tuple(Board, FinTourIA);
  }

  // Debut check si mot possible
  writeToDebugFile("Debut test", ERROR);
  if (Current->DLetter == "-") {
    // Si aucune lettre n'est ajoutée
    affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                    NamesHelper->NameGame, NamesHelper->Name1,
                    NamesHelper->Name2, Joueur, Current->Jarnac);
    cout << "Vous devez ajouter au moins une lettre. Taper 'R' pour "
            "choisir une autre ligne. Taper 'Q' pour changer d'action."
         << endl;
    writeToDebugFile("JouerMot end avec aucune lettre ajoutée", INFO_DETAIL);
    return make_tuple(Board, EchecPlacementMot);
  }
  for (auto i : Current->DLetter) {
    // Si la lettre n'est pas dans le vrac
    if (Current->Jarnac) {
      if (Board[1 - Joueur][0].find(i) == string::npos) {
        affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                        NamesHelper->NameGame, NamesHelper->Name1,
                        NamesHelper->Name2, Joueur, Current->Jarnac);
        cout << "Mot impossible à jouer. Taper 'R' pour choisir une autre "
                "ligne. Taper 'Q' pour changer d'action."
             << endl;
        writeToDebugFile("JouerMot end avec pas les bonnes lettres Jarnac",
                         INFO_DETAIL);
        return make_tuple(Board, EchecPlacementMot);
      }
    } else {
      if (Board[Joueur][0].find(i) == string::npos) {
        affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                        NamesHelper->NameGame, NamesHelper->Name1,
                        NamesHelper->Name2, Joueur, Current->Jarnac);
        cout << "Mot impossible à jouer. Taper 'R' pour choisir une autre "
                "ligne. Taper 'Q' pour changer d'action."
             << endl;
        writeToDebugFile("JouerMot end avec pas les bonnes lettres",
                         INFO_DETAIL);
        return make_tuple(Board, EchecPlacementMot);
      }
    }
  }
  if (!trouve(Current->Word, DictHelper)) {
    // Si le mot n'est pas dans le dictionnaire
    affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                    NamesHelper->NameGame, NamesHelper->Name1,
                    NamesHelper->Name2, Joueur, Current->Jarnac);
    cout << "Mot inexistant. Taper 'R' pour choisir une autre "
            "ligne. "
            "Taper 'Q' pour changer d'action."
         << endl;
    writeToDebugFile("Mot joué : " + Current->Word, INFO);
    writeToDebugFile("JouerMot end avec mot inexistant", INFO_DETAIL);
    return make_tuple(Board, EchecPlacementMot);
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
    writeToDebugFile(
        "Board[Joueur][Current->Ligne] : " + Board[Joueur][Current->Ligne],
        ERROR);
    writeToDebugFile("Current->Word : " + Current->Word, ERROR);
    // Retire du vrac - Ajoute au plateau - Ajoute une lettre au vrac
    Board[Joueur][0] = retire(Board[Joueur][0], Current->DLetter);
    Board[Joueur][Current->Ligne] = Current->Word;
    Board[Joueur][0] += piocheLettre(LETTERS);
  } else {
    // Retire du vrac de l'adversaire - (Retire du plateau adverse) - Ajoute à
    // notre plateau
    Board[1 - Joueur][0] = retire(Board[1 - Joueur][0], Current->DLetter);
    if (Current->Origin > 0) {
      Board[1 - Joueur][Current->Origin] = "";
    }
    Board[Joueur][Current->Ligne] = Current->Word;
  }

  affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                  NamesHelper->NameGame, NamesHelper->Name1, NamesHelper->Name2,
                  Joueur, Current->Jarnac);
  writeToDebugFile("JouerMot end avec mot joué : " + Current->Word,
                   INFO_DETAIL);
  return make_tuple(Board, ReussitePlacementMot);
}

BOARD PlaceMot(BOARD Board, int Joueur, ForDict *DictHelper, bool isJarnac,
               Names *NamesHelper, StoreLetters *LETTERS) {
  writeToDebugFile("PlaceMot", INFO_DETAIL);
  string Say = "Sur quelle ligne souhaitez-vous jouer ?";
  if (isJarnac) {
    Say = "Où pensez-vous voir un mot ?";
  }
  string ligneTxt;
  string mot;
  string CurrentLigne = "";
  string diffLetter;
  string Vrac = Board[Joueur][0];
  int NumLigne;

  Play *Current = new Play;
  Current->Jarnac = isJarnac;
  Current->End = false;

  affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                  NamesHelper->NameGame, NamesHelper->Name1, NamesHelper->Name2,
                  Joueur, isJarnac);
  TentativeMotPlace state = EchecPlacementMot;
  while (state == EchecPlacementMot) {
    ligneTxt = "";
    // get line
    while (ligneTxt == "") {
      cout << Say << endl;
      cin >> ligneTxt;
      if (ligneTxt == "Q" or ligneTxt == "q") {
        writeToDebugFile("PlaceMot end avec Q", INFO_DETAIL);
        return Board;
      }
      ligneTxt = purifieNbr(ligneTxt);
      if (ligneTxt.size() == 0) {
        cout << "Numéro de ligne invalide. Tapez 'Q' pour "
                "changer d'action."
             << endl;
        ligneTxt = "";
        writeToDebugFile("Numéro de ligne invalide", ALL_LOG);
      }
    }

    NumLigne = atoi(ligneTxt.c_str());
    if (isJarnac) {
      CurrentLigne = Board[1 - Joueur][NumLigne];
    } else {
      CurrentLigne = Board[Joueur][NumLigne];
    }
    // get word
    cout << endl << "Quel mot souhaitez-vous jouer ?" << endl;
    cin >> mot;
    mot = purifie(mot);
    if (mot == "R") {
      writeToDebugFile("PlaceMot restart avec R", ALL_LOG);
      continue;
    }
    if (mot == "Q") {
      writeToDebugFile("PlaceMot end avec Q", INFO_DETAIL);
      return Board;
    }
    diffLetter =
        retire(mot, CurrentLigne);  // Lettre à ajouter pour jouer ce mot

    writeToDebugFile("diffLetter : " + diffLetter, ERROR);

    // Setup de la structure Play
    Current->Word = mot;
    Current->DLetter = diffLetter;
    if (isJarnac) {
      Current->Ligne = getLine(Board, "", Joueur);
      Current->Origin = NumLigne;
    } else {
      Current->Ligne = NumLigne;
      Current->Origin = -1;
    }

    // Jouer le mot
    tie(Board, state) =
        JouerMot(Board, Joueur, Current, NamesHelper, DictHelper, LETTERS);
  }
  writeToDebugFile("PlaceMot end", INFO_DETAIL);
  return Board;
}

tuple<BOARD, string> choixAction(BOARD Board, int Joueur, ForDict *DictHelper,
                                 Names *NamesHelper, int Tour,
                                 StoreLetters *LETTERS) {
  writeToDebugFile("choixAction", INFO_DETAIL);
  string ChoixUtilisateur;
  bool dejaJoue = false;
  affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                  NamesHelper->NameGame, NamesHelper->Name1, NamesHelper->Name2,
                  Joueur, false);
  while (true) {
    cout << "Quelle action souhaitez-vous effectuer ?" << endl;
    cin >> ChoixUtilisateur;
    ChoixUtilisateur = purifie(ChoixUtilisateur);
    cout << endl;
    writeToDebugFile("ChoixUtilisateur : " + ChoixUtilisateur, ALL_LOG);
    if (ChoixUtilisateur[0] == PlacerUnMot) {
      // Si le joueur veut placer un mot
      Board = PlaceMot(Board, Joueur, DictHelper, false, NamesHelper, LETTERS);
      dejaJoue = true;
    } else if (ChoixUtilisateur[0] == TentativeJarnac and Tour != 0) {
      // Si le joueur veut crier Jarnac
      if (dejaJoue) {
        writeToDebugFile("Joueur a déjà joué un mot", ALL_LOG);
        cout << "Vous avez déjà joué un mot." << endl;
      } else {
        Board = PlaceMot(Board, Joueur, DictHelper, true, NamesHelper, LETTERS);
      }
    } else if (ChoixUtilisateur[0] == FinTour) {
      // Si le joueur veut finir son tour
      writeToDebugFile("choixAction end avec FinTour", INFO_DETAIL);
      return make_tuple(Board, "");
    } else if (ChoixUtilisateur[0] == VoirPlateau) {
      // Si le joueur veut voir le plateau
      affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                      NamesHelper->NameGame, NamesHelper->Name1,
                      NamesHelper->Name2, Joueur, false);
    } else if (ChoixUtilisateur[0] == VoirMenu) {
      // Si le joueur veut voir les actions possibles
      ActionPossible();
    } else if (ChoixUtilisateur[0] == Abandonner) {
      writeToDebugFile("choixAction end avec Abandonner", INFO_DETAIL);
      // Si le joueur veut abandonner
      return make_tuple(Board, "-STOP-");
    } else {
      // Si le joueur a fait un choix invalide
      affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                      NamesHelper->NameGame, NamesHelper->Name1,
                      NamesHelper->Name2, Joueur, false);
      cout << "Choix invalide, tapez 'M' pour voir les actions "
              "possibles."
           << endl;
    }
  }
  writeToDebugFile("choixAction end", INFO_DETAIL);
  return make_tuple(Board, "");
}

tuple<BOARD, string> Round(BOARD Board, int Joueur, ForDict *DictHelper,
                           Names *NamesHelper, StorePlayers *PlayerHelper,
                           int Tour, StoreLetters *LETTERS) {
  writeToDebugFile("Round", INFO_DETAIL);
  if (!(PlayerHelper->isAI[Joueur])) {
    writeToDebugFile("Humain", ALL_LOG);
    // Si humain
    if (Tour >= 2) {
      // On ne pioche qu'à partir du 2e tour
      Board = piocheOuEchange(Board, Joueur, LETTERS);
    }
    writeToDebugFile("Round end", INFO_DETAIL);
    return choixAction(Board, Joueur, DictHelper, NamesHelper, Tour, LETTERS);
  } else {
    writeToDebugFile("IA", ALL_LOG);
    // Si IA
    Play *IAMove = new Play;
    TentativeMotPlace state = EchecPlacementMot;

    if (Tour > 0) {
      PlayerHelper->AIJarnac = true;
      do {
        PlayerHelper->AIPlay[Joueur] = true;
        while (PlayerHelper->AIPlay[Joueur]) {
          usleep(100000);
        }
        // IAMove = BestMove(Board, Joueur, true, PlayerHelper->AIS[Joueur]);
        tie(Board, state) = JouerMot(Board, Joueur, PlayerHelper->IAMove,
                                     NamesHelper, DictHelper, LETTERS);
      } while (state == ReussitePlacementMot);
    }

    if (Tour >= 2) {
      PlayerHelper->AIExchange[Joueur] = true;
      while (PlayerHelper->AIExchange[Joueur]) {
        usleep(100000);
      }
      // On ne pioche qu'à partir du 2e tour
      // string LettreIA = PiocheEchange(Board, Joueur);
      if (PlayerHelper->AILetters[0] == '\0') {
        Board[Joueur][0] += piocheLettre(LETTERS);
      } else {
        shuffleBag(LETTERS, PlayerHelper->AILetters);
      }
    }
    PlayerHelper->AIJarnac = false;
    do {
      // affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
      //                 NamesHelper->NameGame, NamesHelper->Name1,
      //                 NamesHelper->Name2, Joueur, false);

      PlayerHelper->AIPlay[Joueur] = true;
      while (PlayerHelper->AIPlay[Joueur]) {
        usleep(100000);
      }
      // IAMove = BestMove(Board, Joueur, true, PlayerHelper->AIS[Joueur]);
      tie(Board, state) = JouerMot(Board, Joueur, PlayerHelper->IAMove,
                                   NamesHelper, DictHelper, LETTERS);

      // Si l'IA a joué un mot impossible
      if (state == EchecPlacementMot) {
        affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                        NamesHelper->NameGame, NamesHelper->Name1,
                        NamesHelper->Name2, Joueur, IAMove->Jarnac);
        writeToDebugFile("Mot joué : " + IAMove->Word, ERROR);
        writeToDebugFile("Lettres jouées : " + IAMove->DLetter, ERROR);
        writeToDebugFile("Ligne jouée : " + to_string(IAMove->Ligne), ERROR);
        writeToDebugFile("Ligne d'origine : " + to_string(IAMove->Origin),
                         ERROR);
        writeToDebugFile("Jarnac : " + to_string(IAMove->Jarnac), ERROR);
        writeToDebugFile("Fin du jeu IA a joué mot impossible", ERROR);
        throw invalid_argument("L'IA a tenté de jouer un mot impossible.");
      }
    } while (state == ReussitePlacementMot);
    writeToDebugFile("Round end", INFO_DETAIL);
    return make_tuple(Board, "");
  }
}

bool endGame(BOARD Board, int Joueur) {
  writeToDebugFile("endGame", INFO_DETAIL);
  // Si toutes les lignes sont commencées et aucun jarnac n'est possible
  // alors fin de la partie
  for (int j = 1; j < Board[Joueur].size(); j++) {
    if (Board[Joueur][j].length() == 0) {
      writeToDebugFile("endGame end avec false", INFO_DETAIL);
      return false;
    }
  }
  writeToDebugFile("endGame end avec true", INFO_DETAIL);
  return true;
}

BOARD initBoard(StoreLetters *LETTERS, int W, int H) {
  writeToDebugFile("initBoard", INFO_DETAIL);
  // initialisation de la pioche de lettre pour les 2 joueurs
  LETTERS->Letters = createLetters();
  LETTERS->Len = LETTERS->Letters.size();
  // initialisation du plateau
  BOARD Board = {{""}, {""}};
  for (int i = 0; i < NBJOUEUR; i++) {  // Pour chaque joueur
    // Pioche lettre
    for (int j = 0; j < TAILLE_VRAC; j++) {
      Board[i][0] += piocheLettre(LETTERS);
    }

    // Création des lignes
    for (int j = 1; j < H; j++) {
      Board[i].push_back("");
    }
  }
  writeToDebugFile("initBoard end", INFO_DETAIL);
  return Board;
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

bool lanceLeJeu(string joueur0, string joueur1, string Name, bool IA1,
                bool IA2) {
  writeToDebugFile("lanceLeJeu", INFO_DETAIL);
  ForDict *DictHelper = new ForDict;
  // Demander à l'utilisateur de choisir le PATH vers dico (pas pratique)
  // string nomDico = choisirDictionnaire();
  // CreateHelper(DictHelper, nomDico, TAILLES_CLEES_DICO);
  CreateHelper(DictHelper, "Text/DictionnairePurified.txt", TAILLES_CLEES_DICO);

  // Stockage des noms
  Names *NamesHelper = new Names;
  NamesHelper->Name1 = joueur0;
  NamesHelper->Name2 = joueur1;
  NamesHelper->NameGame = Name;

  // Stockage des IA

  int adrrAdrr = shmget(0x1233, sizeof(Adresses), 0666 | IPC_CREAT);
  cout << adrrAdrr << endl;
  Adresses *adrr = (Adresses *)shmat(adrrAdrr, NULL, 0);

  StorePlayers *PlayersHelper = new StorePlayers;
  if (IA1) {
    writeToDebugFile("IA1 loading", ALL_LOG);
    // PlayersHelper->AIS[0] = new AI;
    // StartUpAI(PlayersHelper->AIS[0]);
    PlayersHelper->isAI[0] = true;
    int adrrPlay = shmget(0x1234, sizeof(bool), 0666 | IPC_CREAT);
    adrr->adrrPlay[0] = adrrPlay;
    PlayersHelper->AIPlay[0] = (bool *)shmat(adrrPlay, 0, 0);
    PlayersHelper->AIPlay[0] = false;
    int adrrExchange = shmget(0x1235, sizeof(bool *), 0666 | IPC_CREAT);
    adrr->adrrExchange[0] = adrrExchange;
    PlayersHelper->AIExchange[0] = (bool *)shmat(adrrExchange, 0, 0);
    PlayersHelper->AIExchange[0] = false;
    adrr->Ready[0] = false;
  }
  if (IA2) {
    writeToDebugFile("IA2 loading", ALL_LOG);
    // PlayersHelper->AIS[1] = new AI;
    // StartUpAI(PlayersHelper->AIS[1]);
    PlayersHelper->isAI[1] = true;
    int adrrPlay = shmget(0x1236, sizeof(bool *), 0666 | IPC_CREAT);
    adrr->adrrPlay[1] = adrrPlay;
    PlayersHelper->AIPlay[1] = (bool *)shmat(adrrPlay, 0, 0);
    PlayersHelper->AIPlay[1] = false;
    int adrrExchange = shmget(0x1237, sizeof(bool *), 0666 | IPC_CREAT);
    adrr->adrrExchange[1] = adrrExchange;
    PlayersHelper->AIExchange[1] = (bool *)shmat(adrrExchange, 0, 0);
    PlayersHelper->AIExchange[1] = false;
    adrr->Ready[1] = false;
  }
  if (IA1 or IA2) {
    int adrrJarnac = shmget(0x1238, sizeof(bool *), 0666 | IPC_CREAT);
    adrr->adrrJarnac = adrrJarnac;
    PlayersHelper->AIJarnac = (bool *)shmat(adrrJarnac, 0, 0);
    PlayersHelper->AIJarnac = false;
    int adrrLetters = shmget(0x1239, 3, 0666 | IPC_CREAT);
    adrr->adrrLetters = adrrLetters;
    PlayersHelper->AILetters = (char *)shmat(adrrLetters, NULL, 0);
    PlayersHelper->AILetters[0] = '\0';
    int adrrPlayStruct = shmget(0x123A, sizeof(Play), 0666 | IPC_CREAT);
    adrr->adrrPlayMove = adrrPlayStruct;
    PlayersHelper->IAMove = (Play *)shmat(adrrPlayStruct, NULL, 0);
  }

  cout << "Waiting for AIs..." << endl;
  while (!adrr->Ready[0] or !adrr->Ready[1]) {
    usleep(100000);
  }
  cout << "AIs ready" << endl;
  exit(1);
  BOARD Board;
  StoreLetters *LETTERS;
  int NbPartie = 0;
  int Joueur;
  int Tour;
  string mot;
  writeToDebugFile("Nouvelle partie N°" + to_string(NbPartie), INFO);
  LETTERS = new StoreLetters;
  // writeToDebugFile("Nouvelle partie N°" + to_string(NbPartie));
  Board = initBoard(LETTERS, NB_LIGNES_PLATEAU, TAILLE_MAX_MOT);

  // Affiche règle si pas IA
  if (!IA1 or !IA2) {
    writeToDebugFile("Affichage des règles", ALL_LOG);
    affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT, Name,
                    joueur0, joueur1, 0, false);
    ActionPossible();
    cout << "Appuyez sur 'enter' pour continuer" << endl;
    getchar();
  }

  Joueur = 0;
  Tour = 0;
  tie(Board, mot) = Round(Board, Joueur, DictHelper, NamesHelper, PlayersHelper,
                          Tour, LETTERS);
  Tour++;
  if (mot == "-STOP-") {
    cout << "Fin du jeu, " << NamesHelper->Name2 << " a gagné." << endl;
    writeToDebugFile("Fin du jeu, " + NamesHelper->Name2 + " a gagné.",
                     INFO_DETAIL);
    return true;
  }

  Joueur = 1 - Joueur;

  tie(Board, mot) = Round(Board, Joueur, DictHelper, NamesHelper, PlayersHelper,
                          Tour, LETTERS);
  Tour++;
  if (mot == "-STOP-") {
    cout << "Fin du jeu, " << NamesHelper->Name1 << " a gagné." << endl;
    writeToDebugFile("Fin du jeu, " + NamesHelper->Name1 + " a gagné.",
                     INFO_DETAIL);
    return true;
  }
  writeToDebugFile("Fin du premier tour", ALL_LOG);
  while (!endGame(Board, Joueur) and mot != "-STOP-") {
    Joueur = 1 - Joueur;
    affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT, Name,
                    joueur0, joueur1, Joueur, false);
    writeToDebugFile("Début du tour : " + to_string(Tour), ALL_LOG);
    tie(Board, mot) = Round(Board, Joueur, DictHelper, NamesHelper,
                            PlayersHelper, Tour, LETTERS);
    Tour++;
    writeToDebugFile("Fin du tour : " + to_string(Tour), ALL_LOG);
  }
  cout << "Fin de la partie N°" << NbPartie << endl;
  writeToDebugFile("Fin de la partie N°" + to_string(NbPartie), 2);
  affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT, Name,
                  joueur0, joueur1, 0, false);

  writeToStatsFile("Vrac: " + Board[0][0] + " " + Board[1][0]);

  writeToDebugFile("Letters : ", ALL_LOG);
  for (auto i : LETTERS->Letters) {
    writeToDebugFile(i, ALL_LOG);
  }
  writeToDebugFile("Letters size : " + to_string(LETTERS->Len), ALL_LOG);
  writeToDebugFile("Board : ", ALL_LOG);
  for (auto i : Board) {
    for (auto j : i) {
      writeToDebugFile(j, ALL_LOG);
    }
  }
  int Point1 = calculPoints(Board[0], TAILLE_MAX_MOT, NB_LIGNES_PLATEAU);
  int Point2 = calculPoints(Board[1], TAILLE_MAX_MOT, NB_LIGNES_PLATEAU);

  if (Point1 == Point2) {
    cout << "Fin du jeu, égalité" << endl;
  } else if (Point1 > Point2) {
    cout << "Fin du jeu, " << NamesHelper->Name1 << " a gagné." << endl;
  } else {
    cout << "Fin du jeu, " << NamesHelper->Name2 << " a gagné." << endl;
  }
  writeToDebugFile("lanceLeJeu end", INFO_DETAIL);
  return true;
}
