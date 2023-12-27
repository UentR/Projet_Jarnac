#include "loadIA.hpp"

struct ForDict;
struct Node;
struct AI;
struct Play;

char *envIALevel = getenv("LEVEL");
bool EmptyIALevel = envIALevel == NULL;
const int LevelIA = (EmptyIALevel) ? 60 : atoi(envIALevel);

char *Sort(char *Mot) {
  int Size = strlen(Mot);
  char *Sorted = new char[Size + 1];
  strcpy(Sorted, Mot);
  sort(Sorted, Sorted + Size);
  return Sorted;
}

char *chRetire(char *Word, char Letter) {
  char *newString = new char[strlen(Word) + 1];
  for (auto *t = Word; *t; t++) {
    if (*t != Letter) {
      strcat(newString, t);
    }
  }
  return newString;
}

string Retire(string Word, string Duplicate) {
  string newString = "";
  for (char character : Word) {
    if (Duplicate.find(character) == string::npos) {
      newString += character;
    }
  }
  return newString;
}

set<char *> DictPermutations(char *Vrac, int Length) {
  set<char *> Permutations = {};
  char *Word;
  if (Length == 1) {
    for (auto *t = Vrac; *t; t++) {
      Permutations.insert(t);
    }
  } else {
    for (auto *t = Vrac; *t; t++) {
      char *NewVrac = chRetire(Vrac, *t);
      set<char *> NewPermutations = DictPermutations(NewVrac, Length - 1);
      for (char *Permutation : NewPermutations) {
        strcpy(Word, Permutation);
        strcat(Word, t);
        Permutations.insert(Sort(Word));
      }
    }
  }
  return Permutations;
}

set<string> FindWords(char *Vrac, map<char *, char *> Words, bool Jarnac) {
  set<string> FoundWords = {};
  string SortedWord;
  bool StopSearch = false;
  int Len = min((int)strlen(Vrac) - 1, 9);
  int I;
  for (int i = 3; i <= Len; i++) {
    if (Jarnac) {
      I = (Len + 3) - i;
    } else {
      I = i;
    }
    set<char *> Permutations = DictPermutations(Vrac, I);
    for (char *Permutation : Permutations) {
      if (Words.find(Permutation) != Words.end() and
          ((rand() % 100) < LevelIA)) {
        FoundWords.insert(Words[Permutation]);
        StopSearch = true;
      }
    }
    if (StopSearch) {
      break;
    }
  }
  return FoundWords;
}

tuple<Node *, char *> Analyze(char *Vrac, vector<Node *> PlayerWords) {
  set<tuple<Node *, char *, char *, Node *> > toAnalyze = {};
  if (PlayerWords.size() == 0) {
    return make_tuple(new Node, (char *)"");
  }
  // On analyse tous les mots du joueur
  for (Node *Word : PlayerWords) {
    toAnalyze.insert(make_tuple(Word, Vrac, (char *)"", Word));
  }

  tuple<Node *, char *> BaseCase = make_tuple(PlayerWords[0], (char *)"");
  set<tuple<Node *, char *> > End = {};
  Node *Word;
  char *tVrac;
  bool Child;
  char *Path;
  char *NewPath;
  Node *Origin;
  while (!toAnalyze.empty()) {
    tie(Word, tVrac, Path, Origin) = *toAnalyze.begin();
    toAnalyze.erase(toAnalyze.begin());
    Child = false;
    for (auto *t = tVrac; t != '\0'; t++) {
      // for (auto *Finder = Word->Children; *t; t++) {
      if ((Word->Children.find(*t) != Word->Children.end()) and
          ((rand() % 100) < LevelIA)) {
        Child = true;
        // Si on peut atteindre un enfant, on l'ajoute à la liste des
        // noeuds à analyser

        toAnalyze.insert(make_tuple(Word->Children[*t], chRetire(tVrac, *t),
                                    Path + Letter, Origin));
      }
    }
    // Si on ne peut pas atteindre d'enfant, on ajoute le mot à la liste
    // des mots finaux
    if (!Child) {
      End.insert(make_tuple(Origin, Path));
    }
  }

  // On cherche le mot qui permet d'ajouter le plus de lettres
  int Longest = 0;
  string BestPath;
  for (tuple<Node *, string> EndWord : End) {
    tie(Word, Path) = EndWord;
    if (Path.length() > Longest) {
      Longest = Path.length();
      BaseCase = make_tuple(Word, Path);
    }
  }
  return BaseCase;
}

string PiocheEchange(BOARD Board, int Joueur) { return ""; }

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

