#include "controleurChoixAction.hpp"

struct ForDict;
struct Play;
struct IA;
struct Names;
struct StorePlayers;

bool isVowel(string Letter) {
  return (Letter == "A" or Letter == "E" or Letter == "I" or Letter == "O" or
          Letter == "U" or Letter == "Y");
}

bool CheckVowel(int Start, int End) {
  for (int i = Start; i > End; i--) {
    if (isVowel(LETTERS[i])) {
      return true;
    }
  }
  return false;
}

vector<string> createLetters() {
  // Toutes les lettres de base du jeu
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
    Vowel1 = CheckVowel(Size, Size - 6);
    Vowel2 = CheckVowel(Size - 7, Size - 12);
  }
  return vectorLetter;
}

vector<string> LETTERS = createLetters();
int LEN = LETTERS.size();

void shuffleBag(string Rep) {
  for (auto i : Rep) {
    LETTERS.push_back(string(1, i));
    LEN++;
  }
  random_shuffle(LETTERS.begin(), LETTERS.end());
}

string piocheLettre() {
  if (LEN != 0) {
    LEN--;
    LETTERS.pop_back();
    return LETTERS[LEN];
  }
  return "";
}

void ActionPossible() {
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
}

tuple<BOARD, bool> echangeLettre(BOARD Board, int Joueur) {
  if (Board[Joueur][0].size() < 3) {
    cout << "Vous n'avez pas assez de lettre pour en échanger." << endl;
    return make_tuple(Board, false);
  }

  string VracEnleve = "";
  string nouveauVrac = "";
  while (VracEnleve == "") {
    cout << "Votre vrac est actuellement : " << Board[Joueur][0] << endl
         << "De quelles lettres voulez-vous vous séparer ?" << endl;
    cin >> VracEnleve;
    VracEnleve = purifie(VracEnleve);
    if (VracEnleve.size() != 3) {
      cout << "Vous devez choisir exactement 3 lettres." << endl;
      VracEnleve = "";
    }

    nouveauVrac = Retire(Board[Joueur][0], VracEnleve);
    if (nouveauVrac == "-") {
      cout << "Vous ne disposez pas d'au moins une de ces lettres." << endl;
      VracEnleve = "";
    }
  }

  Board[Joueur][0] = nouveauVrac;
  for (int i = 0; i < 3; i++) {
    Board[Joueur][0] += piocheLettre();
  }
  shuffleBag(VracEnleve);
  return make_tuple(Board, true);
}

BOARD piocheOuEchange(BOARD Board, int Joueur) {
  string Rep;
  bool state;
  while (true) {
    cout << "Voulez-vous piocher une lettre ou échanger 3 lettres ?" << endl;
    cout << "    P : Piocher une lettre" << endl;
    cout << "    E : Echanger 3 lettres" << endl;
    cin >> Rep;
    Rep = purifie(Rep);
    if (Rep == "P") {
      Board[Joueur][0] += piocheLettre();
      return Board;
    } else if (Rep == "E") {
      tie(Board, state) = echangeLettre(Board, Joueur);
      if (state) {
        return Board;
      }
    } else {
      cout << "Choix invalide." << endl;
      cout << "    P : Piocher une lettre" << endl;
      cout << "    E : Echanger 3 lettres" << endl;
    }
  }
}

