#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

#include "Debug.hpp"
#include "loadIA.hpp"
#include "mAnagrammes.hpp"
#include "mDictionnaire.hpp"
#include "vueEnModeTexte.hpp"

struct ForDict;
struct Play;
struct IA;

using BOARD = vector<vector<string>>;

struct Names {
  string Name1;
  string Name2;
  string NameGame;
};

struct StorePlayers {
  bool isAI[2] = {false, false};
  AI *AIS[2];
};

/**
 * Checks if a letter is a vowel.
 *
 * @param Letter The letter to check.
 * @return True if Letter is 'A', 'E', 'I', 'O', 'U' or 'Y', false otherwise.
 */
bool isVowel(string Letter) {
  return (Letter == "A" or Letter == "E" or Letter == "I" or Letter == "O" or
          Letter == "U" or Letter == "Y");
}

/**
 * Creates a vector containing all the letters to use for the game,
 * with letter frequencies based on Scrabble letter distributions.
 *
 * Initializes a map with the frequency of each letter.
 * Populates a vector with the appropriate number of each letter.
 * Shuffles the vector while ensuring at least one vowel in the first
 * and second halves.
 * Returns the shuffled vector of letters.
 */
vector<string> createLetters() {
  map<string, int> usableLetter = {
      {"A", 14}, {"B", 4},  {"C", 7}, {"D", 5},  {"E", 19}, {"F", 2}, {"G", 4},
      {"H", 2},  {"I", 11}, {"J", 1}, {"K", 1},  {"L", 6},  {"M", 5}, {"N", 9},
      {"O", 8},  {"P", 4},  {"Q", 1}, {"R", 10}, {"S", 7},  {"T", 9}, {"U", 8},
      {"V", 2},  {"W", 1},  {"X", 1}, {"Y", 1},  {"Z", 2}};

  vector<string> Rep = {};
  for (auto &iter : usableLetter) {
    for (int i = 0; i < iter.second; i++) {
      Rep.push_back(iter.first);
    }
  }

  srand(time(NULL));
  bool Vowel1 = false;
  bool Vowel2 = false;
  int Size = Rep.size() - 1;

  while (!Vowel1 || !Vowel2) {
    random_shuffle(Rep.begin(), Rep.end());

    // faire fonction
    for (int i = Size; i > Size - 6; i--) {
      if (isVowel(Rep[i])) {
        Vowel1 = true;
        break;
      }
      Vowel1 = false;
    }
    for (int i = Size - 7; i > Size - 12; i--) {
      if (isVowel(Rep[i])) {
        Vowel2 = true;
        break;
      }
      Vowel2 = false;
    }
  }
  return Rep;
}

vector<string> LETTERS = createLetters();
int LEN = LETTERS.size();

/**
 * Prompts the user to choose a dictionary file to use for the game.
 *
 * Prints a message asking the user which dictionary they want to choose.
 * Reads the user's input into a string variable.
 * Returns the user's dictionary choice as a string.
 */
string choisirDictionnaire() {
  string choix;
  cout << "Quel dictionnaire souhaitez-vous choisir" << endl;
  cin >> choix;
  return choix;
}

/**
 * Pops the last letter from the global LETTERS vector
 * containing all letters left in the bag.
 * Decrements the global LEN counter tracking remaining letters.
 * Returns the popped letter to the caller.
 */
string piocheLettre() {
  if (LEN != 0) {
    LEN--;
    LETTERS.pop_back();
    return LETTERS[LEN];
  }
  return "";
}

/**
 * Shuffles the global LETTERS vector containing all letters in the bag.
 *
 * Takes the string Rep containing the letters to add back to the bag.
 * For each letter in Rep:
 *   - Adds the letter back to the LETTERS vector
 *   - Increments the global LEN counter tracking number of letters
 * After adding all letters back, shuffles the LETTERS vector randomly.
 */
void shuffleBag(string Rep) {
  for (auto i : Rep) {
    LETTERS.push_back(string(1, i));
    LEN++;
  }
  random_shuffle(LETTERS.begin(), LETTERS.end());
}

// DOESN'T WORK BUT BORING
/**
 * Exchanges 3 letters from the player's letter rack with new letters.
 *
 * Checks if the player has at least 3 letters.
 * Prompts the player to choose 3 letters to exchange.
 * Validates the 3 letters chosen.
 * Removes the 3 letters from the player's rack.
 * Adds 3 new random letters to the player's rack.
 * Returns the updated board state.
 */