Play *BestMove(BOARD *Board, int Joueur, bool Jarnac, AI *AIHelper) {
  cout << "BestMove" << endl;
  writeToDebugFile("BestMove", ERROR);
  Play *Current = new Play;
  Current->Jarnac = Jarnac;
  Current->End = false;

  string Path;
  Node *Word = new Node;

  vector<Node *> PlayerWords = {};
  set<string> Playable = {};
  cout << "Jarnac : " << Jarnac << endl;
  int Ligne = getLine(Board->Board, "", Joueur);
  cout << "Ligne : " << Ligne << endl;
  writeToDebugFile("Jarnac : " + to_string(Jarnac), ERROR);

  int CurrJoueur = Joueur;
  if (Jarnac) {
    CurrJoueur = 1 - Joueur;
  }

  if (Ligne != -1) {
    cout << "Ligne != -1" << endl;
    for (int i = 0; i < 8; i++) {
      if (*(Board->Board + CurrJoueur * 80 + i * 10) != '\0') {
        PlayerWords.push_back(
            AIHelper->NodeDict[Sort(*(Board + CurrJoueur * 80 + i * 10))]);
      }
    }
    cout << "PlayerWords.size() : " << PlayerWords.size() << endl;
    tie(Word, Path) = Analyze(Board->Vracs[CurrJoueur], PlayerWords);
    cout << "Word : " << Word->Ana << endl;
    if (Path.length() >= 1) {
      Current->Word = Word->Children[Path[0]]->Ana;
      if (Jarnac) {
        Current->Origin = getLine(Board, Word->Ana, CurrJoueur);
        Current->Ligne = Ligne;
      } else {
        Current->Ligne = getLine(Board, Word->Ana, Joueur);
      }
      Current->DLetter = Path[0];
      return Current;
    }
    cout << "Path.length() < 1" << endl;
    // On regarde si on peut placer un mot avec le vrac de l'adversaire
    Playable = FindWords(Board->Vracs[CurrJoueur], AIHelper->Dict, true);
    cout << "Playable.size() : " << Playable.size() << endl;
    if (Playable.size() > 0) {
      Current->Word = *Playable.begin();
      if (Jarnac) {
        Current->Origin = getLine(Board, Word->Ana, CurrJoueur);
        Current->Ligne = Ligne;
      } else {
        Current->Ligne = Ligne;
      }
      Current->DLetter = *Playable.begin();
      return Current;
    }
    cout << "Hre" << endl;
  }

  Current->End = true;
  Current->DLetter = "";
  Current->Jarnac = false;
  return Current;
}

void LoadDict(AI *AIHelper, string FileName) {
  ifstream File;
  char *Word;
  File.open(FileName);
  // Avec une map on peut trouver un mot en O(1) au lieu de chercher
  // dans un vecteur en O(n)
  // En triant les mots on stocke moins de noeuds dans l'arbre
  // qui auraient eu les mêmes enfants
  while (File >> Word) {
    AIHelper->Dict[Sort(Word)] = Word;
  }
  File.close();
}

void LoadTree(AI *AIHelper, string FileName) {
  ifstream File;
  Node *T;
  char letter;
  char *Word;
  File.open(FileName);
  T = new Node;
  // Structure du fichier :
  // Mot Lettre1 Lettre2 ... LettreN;
  while (File >> T->Ana) {
    File >> letter;
    while (letter != ";") {
      strcpy(T->Ana, Word);
      strcat(Word, &letter);
      // On ajoute les noeuds enfants
      T->Children[letter] = AIHelper->NodeDict[Sort(Word)];
      File >> letter;
    }
    AIHelper->NodeDict[Sort(T->Ana)] = T;
    T = new Node;
  }
  File.close();
}

void StartUpAI(AI *AIHelper) {
  cout << "Loading AI" << endl;
  LoadDict(AIHelper, "Text/DicoIA.txt");
  cout << "Dict loaded" << endl;
  LoadTree(AIHelper, "Text/Arbre.txt");
  cout << "Tree loaded" << endl;
}

void TEST() {
  AI *AIHelper = new AI;
  LoadTree(AIHelper, "Text/Arbre.txt");

  cout << "Tree loaded" << endl;
  vector<string> WORDS = {};
  int NBR;
  cout << "Combien de mots déjà placé : " << endl;
  cin >> NBR;
  string TT;
  for (int i = 0; i < NBR; i++) {
    cout << "Mot numéro " << i + 1 << " : " << endl;
    cin >> TT;
    WORDS.push_back(TT);
  }
  vector<Node *> PlayerWords = {};
  for (string Word : WORDS) {
    PlayerWords.push_back(AIHelper->NodeDict[Sort(Word)]);
  }

  cout << "Player words loaded" << endl;

  string Vrac;
  cout << "Quel est votre vrac ?" << endl;
  cin >> Vrac;

  Node *Origin;
  string Path;
  tie(Origin, Path) = Analyze(Vrac, PlayerWords);
  cout << "Longest path: " << Path << endl;
  cout << "Longest path length: " << Path.length() << endl;
  cout << "Origin: " << Origin->Ana << endl;

  cout << "To place: " << endl;
  Node *Temp = Origin;
  cout << Temp->Ana << endl;
  for (char Letter : Path) {
    Temp = Temp->Children[Letter];
    cout << Temp->Ana << endl;
  }
}

// int main() {
//   // TEST();

//   AI *AIHelper = new AI;
//   cout << "Hello" << endl;
//   StartUpAI(AIHelper);
//   cout << "AI loaded" << endl;

//   BOARD Board = {{"DIAM", "TETS", "TOQUE", "", "", "", "", "", ""},
//                  {"ODHSQ", "SECOND", "MOINS", "", "", "", "", "", ""}};

//   Play *Current = BestMove(Board, 0, true, AIHelper);
//   cout << Current->Word << endl;
//   cout << Current->Ligne << endl;
//   cout << Current->Jarnac << endl;
//   cout << Current->End << endl;

//   cout << AIHelper->NodeDict[Sort("TEST")]->Children['S'] << endl;
// }
