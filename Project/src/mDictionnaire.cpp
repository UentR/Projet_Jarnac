#include "mDictionnaire.hpp"

struct ForDict;

char majuscule(char lettre) {
  if (lettre >= 65 and lettre <= 90) return lettre;
  if (lettre >= 97 and lettre <= 122) return lettre - 32;
  return '-';
}

string majuscule(string lettre) {
  char Lettre = lettre[0];
  string Rep = "";
  Rep += majuscule(Lettre);
  return Rep;
}

string purifie(string mot) {
  string Rep = "";
  for (char i : mot) {
    if (i >= 65 and i <= 90) Rep += i;
    if (i >= 97 and i <= 122) Rep += i - 32;
  }
  return Rep;
}

string purifieNbr(string Nbr) {
  string Rep = "";
  for (char i : Nbr) {
    if (i >= 48 and i <= 57) Rep += i;
  }
  return Rep;
}

bool trouve(string mot, ForDict *DictHelper) {
  string Word = purifie(mot);
  // Quitter si le mot n'est pas valide
  if (Word == "") {
    return false;
  }
  // Add doc for Idx
  int Idx = CalcIdx(Word, DictHelper->NBR);
  // Doc sur README.md
  long Borne = DictHelper->Bornes[Idx];
  int BorneInf = (Borne & (int)(pow(2, DictHelper->SHIFT) - 1));
  int BorneSup = Borne >> DictHelper->SHIFT;

  for (int i = BorneInf; i < BorneSup; i++) {
    if (DictHelper->Mots[i] == Word) {
      return true;
    }
  }
  return false;
}

int CalcIdx(string Word, int NBR) {
  int Idx = 0;
  int Mult = 1;
  for (int j = NBR - 1; j >= 0; j--) {
    Idx += (Word[j] - 65) * Mult;
    Mult *= 26;
  }
  return Idx;
}

void CreateBorne(ForDict *DictHelper) {
  if (DictHelper->SHIFT > 32) {
    cerr << "Dictionnaire trop grand, overflow" << endl;
    exit(136);
  }

  double Size = pow(26, DictHelper->NBR);
  DictHelper->Bornes = vector<long>(Size, 0);
  string First = DictHelper->Mots[0].substr(0, DictHelper->NBR);
  long BorneInf = 0;
  long BorneSup = 0;
  int Idx;
  string Word;
  for (int i = 0; i < DictHelper->Mots.size(); i++) {
    Word = DictHelper->Mots[i];
    // Check si les NBR premiers caractères sont différents
    if (Word.rfind(First, 0) != 0) {
      // Ils sont différents, on enregistre la borne
      Idx = CalcIdx(First, DictHelper->NBR);
      BorneSup = i;
      DictHelper->Bornes[Idx] =
          (BorneSup << DictHelper->SHIFT) + (BorneInf);  // Se référer à la doc
      BorneInf = i;
      First = Word.substr(0, DictHelper->NBR);
    }
  }
  // Prise en compte de la dernière borne
  Idx = CalcIdx(First, DictHelper->NBR);
  BorneSup = DictHelper->Mots.size();
  DictHelper->Bornes[Idx] =
      (BorneSup << DictHelper->SHIFT) + (BorneInf);  // Se référer à la doc
}

void importeDico(string adresseDico, ForDict *DictHelper) {
  ifstream File;
  File.open(adresseDico);
  string Word;
  int Nbr = 0;
  // Si Nbr > 2^18, on overflow lors de la création des bornes
  while (File >> Word and Nbr < pow(2, 18)) {
    Nbr++;
    DictHelper->Mots.push_back(Word);
  }
  File.close();
  DictHelper->SHIFT = ceil(log(Nbr) / log(2));
}

void CreateHelper(ForDict *DictHelper, string adresseDico, int NBR) {
  DictHelper->NBR = NBR;
  importeDico(adresseDico, DictHelper);
  CreateBorne(DictHelper);
}

// void TEST() {
//   ForDict *DictHelper = new ForDict;
//   CreateHelper(DictHelper, "Text/DictionnairePurified.txt", 3);
//   CHECK(majuscule('a') == 'A');
//   CHECK(majuscule('b') == 'B');
//   CHECK(majuscule('D') == 'D');
//   CHECK(majuscule('^') == '-');

//   CHECK(purifie(" Ou i¹") == "OUI");
//   CHECK(purifie("NOM?") == "NOM");

//   CHECK(trouve("ZYTHUMS", DictHelper));
//   CHECK(trouve("non", DictHelper));
//   CHECK(trouve("ABACAS", DictHelper));
//   CHECK(trouve("AAS", DictHelper));
//   CHECK(trouve("AALENIEN", DictHelper));
//   CHECK(trouve("ABACA", DictHelper));
//   CHECK(trouve("REABONNAI", DictHelper));
//   CHECK(trouve("REABONNA", DictHelper));
//   CHECK(trouve("BITA", DictHelper));

//   CHECK(!trouve("ZZZ", DictHelper));
//   CHECK(!trouve("???", DictHelper));
//   CHECK(!trouve("Nomination", DictHelper));
// }

// int main() { return 0; }