// structure pour echanger des lettres
BOARD echangeLettre(BOARD Board, int Joueur) {
  if (Board[Joueur][0].size() < 3) {
    cout << "Vous n'avez pas assez de lettre pour en échanger." << endl;
    return Board;
  }
  string Rep = "";
  string Res;
  while (Rep == "") {
    cout << "Votre vrac est actuellement : " << Board[Joueur][0] << endl
         << "De quelleq lettreq voulez-vous vous séparer ?" << endl;
    cin >> Rep;
    Rep = purifie(Rep);
    if (Rep.size() != 3) {
      cout << "Vous devez choisir exactement 3 lettres." << endl;
      Rep = "";
    }

    Res = retire(Board[Joueur][0], Rep);
    if (Res == "-") {
      cout << "Vous ne disposez pas d'au moins une de ces lettres." << endl;
      Rep = "";
    }
  }
  Board[Joueur][0] = Res;
  for (int i = 0; i < 3; i++) {
    Board[Joueur][0] += piocheLettre();
  }
  shuffleBag(Rep);
  return Board;
}

/**
 * Prints a message to the console showing the player their possible actions
 * during the game. Actions include abandoning, continuing, exchanging letters,
 * finishing turn, challenging Jarnac, viewing menu, and viewing board.
 */
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

/**
 * Places a word on the board for the given player.
 *
 * Updates the player's letter rack, board state, and scores.
 * Handles normal play or Jarnac (challenging opponent's word).
 *
 * @param Board The current board state
 * @param Joueur The current player index
 * @param Ligne The board row to place the word on
 * @param Mot The word to be placed
 * @param diffLetter The letters used from the player's rack
 * @param isJarnac True if this is a Jarnac challenge
 * @return The updated board state
 */
tuple<BOARD, int> JouerMot(BOARD Board, int Joueur, Play *Current,
                           Names *NamesHelper, ForDict *DictHelper) {
  // writeToDebugFile("Entrée dans fct JouerMot");

  if (Current->End) {
    // cout << "Fin du tour de l'IA" << endl;
    // writeToDebugFile("Fin du tour de l'IA");
    return make_tuple(Board, 2);
  }

  if (Current->DLetter == "") {
    affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                    NamesHelper->Name1, NamesHelper->Name2, Joueur,
                    Current->Jarnac);
    cout << "Vous devez ajouter au moins une lettre. Taper 'R' pour "
            "choisir une autre ligne. Taper 'Q' pour changer d'action."
         << endl;
    return make_tuple(Board, 0);
  }

  for (auto i : Current->DLetter) {
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
    affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                    NamesHelper->Name1, NamesHelper->Name2, Joueur,
                    Current->Jarnac);
    cout << "Mot inexistant. Taper 'R' pour choisir une autre ligne. "
            "Taper 'Q' pour changer d'action."
         << endl;
    cout << Current->Word << endl;
    return make_tuple(Board, 0);
  }

  // writeToDebugFile(to_string(Current->Jarnac));
  if (!(Current->Jarnac)) {
    Board[Joueur][0] = retire(Board[Joueur][0], Current->DLetter);
    // writeToDebugFile("Found Word : " + Current->Word);
    // writeToDebugFile("Ligne : " + to_string(Current->Ligne));
    // writeToDebugFile("Origin : " + to_string(Current->Origin));
    Board[Joueur][Current->Ligne] = Current->Word;
    Board[Joueur][0] += piocheLettre();
  } else {
    // writeToDebugFile("Found Word : " + Current->Word);
    // writeToDebugFile("Ligne : " + to_string(Current->Ligne));
    // writeToDebugFile("Origin : " + to_string(Current->Origin));
    Board[1 - Joueur][0] = retire(Board[1 - Joueur][0], Current->DLetter);
    if (Current->Origin != -1) {
      Board[1 - Joueur][Current->Origin] = "";
    }
    Board[Joueur][Current->Ligne] = Current->Word;
  }
  affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                  NamesHelper->Name1, NamesHelper->Name2, Joueur,
                  Current->Jarnac);
  return make_tuple(Board, 1);
}

