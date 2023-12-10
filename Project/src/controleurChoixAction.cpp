#include "controleurChoixAction.hpp"

struct ForDict;
struct Play;
struct IA;
struct Names;
struct StorePlayers;
struct StoreLetters;

bool isVowel(string Letter) {
  return (Letter == "A" or Letter == "E" or Letter == "I" or Letter == "O" or
          Letter == "U" or Letter == "Y");
}

bool CheckVowel(vector<string> vectorLetter, int Start, int End) {
  for (int i = Start; i > End; i--) {
    if (isVowel(vectorLetter[i])) {
      return true;
    }
  }
  return false;
}

vector<string> createLetters() {
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
  return vectorLetter;
}

void shuffleBag(StoreLetters *LETTERS, string Rep) {
  for (auto i : Rep) {
    // On remet les lettres dans le sac
    LETTERS->Letters.push_back(string(1, i));
    LETTERS->Len++;
  }
  // On mélange le sac
  random_shuffle(LETTERS->Letters.begin(), LETTERS->Letters.end());
}

string piocheLettre(StoreLetters *LETTERS) {
  if (LETTERS->Len != 0) {
    // Si le sac n'est pas vide
    LETTERS->Len--;
    LETTERS->Letters.erase(LETTERS->Letters.begin() + LETTERS->Len);
    return LETTERS->Letters[LETTERS->Len];
  }
  cout << "Le sac est vide " << endl;
  cout << "Appuyez sur 'enter' pour continuer" << endl;
  getchar();
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

#define NB_LETTRES_ECHANGE 3

tuple<BOARD, bool> echangeLettre(BOARD Board, int Joueur,
                                 StoreLetters *LETTERS) {
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
    }

    nouveauVrac = Retire(Board[Joueur][0], LettresAEchanger);
    if (nouveauVrac == "-") {
      cout << "Vous ne disposez pas d'au moins une de ces lettres." << endl;
      LettresAEchanger = "";
    }
  }

  Board[Joueur][0] = nouveauVrac;
  for (int i = 0; i < NB_LETTRES_ECHANGE; i++) {
    Board[Joueur][0] += piocheLettre(LETTERS);
  }
  shuffleBag(LETTERS, LettresAEchanger);
  return make_tuple(Board, true);
}

BOARD piocheOuEchange(BOARD Board, int Joueur, StoreLetters *LETTERS) {
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
      return Board;
    } else if (RepJoueur == "E") {
      tie(Board, EchangeReussi) = echangeLettre(Board, Joueur, LETTERS);
      if (EchangeReussi) {
        return Board;
      }
    } else {
      cout << "Choix invalide." << endl;
      cout << "    P : Piocher une lettre" << endl;
      cout << "    E : Echanger " << NB_LETTRES_ECHANGE << " lettres" << endl;
    }
  }
}

enum TentativeMotPlace {
  // Etat du mot joué
  EchecPlacementMot,
  ReussitePlacementMot,
  FinTourIA,
};

enum ChoixPossible {
  // Etat du choix de l'action
  PlacerUnMot = 'C',
  TentativeJarnac = 'J',
  FinTour = 'F',
  VoirPlateau = 'P',
  VoirMenu = 'M',
  Abandonner = 'A',
};