tuple<BOARD, int> JouerMot(BOARD Board, int Joueur, Play *Current,
                           Names *NamesHelper, ForDict *DictHelper) {
  if (Current->End) {
    // Si l'IA a fini son tour
    return make_tuple(Board, 2);
  }

  if (Current->DLetter == "") {
    // Si aucune lettre n'est ajoutée
    affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                    NamesHelper->Name1, NamesHelper->Name2, Joueur,
                    Current->Jarnac);
    cout << "Vous devez ajouter au moins une lettre. Taper 'R' pour "
            "choisir une autre ligne. Taper 'Q' pour changer d'action."
         << endl;
    return make_tuple(Board, 0);
  }

  for (auto i : Current->DLetter) {
    // Si la lettre n'est pas dans le vrac
    if (Current->Jarnac) {
      if (Board[1 - Joueur][0].find(i) == string::npos) {
        affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                        NamesHelper->Name1, NamesHelper->Name2, Joueur,
                        Current->Jarnac);
        cout << "Mot impossible à jouer. Taper 'R' pour choisir une autre "
                "ligne. Taper 'Q' pour changer d'action."
             << endl;
        return make_tuple(Board, 0);
      }
    } else {
      if (Board[Joueur][0].find(i) == string::npos) {
        affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                        NamesHelper->Name1, NamesHelper->Name2, Joueur,
                        Current->Jarnac);
        cout << "Mot impossible à jouer. Taper 'R' pour choisir une autre "
                "ligne. Taper 'Q' pour changer d'action."
             << endl;
        return make_tuple(Board, 0);
      }
    }
  }

  if (!trouve(Current->Word, DictHelper)) {
    // Si le mot n'est pas dans le dictionnaire
    affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                    NamesHelper->Name1, NamesHelper->Name2, Joueur,
                    Current->Jarnac);
    cout << "Mot inexistant. Taper 'R' pour choisir une autre ligne. "
            "Taper 'Q' pour changer d'action."
         << endl;
    cout << Current->Word << endl;
    return make_tuple(Board, 0);
  }

  if (!(Current->Jarnac)) {
    // Retire du vrac - Ajoute au plateau - Ajoute une lettre au vrac
    Board[Joueur][0] = Retire(Board[Joueur][0], Current->DLetter);
    Board[Joueur][Current->Ligne] = Current->Word;
    Board[Joueur][0] += piocheLettre();
  } else {
    // Retire du vrac de l'adversaire - (Retire du plateau adverse) - Ajoute à
    // notre plateau
    Board[1 - Joueur][0] = Retire(Board[1 - Joueur][0], Current->DLetter);
    if (Current->Origin > 0) {
      Board[1 - Joueur][Current->Origin] = "";
    }
    Board[Joueur][Current->Ligne] = Current->Word;
  }
  affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                  NamesHelper->Name1, NamesHelper->Name2, Joueur,
                  Current->Jarnac);
  return make_tuple(Board, 1);
}

BOARD PlaceMot(BOARD Board, int Joueur, ForDict *DictHelper, bool isJarnac,
               Names *NamesHelper) {
  string Say = "Sur quelle ligne souhaitez-vous jouer ?";
  if (isJarnac) {
    Say = "Où pensez-vous voir un mot ?";
  }
  bool Joue = true;
  string ligneTxt;
  string mot;
  string CurrentLigne = "";
  string diffLetter;
  string Vrac = Board[Joueur][0];
  bool Continue;
  int NumLigne;

  Play *Current = new Play;
  Current->Jarnac = isJarnac;
  Current->End = false;

  affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                  NamesHelper->Name1, NamesHelper->Name2, Joueur, isJarnac);
  int state = 0;
  while (!state) {
    ligneTxt = "";
    // get line
    while (ligneTxt == "") {
      cout << Say << endl;
      cin >> ligneTxt;
      if (ligneTxt == "Q" or ligneTxt == "q") {
        return Board;
      }
      ligneTxt = purifieNbr(ligneTxt);
      if (ligneTxt.size() != 0) {
        cout << "Numéro de ligne invalide. Tapez 'Q' pour changer d'action."
             << endl;
        ligneTxt = "";
      }
    }

    NumLigne = atoi(ligneTxt.c_str());
    CurrentLigne = Board[Joueur][NumLigne];

    // get word
    cout << endl << "Quel mot souhaitez-vous jouer ?" << endl;
    cin >> mot;
    mot = purifie(mot);
    if (mot == "R") {
      continue;
    }
    if (mot == "Q") {
      return Board;
    }
    diffLetter =
        Retire(mot, CurrentLigne);  // Lettre à ajouter pour jouer ce mot

    // Setup de la structure Play
    Current->Word = mot;
    Current->DLetter = diffLetter;
    if (isJarnac) {
      Current->Ligne = getLine(Board, mot, 1 - Joueur);
      Current->Origin = NumLigne;
    } else {
      Current->Ligne = NumLigne;
      Current->Origin = -1;
    }

    // Jouer le mot
    tie(Board, state) =
        JouerMot(Board, Joueur, Current, NamesHelper, DictHelper);
  }
  return Board;
}