/**
 * Prompts the user to place a word on the board, handling invalid input.
 * Allows choosing a line, entering a word, and validating it against the board
 * state and dictionary. Handles confirmation loops and user cancellation.
 * Returns the updated board state after successfully placing a word.
 */
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

  // Constante 8 et 9
  affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                  NamesHelper->Name1, NamesHelper->Name2, Joueur, isJarnac);
  int state = 0;
  while (!state) {
    mot = "-";
    ligneTxt = "";
    // get line
    while (ligneTxt == "") {
      cout << Say << endl;
      cin >> ligneTxt;
      if (ligneTxt == "Q") {
        state = 1;
        break;
      }
      if (("1" > ligneTxt) || ("9" < ligneTxt) || (ligneTxt.size() == 2)) {
        cout << "Numéro de ligne invalide. Tapez 'Q' pour changer d'action."
             << endl;
        ligneTxt = "";
      }
    }

    NumLigne = atoi(ligneTxt.c_str());
    CurrentLigne = Board[Joueur][NumLigne];
    mot = "-";

    // get word
    while (mot == "-" and ligneTxt != "Q") {
      cout << endl << "Quel mot souhaitez-vous jouer ?" << endl;
      cin >> mot;
      mot = purifie(mot);
      if (mot == "R") {
        break;
      }
      if (mot == "Q") {
        Joue = false;
        break;
      }
      diffLetter = retire(mot, CurrentLigne);
      Current->Word = mot;
      Current->DLetter = diffLetter;
      if (isJarnac) {
        Current->Ligne = findWord(Board, mot, 1 - Joueur);
        Current->Origin = NumLigne;
      } else {
        Current->Ligne = NumLigne;
        Current->Origin = -1;
      }

      tie(Board, state) =
          JouerMot(Board, Joueur, Current, NamesHelper, DictHelper);
    }
  }
  return Board;
}

BOARD piocheOuEchange(BOARD Board, int Joueur) {
  string Rep;
  cout << "Voulez-vous piocher une lettre ou échanger 3 lettres ?" << endl;
  cout << "    P : Piocher une lettre" << endl;
  cout << "    E : Echanger 3 lettres" << endl;
  cin >> Rep;
  Rep = purifie(Rep);
  if (Rep == "P") {
    Board[Joueur][0] += piocheLettre();
  } else if (Rep == "E") {
    Board = echangeLettre(Board, Joueur);
  } else {
    cout << "Choix invalide." << endl;
    cout << "    P : Piocher une lettre" << endl;
    cout << "    E : Echanger 3 lettres" << endl;
  }
  return Board;
}

/**
 * Prompts the user to choose an action and performs it, alternating turns
 * between players.
 *
 * Displays the game board, prompts for an action, and executes it, handling
 * invalid input. Actions include placing a word (normal or Jarnac mode),
 * exchanging letters, passing turn, viewing possible actions, and resigning.
 *
 * Returns a special string to stop the game loop if the user resigns, otherwise
 * returns an empty string to continue alternating turns.
 */
tuple<BOARD, string> choixAction(BOARD Board, int Joueur, ForDict *DictHelper,
                                 Names *NamesHelper, int Tour) {
  string coup;
  bool J = true;  // check for tour si jarnac a un sens
  affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                  NamesHelper->Name1, NamesHelper->Name2, Joueur, false);
  while (true) {
    cout << "Quelle action souhaitez-vous effectuer ?" << endl;
    cin >> coup;
    coup = purifie(coup);
    cout << endl;
    if (coup == "C") {
      Board = PlaceMot(Board, Joueur, DictHelper, false, NamesHelper);
      J = false;
    } else if (coup == "F") {
      return make_tuple(Board, "");
    } else if (coup == "J" and Tour != 0) {
      if (J) {
        Board = PlaceMot(Board, Joueur, DictHelper, true, NamesHelper);
      } else {
        cout << "Vous avez déjà joué un mot." << endl;
      }
    } else if (coup == "P") {
      affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                      NamesHelper->Name1, NamesHelper->Name2, Joueur, false);
    } else if (coup == "M") {
      ActionPossible();
    } else if (coup == "A") {
      return make_tuple(Board, "-STOP-");
    } else {
      affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                      NamesHelper->Name1, NamesHelper->Name2, Joueur, false);
      cout << "Choix invalide, tapez 'M' pour voir les actions possibles."
           << endl;
    }
  }
  return make_tuple(Board, "");
}

/**
 * Initializes a new game board with the given width and height.
 *
 * Creates a BOARD with two empty player boards, initialized with
 * empty strings. Draws 6 random letters for each player's hand.
 * Initializes the remaining rows to empty strings.
 *
 * Returns the initialized BOARD.
 */
BOARD initBoard(int W, int H) {
  BOARD Board = {{""}, {""}};
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 6; j++) {
      Board[i][0] += piocheLettre();
    }

    for (int j = 1; j < H; j++) {
      Board[i].push_back("");
    }
  }
  return Board;
}