tuple<BOARD, TentativeMotPlace> JouerMot(BOARD Board, int Joueur, Play *Current,
                                         Names *NamesHelper,
                                         ForDict *DictHelper,
                                         StoreLetters *LETTERS) {
  if (Current->End) {
    // Si l'IA a fini son tour
    return make_tuple(Board, FinTourIA);
  }

  // Debut check si mot possible
  if (Current->DLetter == "") {
    // Si aucune lettre n'est ajoutée
    affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                    NamesHelper->NameGame, NamesHelper->Name1,
                    NamesHelper->Name2, Joueur, Current->Jarnac);
    cout << "Vous devez ajouter au moins une lettre. Taper 'R' pour "
            "choisir une autre ligne. Taper 'Q' pour changer d'action."
         << endl;
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
        return make_tuple(Board, EchecPlacementMot);
      }
    }
  }
  if (!trouve(Current->Word, DictHelper)) {
    // Si le mot n'est pas dans le dictionnaire
    affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                    NamesHelper->NameGame, NamesHelper->Name1,
                    NamesHelper->Name2, Joueur, Current->Jarnac);
    cout << "Mot inexistant. Taper 'R' pour choisir une autre ligne. "
            "Taper 'Q' pour changer d'action."
         << endl;
    cout << Current->Word << endl;
    return make_tuple(Board, EchecPlacementMot);
  }
  // Fin check si mot possible

  // Ici mots possibles
  if (!(Current->Jarnac)) {
    // Retire du vrac - Ajoute au plateau - Ajoute une lettre au vrac
    Board[Joueur][0] = Retire(Board[Joueur][0], Current->DLetter);
    Board[Joueur][Current->Ligne] = Current->Word;
    Board[Joueur][0] += piocheLettre(LETTERS);
  } else {
    // Retire du vrac de l'adversaire - (Retire du plateau adverse) - Ajoute à
    // notre plateau
    Board[1 - Joueur][0] = Retire(Board[1 - Joueur][0], Current->DLetter);
    if (Current->Origin > 0) {
      Board[1 - Joueur][Current->Origin] = "";
    }
    Board[Joueur][Current->Ligne] = Current->Word;
  }

  writeToDebugFile(Current->Word);

  // affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
  // NamesHelper->NameGame,
  //                 NamesHelper->Name1, NamesHelper->Name2, Joueur,
  //                 Current->Jarnac);
  return make_tuple(Board, ReussitePlacementMot);
}

BOARD PlaceMot(BOARD Board, int Joueur, ForDict *DictHelper, bool isJarnac,
               Names *NamesHelper, StoreLetters *LETTERS) {
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
        JouerMot(Board, Joueur, Current, NamesHelper, DictHelper, LETTERS);
  }
  return Board;
}

tuple<BOARD, string> choixAction(BOARD Board, int Joueur, ForDict *DictHelper,
                                 Names *NamesHelper, int Tour,
                                 StoreLetters *LETTERS) {
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
    if (ChoixUtilisateur[0] == PlacerUnMot) {
      // Si le joueur veut placer un mot
      Board = PlaceMot(Board, Joueur, DictHelper, false, NamesHelper, LETTERS);
      dejaJoue = true;
    } else if (ChoixUtilisateur[0] == TentativeJarnac and Tour != 0) {
      // Si le joueur veut crier Jarnac
      if (dejaJoue) {
        cout << "Vous avez déjà joué un mot." << endl;
      } else {
        Board = PlaceMot(Board, Joueur, DictHelper, true, NamesHelper, LETTERS);
      }
    } else if (ChoixUtilisateur[0] == FinTour) {
      // Si le joueur veut finir son tour
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
      // Si le joueur veut abandonner
      return make_tuple(Board, "-STOP-");
    } else {
      // Si le joueur a fait un choix invalide
      affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                      NamesHelper->NameGame, NamesHelper->Name1,
                      NamesHelper->Name2, Joueur, false);
      cout << "Choix invalide, tapez 'M' pour voir les actions possibles."
           << endl;
    }
  }
  return make_tuple(Board, "");
}

