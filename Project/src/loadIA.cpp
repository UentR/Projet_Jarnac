#include "loadIA.hpp"

struct ForDict;
struct Node;
struct AI;
struct Play;

char *envIALevel = getenv("LEVEL");
bool EmptyIALevel = envIALevel == NULL;
const int LevelIA = (EmptyIALevel) ? 60 : atoi(envIALevel);

string Sort(string Mot) {
  string Sorted = Mot;
  sort(Sorted.begin(), Sorted.end());
  return Sorted;
}

string chRetire(string Word, char Letter) {
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
      string NewVrac = chRetire(Vrac, Letter);
      set<string> NewPermutations = DictPermutations(NewVrac, Length - 1);
      for (string Permutation : NewPermutations) {
        Permutations.insert(Sort(Letter + Permutation));
      }
    }
  }
  return Permutations;
}

set<string> FindWords(string Vrac, map<string, string> Words, bool Jarnac) {
  set<string> FoundWords = {};
  string SortedWord;
  bool StopSearch = false;
  int Len = min((int)Vrac.length(), 9);
  int I;
  for (int i = 3; i <= Len; i++) {
    if (Jarnac) {
      I = (Len + 3) - i;
    } else {
      I = i;
    }
    set<string> Permutations = DictPermutations(Vrac, I);
    for (string Permutation : Permutations) {
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

tuple<Node *, string> Analyze(string Vrac, vector<Node *> PlayerWords) {
  set<tuple<Node *, string, string, Node *> > toAnalyze = {};
  if (PlayerWords.size() == 0) {
    return make_tuple(new Node, "");
  }
  // On analyse tous les mots du joueur
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
      if ((Word->Children.find(Letter) != Word->Children.end()) and
          ((rand() % 100) < LevelIA)) {
        Child = true;
        // Si on peut atteindre un enfant, on l'ajoute à la liste des
        // noeuds à analyser
        toAnalyze.insert(make_tuple(Word->Children[Letter],
                                    chRetire(tVrac, Letter), Path + Letter,
                                    Origin));
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

int getLine(BOARD Board, string Word, int Joueur) {
  for (int i = 1; i < Board[Joueur].size(); i++) {
    if (Sort(Board[Joueur][i]) == Sort(Word)) {
      return i;
    }
  }
  return -1;
}

Play *BestMove(BOARD Board, int Joueur, bool Jarnac, AI *AIHelper) {
  writeToDebugFile("BestMove", ERROR);
  Play *Current = new Play;
  Current->Jarnac = Jarnac;
  Current->End = false;

  string Path;
  Node *Word = new Node;

  vector<Node *> PlayerWords = {};
  set<string> Playable = {};

  int Ligne = getLine(Board, "", Joueur);
  writeToDebugFile("Jarnac : " + to_string(Jarnac), ERROR);
  if (Jarnac) {
    if (Ligne != -1) {
      writeToDebugFile("Jarnac", ERROR);
      // On regarde si on peut agrandir les mots de l'adversaire
      for (int i = 1; i < Board[1 - Joueur].size(); i++) {
        if (Board[1 - Joueur][i].length() > 0) {
          PlayerWords.push_back(AIHelper->NodeDict[Sort(Board[1 - Joueur][i])]);
        }
      }
      tie(Word, Path) = Analyze(Board[1 - Joueur][0], PlayerWords);
      if (Path.length() >= 1) {
        Current->Word = Word->Children[Path[0]]->Ana;
        Current->Origin = getLine(Board, Word->Ana, 1 - Joueur);
        Current->Ligne = Ligne;
        Current->DLetter = Path[0];
        return Current;
      }

      // On regarde si on peut placer un mot avec le vrac de l'adversaire
      Playable = FindWords(Board[1 - Joueur][0], AIHelper->Dict, true);
      if (Playable.size() > 0) {
        Current->Word = *Playable.begin();
        Current->Ligne = Ligne;
        Current->Origin = getLine(Board, "", 1 - Joueur);
        Current->DLetter = *Playable.begin();
        return Current;
      }
    }

    Current->End = true;
    Current->DLetter = "";
    return Current;
  } else {
    writeToDebugFile("Ligne : " + to_string(Ligne), ERROR);
    // On regarde si on peut agrandir l'un de nos mots
    for (int i = 1; i < Board[Joueur].size(); i++) {
      if (Board[Joueur][i].length() > 0) {
        writeToDebugFile(
            "Board[Joueur][" + to_string(i) + "] : " + Board[Joueur][i], ERROR);
        PlayerWords.push_back(AIHelper->NodeDict[Sort(Board[Joueur][i])]);
      }
    }
    tie(Word, Path) = Analyze(Board[Joueur][0], PlayerWords);
    writeToDebugFile("Path : " + Path, ERROR);
    writeToDebugFile("Path length : " + to_string(Path.length()), ERROR);
    if (Path.length() >= 1) {
      Current->Word = Word->Children[Path[0]]->Ana;
      Current->Ligne = getLine(Board, Word->Ana, Joueur);
      Current->DLetter = Path[0];
      return Current;
    }

    // On regarde si on peut placer un mot avec notre vrac
    // Ne pas chercher si tableau complet
    if (Ligne != -1) {
      Playable = FindWords(Board[Joueur][0], AIHelper->Dict, false);
      if (Playable.size() > 0) {
        Current->Word = *Playable.begin();
        Current->Ligne = Ligne;
        Current->DLetter = *Playable.begin();
        return Current;
      }
    }

    // Si on n'a toujours pas trouvé de mot on fini notre tour
    Current->End = true;
    Current->DLetter = "";
    return Current;
  }
}

void LoadDict(AI *AIHelper, string FileName) {
  ifstream File;
  string Word;
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
  string letter;
  File.open(FileName);
  T = new Node;
  // Structure du fichier :
  // Mot Lettre1 Lettre2 ... LettreN;
  while (File >> T->Ana) {
    File >> letter;
    while (letter != ";") {
      // On ajoute les noeuds enfants
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