tuple<BOARD, string> choixAction(BOARD Board, int Joueur, ForDict *DictHelper,
                                 Names *NamesHelper, int Tour) {
  string coup;
  bool dejaJoue = false;
  affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                  NamesHelper->Name1, NamesHelper->Name2, Joueur, false);
  while (true) {
    cout << "Quelle action souhaitez-vous effectuer ?" << endl;
    cin >> coup;
    coup = purifie(coup);
    cout << endl;
    if (coup == "C") {
      // Si le joueur veut placer un mot
      Board = PlaceMot(Board, Joueur, DictHelper, false, NamesHelper);
      dejaJoue = true;
    } else if (coup == "J" and Tour != 0) {
      // Si le joueur veut crier Jarnac
      if (dejaJoue) {
        cout << "Vous avez déjà joué un mot." << endl;
      } else {
        Board = PlaceMot(Board, Joueur, DictHelper, true, NamesHelper);
      }
    } else if (coup == "F") {
      // Si le joueur veut finir son tour
      return make_tuple(Board, "");
    } else if (coup == "P") {
      // Si le joueur veut voir le plateau
      affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                      NamesHelper->Name1, NamesHelper->Name2, Joueur, false);
    } else if (coup == "M") {
      // Si le joueur veut voir les actions possibles
      ActionPossible();
    } else if (coup == "A") {
      // Si le joueur veut abandonner
      return make_tuple(Board, "-STOP-");
    } else {
      // Si le joueur a fait un choix invalide
      affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                      NamesHelper->Name1, NamesHelper->Name2, Joueur, false);
      cout << "Choix invalide, tapez 'M' pour voir les actions possibles."
           << endl;
    }
  }
  return make_tuple(Board, "");
}

tuple<BOARD, string> Round(BOARD Board, int Joueur, ForDict *DictHelper,
                           Names *NamesHelper, StorePlayers *PlayerHelper,
                           int Tour) {
  if (!(PlayerHelper->isAI[Joueur])) {
    // Si humain
    if (Tour >= 2) {
      // On ne pioche qu'à partir du 2e tour
      Board = piocheOuEchange(Board, Joueur);
    }
    return choixAction(Board, Joueur, DictHelper, NamesHelper, Tour);
  } else {
    // Si IA
    if (Tour >= 2) {
      // On ne pioche qu'à partir du 2e tour
      Board[Joueur][0] += piocheLettre();
    }

    Play *IAMove = new Play;
    int nbPlay = 0;
    bool JarnacPossible = true;
    int state;
    do {
      // affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
      //                 NamesHelper->Name1, NamesHelper->Name2, Joueur, false);

      if (Tour == 0 or nbPlay > 0) {
        // Verifier si jarnac possible
        JarnacPossible = false;
      }
      IAMove =
          BestMove(Board, Joueur, JarnacPossible, PlayerHelper->AIS[Joueur]);
      nbPlay++;
      tie(Board, state) =
          JouerMot(Board, Joueur, IAMove, NamesHelper, DictHelper);

      // Si l'IA a joué un mot impossible
      if (state == 0) {
        affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                        NamesHelper->Name1, NamesHelper->Name2, Joueur,
                        IAMove->Jarnac);
        cout << IAMove->Word << endl;
        cout << IAMove->DLetter << endl;
        cout << IAMove->Ligne << endl;
        cout << IAMove->Origin << endl;
        cout << IAMove->Jarnac << endl;

        throw invalid_argument("L'IA a tenté de jouer un mot impossible.");
      }
    } while (state == 1);
    return make_tuple(Board, "");
  }
}

