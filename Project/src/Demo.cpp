#include "controleurChoixAction.hpp"

using namespace std;

int main() {
  ForDict *DictHelper = new ForDict;
  CreateHelper(DictHelper, "Text/DictionnairePurified.txt", TAILLES_CLEES_DICO);

  // Stockage des noms
  Names *NamesHelper = new Names;
  NamesHelper->Name1 = "IA 1";
  NamesHelper->Name2 = "IA 2";

  // Stockage des IA
  StorePlayers *PlayersHelper = new StorePlayers;
  PlayersHelper->isAI[0] = true;
  PlayersHelper->isAI[1] = true;
  AI *Both = new AI;
  PlayersHelper->AIS[0] = Both;
  PlayersHelper->AIS[1] = Both;

  BOARD Board;
  StoreLetters *LETTERS;
  int NbPartie = 0;
  int Joueur;
  int Tour;
  string mot;
  int Count;
  Count = 0;
  while (true) {
    LETTERS = new StoreLetters;
    Board = initBoard(LETTERS, NB_LIGNES_PLATEAU, TAILLE_MAX_MOT);

    Joueur = 0;
    Tour = 0;
    tie(Board, mot) = Round(Board, Joueur, DictHelper, NamesHelper,
                            PlayersHelper, Tour, LETTERS);
    Tour++;

    Joueur = 1 - Joueur;
    tie(Board, mot) = Round(Board, Joueur, DictHelper, NamesHelper,
                            PlayersHelper, Tour, LETTERS);
    Tour++;

    while (true) {
      Joueur = 1 - Joueur;
      affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                      NamesHelper->NameGame, NamesHelper->Name1,
                      NamesHelper->Name2, Joueur, false);
      tie(Board, mot) = Round(Board, Joueur, DictHelper, NamesHelper,
                              PlayersHelper, Tour, LETTERS);
      Tour++;
      if (endGame(Board, Joueur)) {
        break;
      }
    }
    cout << "Fin de la partie N°" << NbPartie << endl;
    affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                    NamesHelper->NameGame, NamesHelper->Name1,
                    NamesHelper->Name2, 0, false);

    int Point1 = calculPoints(Board[0], TAILLE_MAX_MOT, NB_LIGNES_PLATEAU);
    int Point2 = calculPoints(Board[1], TAILLE_MAX_MOT, NB_LIGNES_PLATEAU);

    if (Point1 == Point2) {
      cout << "Fin du jeu, égalité" << endl;
    } else if (Point1 > Point2) {
      cout << "Fin du jeu, " << NamesHelper->Name1 << " a gagné." << endl;
    } else {
      cout << "Fin du jeu, " << NamesHelper->Name2 << " a gagné." << endl;
    }
    cout << "Fin du jeu, " << NamesHelper->Name1 << " a gagné." << endl;
  }
}