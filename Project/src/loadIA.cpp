#include "loadIA.hpp"

#include "Debug.hpp"
#include "mDictionnaire.hpp"

struct ForDict;
struct Node;
struct AI;
struct Play;

string Sort(string Mot) {
  string Sorted = Mot;
  sort(Sorted.begin(), Sorted.end());
  return Sorted;
}

string fRetire(string Word, char Letter) {
  string newString = "";
  for (char character : Word) {
    if (character != Letter) {
      newString += character;
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

set<string> DictPermutations(string Vrac, int Length) {
  set<string> Permutations = {};
  if (Length == 1) {
    for (char Letter : Vrac) {
      Permutations.insert(string(1, Letter));
    }
  } else {
    for (char Letter : Vrac) {
      string NewVrac = fRetire(Vrac, Letter);
      set<string> NewPermutations = DictPermutations(NewVrac, Length - 1);
      for (string Permutation : NewPermutations) {
        Permutations.insert(Sort(Letter + Permutation));
      }
    }
  }
  return Permutations;
}

set<string> FindWords(string Vrac, map<string, string> Words) {
  set<string> FoundWords = {};
  string SortedWord;
  bool StopSearch = false;
  for (int i = 3; i <= Vrac.length(); i++) {
    set<string> Permutations = DictPermutations(Vrac, i);
    for (string Permutation : Permutations) {
      if (Words.find(Permutation) != Words.end()) {
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

tuple<Node *, string> Analyze(string Vrac, vector<Node *> PlayerWords) {
  set<tuple<Node *, string, string, Node *> > toAnalyze = {};
  if (PlayerWords.size() == 0) {
    return make_tuple(new Node, "");
  }
  for (Node *Word : PlayerWords) {
    toAnalyze.insert(make_tuple(Word, Vrac, "", Word));
  }

  tuple<Node *, string> BaseCase = make_tuple(PlayerWords[0], "");

  set<tuple<Node *, string> > End = {};

  Node *Word;
  string tVrac;
  bool Child;
  string Path;
  Node *Origin;
  while (!toAnalyze.empty()) {
    tie(Word, tVrac, Path, Origin) = *toAnalyze.begin();

    toAnalyze.erase(toAnalyze.begin());

    Child = false;
    for (char Letter : tVrac) {
      if (Word->Children.find(Letter) != Word->Children.end()) {
        Child = true;
        toAnalyze.insert(make_tuple(Word->Children[Letter],
                                    fRetire(tVrac, Letter), Path + Letter,
                                    Origin));
      }
    }
    if (!Child) {
      End.insert(make_tuple(Origin, Path));
    }
  }

  int Longest = 0;
  string BestPath;
  Node *BestStart;
  for (tuple<Node *, string> EndWord : End) {
    tie(Word, Path) = EndWord;
    if (Path.length() > Longest) {
      Longest = Path.length();
      BaseCase = make_tuple(Word, Path);
    }
  }
  return BaseCase;
}

bool PiocheEchange(BOARD Board, int Joueur, bool Jarnac) { return true; }

int getLine(BOARD Board, string Word, int Joueur) {
  for (int i = 1; i < Board[Joueur].size(); i++) {
    if (Sort(Board[Joueur][i]) == Sort(Word)) {
      return i;
    }
  }
  return -1;
}

Play *BestMove(BOARD Board, int Joueur, bool Jarnac, AI *AIHelper) {
  Play *Current = new Play;
  Current->Jarnac = Jarnac;
  Current->End = false;

  string Path;
  Node *Word = new Node;

  vector<Node *> PlayerWords = {};
  set<string> Playable = {};

  int Ligne = getLine(Board, "", Joueur);
  if (Jarnac) {
    // Dans quels sens jouer ici
    // faire fonctions
    for (int i = 1; i < Board[1 - Joueur].size(); i++) {
      if (Board[1 - Joueur][i].length() > 0) {
        PlayerWords.push_back(AIHelper->NodeDict[Sort(Board[1 - Joueur][i])]);
      }
    }
    tie(Word, Path) = Analyze(Board[1 - Joueur][0], PlayerWords);
    if (Path.length() >= 1) {
      Current->Word = Word->Children[Path[0]]->Ana;
      Current->Origin = getLine(Board, Word->Ana, Joueur);
      Current->Ligne = Ligne;
      Current->DLetter = Path[0];
      return Current;
    }
    Playable = FindWords(Board[1 - Joueur][0], AIHelper->Dict);
    if (Playable.size() > 0) {
      Current->Word = *Playable.begin();
      Current->Ligne = Ligne;
      Current->Origin = getLine(Board, "", 1 - Joueur);
      Current->DLetter = *Playable.begin();
      return Current;
    }
  }

  Current->Jarnac = false;

  // writeToDebugFile("Fct IA BestMove");
  PlayerWords = {};
  Playable = {};

  // writeToDebugFile("Fct IA BestMove : Mots allongeables");

  // Mots allongeables
  for (int i = 1; i < Ligne; i++) {
    PlayerWords.push_back(AIHelper->NodeDict[Sort(Board[Joueur][i])]);
  }
  tie(Word, Path) = Analyze(Board[Joueur][0], PlayerWords);
  if (Path.length() >= 1) {
    Current->Word = Word->Children[Path[0]]->Ana;
    Current->Ligne = getLine(Board, Word->Ana, Joueur);
    Current->DLetter = Path[0];
    return Current;
  }

  // Mots placables
  Playable = FindWords(Board[Joueur][0], AIHelper->Dict);
  if (Playable.size() > 0) {
    Current->Word = *Playable.begin();
    Current->Ligne = Ligne;
    Current->DLetter = *Playable.begin();
    return Current;
  }
  Current->End = true;
  Current->DLetter = "";
  // writeToDebugFile("Fct IA BestMove : Fin");
  return Current;
}

void LoadDict(AI *AIHelper, string FileName) {
  ifstream File;
  string Word;
  File.open(FileName);
  while (File >> Word) {
    AIHelper->Dict[Sort(Word)] = Word;
  }
  File.close();
}

void LoadTree(AI *AIHelper, string FileName) {
  ifstream File;
  Node *T;
  string letter;
  File.open(FileName);
  T = new Node;
  while (File >> T->Ana) {
    File >> letter;
    while (letter != ";") {
      T->Children[letter[0]] = AIHelper->NodeDict[Sort((T->Ana + letter))];
      File >> letter;
    }
    AIHelper->NodeDict[Sort(T->Ana)] = T;
    T = new Node;
  }
  File.close();
}

void StartUpAI(AI *AIHelper) {
  LoadDict(AIHelper, "Text/DicoIA.txt");
  LoadTree(AIHelper, "Text/Arbre.txt");
}

void TEST() {
  AI *AIHelper = new AI;
  LoadTree(AIHelper, "Text/Arbre.txt");

  cout << "Tree loaded" << endl;
  vector<string> WORDS = {};
  int NBR;
  cout << "Combien de mots déjà placé :" << endl;
  cin >> NBR;
  string TT;
  for (int i = 0; i < NBR; i++) {
    cout << "Mot numéro " << i + 1 << " :" << endl;
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

  cout << "To place:" << endl;
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