bool endGame(BOARD Board, int Joueur) {
  bool AllEmpty = true;
  for (int j = 1; j < Board[Joueur].size(); j++) {
    // Si toutes les lignes sont commencées et aucun jarnac n'est possible
    if (Board[Joueur][j].length() == 0) {
      AllEmpty = false;
      break;
    }
  }
  return AllEmpty;
}

BOARD initBoard(int W, int H) {
  BOARD Board = {{""}, {""}};
  for (int i = 0; i < 2; i++) {
    // Pioche lettre
    for (int j = 0; j < 6; j++) {
      Board[i][0] += piocheLettre();
    }

    // Création du plateau
    for (int j = 1; j < H; j++) {
      Board[i].push_back("");
    }
  }
  return Board;
}

string choisirDictionnaire() {
  string choix;
  cout << "Quel dictionnaire souhaitez-vous choisir" << endl;
  cin >> choix;
  return choix;
}

bool lanceLeJeu(string joueur0, string joueur1, string Name, bool IA1,
                bool IA2) {
  ForDict *DictHelper = new ForDict;
  // string nomDico = choisirDictionnaire();
  // CreateHelper(DictHelper, nomDico, 3);
  CreateHelper(DictHelper, "Text/DictionnairePurified.txt", 3);

  BOARD Board = initBoard(8, 9);

  Names *NamesHelper = new Names;
  NamesHelper->Name1 = joueur0;
  NamesHelper->Name2 = joueur1;
  NamesHelper->NameGame = Name;

  StorePlayers *PlayersHelper = new StorePlayers;
  if (IA1) {
    PlayersHelper->AIS[0] = new AI;
    StartUpAI(PlayersHelper->AIS[0]);
    PlayersHelper->isAI[0] = true;
  }
  if (IA2) {
    PlayersHelper->AIS[1] = new AI;
    StartUpAI(PlayersHelper->AIS[1]);
    PlayersHelper->isAI[1] = true;
  }

  // Affiche règle si pas IA
  if (!IA1 or !IA2) {
    affichePlateaux(Board[0], Board[1], 8, 9, Name, joueur0, joueur1, 0, false);
    ActionPossible();
    cout << "Appuyez sur 'enter' pour continuer" << endl;
    getchar();
  }

  string mot;
  int Joueur = 0;
  int Tour = 0;
  tie(Board, mot) =
      Round(Board, Joueur, DictHelper, NamesHelper, PlayersHelper, Tour);
  Tour++;
  if (mot == "-STOP-") {
    cout << "Fin du jeu, " << NamesHelper->Name2 << " a gagné." << endl;
    return true;
  }

  Joueur = 1 - Joueur;

  tie(Board, mot) =
      Round(Board, Joueur, DictHelper, NamesHelper, PlayersHelper, Tour);
  Tour++;
  if (mot == "-STOP-") {
    cout << "Fin du jeu, " << NamesHelper->Name1 << " a gagné." << endl;
    return true;
  }

  while (mot == "") {
    Joueur = 1 - Joueur;
    affichePlateaux(Board[0], Board[1], 8, 9, Name, joueur0, joueur1, Joueur,
                    false);
    tie(Board, mot) =
        Round(Board, Joueur, DictHelper, NamesHelper, PlayersHelper, Tour);
    Tour++;

    if (endGame(Board, Joueur)) {
      break;
    }
  }

  cout << "Fin du jeu." << endl;

  // if (mot[6] == '0')
  // {
  //     cout << "Fin du jeu, " << NamesHelper->Name2 << " a gagné." << endl;
  // }
  // else
  // {
  //     cout << "Fin du jeu, " << NamesHelper->Name1 << " a gagné." << endl;
  // }
  return true;
}

int main() {
  string j1, j2;
  // cout << "Nom du premier joueur : ";
  // cin >> j1;
  // cout << "Nom du deuxième joueur : ";
  // cin >> j2;

  j1 = "Joueur 0";
  j2 = "Joueur 1";

  // srand(time(NULL));
  // if (rand() % 2) {
  //   string Temp = j1;
  //   j1 = j2;
  //   j2 = Temp;
  // }

  lanceLeJeu(j1, j2, "Jarnac", false, true);
  return 0;
}