tuple<BOARD, string> Round(BOARD Board, int Joueur, ForDict *DictHelper,
                           Names *NamesHelper, StorePlayers *PlayerHelper,
                           int Tour) {
  if (!(PlayerHelper->isAI[Joueur])) {
    if (Tour >= 2) {
      Board = piocheOuEchange(Board, Joueur);
    }
    return choixAction(Board, Joueur, DictHelper, NamesHelper, Tour);
  } else {
    if (Tour >= 2) {
      Board[Joueur][0] += piocheLettre();
    }
    Play *IAMove = new Play;
    int nbPlay = 0;
    bool JarnacPossible = true;
    int state;
    do {
      affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                      NamesHelper->Name1, NamesHelper->Name2, Joueur, false);
      if (Tour == 0 or nbPlay > 0) {
        JarnacPossible = false;
      }
      IAMove =
          BestMove(Board, Joueur, JarnacPossible, PlayerHelper->AIS[Joueur]);
      nbPlay++;
      tie(Board, state) =
          JouerMot(Board, Joueur, IAMove, NamesHelper, DictHelper);

      // writeToDebugFile("State : " + to_string(state));
      // writeToDebugFile("IA End : " + to_string(IAMove->End));

      if (state == 0) {
        affichePlateaux(Board[0], Board[1], 8, 9, NamesHelper->NameGame,
                        NamesHelper->Name1, NamesHelper->Name2, Joueur,
                        IAMove->Jarnac);
        cout << IAMove->Word << endl;
        cout << IAMove->DLetter << endl;
        cout << IAMove->Ligne << endl;
        cout << IAMove->Origin << endl;
        cout << IAMove->Jarnac << endl;

        // throw an error

        throw invalid_argument("L'IA a tenté de jouer un mot impossible.");
      }
    } while (state == 1);
    // writeToDebugFile("SORTIE PRINCIPALE TOUR IA");
    return make_tuple(Board, "");
  }
}

bool endGame(BOARD Board, int Joueur) {
  bool AllEmpty = true;
  for (int j = 1; j < Board[Joueur].size(); j++) {
    if (Board[Joueur][j].length() == 0) {
      AllEmpty = false;
      break;
    }
  }
  return AllEmpty;
}

/**
 * lanceLeJeu launches a new game of Jarnac with the given player names.
 *
 * It initializes the game board, displays intro screens, and runs
 * the main game loop, alternating between players until one wins.
 *
 * It handles setting up the dictionary, calculating word search bounds,
 * calling the display and action selection functions for each turn,
 * and checking for a winning condition.
 *
 * joueur0: Name of Player 1
 * joueur1: Name of Player 2
 * Name: Name displayed at top of board
 */
bool lanceLeJeu(string joueur0, string joueur1, string Name, bool IA1,
                bool IA2) {
  // string nomDico = choisirDictionnaire();

  ForDict *DictHelper = new ForDict;
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

  cout << "Fin Load IA" << endl;

  // affichePlateaux(Board[0], Board[1], 8, 9, Name, joueur0, joueur1, 0);
  // ActionPossible();
  // cout << "Appuyez sur 'enter' pour continuer" << endl;
  // system("PAUSE");
  // getchar();

  // ATTENDRE INPUT D'UN JOUEUR AVANT DE CONTINUER

  string mot;
  int Joueur = 0;
  int Tour = 0;
  tie(Board, mot) =
      Round(Board, Joueur, DictHelper, NamesHelper, PlayersHelper, Tour);
  Tour++;
  if (mot == "-STOP-") {
    cout << "Fin du jeu, " << NamesHelper->Name2 << " a gagné." << endl;
  }

  Joueur = 1 - Joueur;

  tie(Board, mot) =
      Round(Board, Joueur, DictHelper, NamesHelper, PlayersHelper, Tour);
  Tour++;
  if (mot == "-STOP-") {
    cout << "Fin du jeu, " << NamesHelper->Name1 << " a gagné." << endl;
  }

  while (mot == "") {
    Joueur = 1 - Joueur;
    affichePlateaux(Board[0], Board[1], 8, 9, Name, joueur0, joueur1, Joueur,
                    false);
    tie(Board, mot) =
        Round(Board, Joueur, DictHelper, NamesHelper, PlayersHelper, Tour);
    // writeToDebugFile("Joueur : " + to_string(Joueur));
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

/**
 * Main entry point for the program.
 *
 * Calls lanceLeJeu() to start a new game, passing player names.
 *
 * Returns 0 to indicate successful program completion.
 */
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