tuple<BOARD, string> Round(BOARD Board, int Joueur, ForDict *DictHelper,
                           Names *NamesHelper, StorePlayers *PlayerHelper,
                           int Tour, StoreLetters *LETTERS) {
  if (!(PlayerHelper->isAI[Joueur])) {
    // Si humain
    if (Tour >= 2) {
      // On ne pioche qu'à partir du 2e tour
      Board = piocheOuEchange(Board, Joueur, LETTERS);
    }
    return choixAction(Board, Joueur, DictHelper, NamesHelper, Tour, LETTERS);
  } else {
    // Si IA
    if (Tour >= 2) {
      // On ne pioche qu'à partir du 2e tour
      Board[Joueur][0] += piocheLettre(LETTERS);
    }

    Play *IAMove = new Play;
    int nbPlay = 0;
    bool JarnacPossible = true;
    TentativeMotPlace state = EchecPlacementMot;
    do {
      // affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
      // NamesHelper->NameGame,
      //                 NamesHelper->Name1, NamesHelper->Name2, Joueur, false);

      if (Tour == 0 or nbPlay > 0) {
        // Verifier si jarnac possible
        JarnacPossible = false;
      }
      IAMove =
          BestMove(Board, Joueur, JarnacPossible, PlayerHelper->AIS[Joueur]);

      nbPlay++;
      tie(Board, state) =
          JouerMot(Board, Joueur, IAMove, NamesHelper, DictHelper, LETTERS);

      // Si l'IA a joué un mot impossible
      if (state == EchecPlacementMot) {
        affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                        NamesHelper->NameGame, NamesHelper->Name1,
                        NamesHelper->Name2, Joueur, IAMove->Jarnac);
        cout << IAMove->Word << endl;
        cout << IAMove->DLetter << endl;
        cout << IAMove->Ligne << endl;
        cout << IAMove->Origin << endl;
        cout << IAMove->Jarnac << endl;

        throw invalid_argument("L'IA a tenté de jouer un mot impossible.");
      }
    } while (state == ReussitePlacementMot);
    return make_tuple(Board, "");
  }
}

bool endGame(BOARD Board, int Joueur) {
  // Si toutes les lignes sont commencées et aucun jarnac n'est possible
  // alors fin de la partie
  for (int j = 1; j < Board[Joueur].size(); j++) {
    if (Board[Joueur][j].length() == 0) {
      return false;
    }
  }
  return true;
}

BOARD initBoard(StoreLetters *LETTERS, int W, int H) {
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
  // CreateHelper(DictHelper, nomDico, TAILLES_CLEES_DICO);
  CreateHelper(DictHelper, "Text/DictionnairePurified.txt", TAILLES_CLEES_DICO);

  // Stockage des noms
  Names *NamesHelper = new Names;
  NamesHelper->Name1 = joueur0;
  NamesHelper->Name2 = joueur1;
  NamesHelper->NameGame = Name;

  // Stockage des IA
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

  BOARD Board;
  StoreLetters *LETTERS = new StoreLetters;
  int NbPartie = 0;
  while (true) {
    // writeToDebugFile("Nouvelle partie N°" + to_string(NbPartie));
    Board = initBoard(LETTERS, NB_LIGNES_PLATEAU, TAILLE_MAX_MOT);

    // Affiche règle si pas IA
    if (!IA1 or !IA2) {
      affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                      Name, joueur0, joueur1, 0, false);
      ActionPossible();
      cout << "Appuyez sur 'enter' pour continuer" << endl;
      getchar();
    }

    string mot;
    int Joueur = 0;
    int Tour = 0;
    tie(Board, mot) = Round(Board, Joueur, DictHelper, NamesHelper,
                            PlayersHelper, Tour, LETTERS);
    Tour++;
    if (mot == "-STOP-") {
      cout << "Fin du jeu, " << NamesHelper->Name2 << " a gagné." << endl;
      return true;
    }

    Joueur = 1 - Joueur;

    tie(Board, mot) = Round(Board, Joueur, DictHelper, NamesHelper,
                            PlayersHelper, Tour, LETTERS);
    Tour++;
    if (mot == "-STOP-") {
      cout << "Fin du jeu, " << NamesHelper->Name1 << " a gagné." << endl;
      return true;
    }

    while (mot == "") {
      Joueur = 1 - Joueur;
      // affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
      // Name, joueur0, joueur1, Joueur,
      //                 false);
      tie(Board, mot) = Round(Board, Joueur, DictHelper, NamesHelper,
                              PlayersHelper, Tour, LETTERS);
      Tour++;

      if (endGame(Board, Joueur)) {
        break;
      }
    }
    affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT, Name,
                    joueur0, joueur1, 0, false);
    cout << "Fin du jeu." << endl;
    writeToDebugFile("Vrac : " + Board[0][0] + " " + Board[1][0]);
  }
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

  lanceLeJeu(j1, j2, "Jarnac", true, true);
  return 0;
}