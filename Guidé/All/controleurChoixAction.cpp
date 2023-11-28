
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <random>

using namespace std;

#include "mAnagrammes.hpp"
#include "mDictionnaire.hpp"

using BOARD = vector<vector<string>>;

bool isVowel(string Letter)
{
    return (Letter == "A" or Letter == "E" or Letter == "I" or Letter == "O" or Letter == "U" or Letter == "Y");
}

vector<string> createLetters()
{
    map<string, int> usableLetter = {
        {"A", 14},
        {"B", 4},
        {"C", 7},
        {"D", 5},
        {"E", 19},
        {"F", 2},
        {"G", 4},
        {"H", 2},
        {"I", 11},
        {"J", 1},
        {"K", 1},
        {"L", 6},
        {"M", 5},
        {"N", 9},
        {"O", 8},
        {"P", 4},
        {"Q", 1},
        {"R", 10},
        {"S", 7},
        {"T", 9},
        {"U", 8},
        {"V", 2},
        {"W", 1},
        {"X", 1},
        {"Y", 1},
        {"Z", 2}};
    vector<string> Rep = {};
    for (auto &iter : usableLetter)
    {
        for (int i = 0; i < iter.second; i++)
        {
            Rep.push_back(iter.first);
        }
    }
    srand(time(NULL));
    bool Vowel1 = false;
    bool Vowel2 = false;
    int Size = Rep.size() - 1;
    while (!Vowel1 || !Vowel2)
    {
        random_shuffle(Rep.begin(), Rep.end());
        for (int i = Size; i >= Size - 6; i--)
        {
            if (isVowel(Rep[i]))
            {
                Vowel1 = true;
                break;
            }
            Vowel1 = false;
        }
        for (int i = Size - 7; i >= Size - 12; i--)
        {
            if (isVowel(Rep[i]))
            {
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

string choisirDictionnaire()
{
    string choix;
    cout << "Quel dictionnaire souhaitez-vous choisir" << endl;
    cin >> choix;
    return choix;
}

string piocheLettre()
{
    LEN--;
    return LETTERS[LEN];
}

void ActionPossible()
{
    cout << "Au cours du jeu il est possible de taper certaines lettres pour piloter la manche." << endl;
    cout << "    A : Abandonner la Partie" << endl;
    cout << "    C : Continuer en choisissant un mot" << endl;
    cout << "    E : Echanger trois Lettres" << endl;
    cout << "    F : Finir son tour" << endl;
    cout << "    J : crier Jarnac" << endl;
    cout << "    V : Voir ce menu" << endl;
}

// string choisirAction(vector<string> possibilites, string nomJoueur);

// string choisirMot(vector<string> plateau, string nomJoueur, string dico);

// string TourJoueur()
// {
//     piocheLettre();
// }

void JouerMot(BOARD Board, int Joueur, int Ligne, string Mot, string diffLetter, bool isJarnac)
{

    Board[Joueur][0] = retire(Board[Joueur][0], diffLetter);
    if (!isJarnac)
    {
        Board[Joueur][Ligne] = Mot;
    }
    else
    {
        Board[Joueur][Ligne] = "";
        for (int i = 1; i < Board[1 - Joueur].size(); i++)
        {
            if (Board[1 - Joueur][i] == "")
            {
                Board[1 - Joueur][i] = Mot;
                break;
            }
        }
    }
}

void PlaceMot(BOARD Board, int Joueur, vector<string> Dico, vector<long> Bornes, int SHIFT, int NBR, bool isJarnac)
{
    string Say = "Sur quelle ligne souhaitez-vous jouer ?";
    if (isJarnac)
    {
        Joueur = 1 - Joueur;
        Say = "Où pensez-vous voir un mot ?";
    }
    bool Joue = true;
    int Ligne = -1;
    string mot = "-";
    string CurrentLigne = "";
    string diffLetter;
    string Vrac = Board[Joueur][0];

    while (Joue)
    {
        while (Ligne == -1)
        {
            cout << Say << endl;
            cin >> Ligne;
            if (1 > Ligne || 9 < Ligne)
            {
                cout << "Numéro de ligne invalide." << endl;
                Ligne = -1;
            }
        }
        CurrentLigne = Board[Joueur][Ligne];
        while (mot == "-")
        {
            cout << endl
                 << "Quel mot souhaitez-vous jouer ?" << endl;
            cin >> mot;
            mot = purifie(mot);
            if (mot == "R")
            {
                Ligne = -1;
                break;
            }
            diffLetter = retire(mot, CurrentLigne);
            for (auto i : diffLetter)
            {
                if (Vrac.find(i) == string::npos)
                {
                    cout << "Mot impossible a jouer. Taper 'R' pour choisir une autre ligne." << endl;
                    // mot = "-";
                    break;
                }
            }
            if (!trouve(mot, Dico, Bornes, SHIFT, NBR))
            {
                cout << "Mot inexistant. Taper 'R' pour choisir une autre ligne." << endl;
                mot = "-";
            }
            else
            {
                JouerMot(Board, Joueur, Ligne, mot, diffLetter, isJarnac);
                Ligne = -1;
            }
        }
    }
}

string choixAction(BOARD Board, int Joueur, vector<string> Dico, vector<long> Bornes, int SHIFT, int NBR)
{
    string coup = "-";

    while (coup == "-")
    {
        cout << "Quelle action souhaitez-vous effectuer ?" << endl;
        cin >> coup;
        cout << endl;
        if (coup == "C")
        {
            PlaceMot(Board, Joueur, Dico, Bornes, SHIFT, NBR, false);
        }
        else if (coup == "F")
        {
            return "";
        }
        else if (coup == "J")
        {
            PlaceMot(Board, Joueur, Dico, Bornes, SHIFT, NBR, true);
        }
        else if (coup == "E")
        {
            break;
        }
        else if (coup == "V")
        {
            ActionPossible();
            coup = "-";
        }
        else if (coup == "A")
        {
            return "-STOP-";
        }
        else
        {
            cout << "Choix invalide, tapez 'V' pour voir les actions possibles." << endl;
            coup = "-";
        }
    }
    return "";
}

bool lanceLeJeu(string joueur0, string joueur1)
{
    string nomDico = choisirDictionnaire();
    // vector<string> Dico = importeDico(choisirDictionnaire());
    vector<string> Dico = importeDico("Guidé/DictionnairePurified.txt");
    int SHIFT = ceil(log(Dico.size()) / log(2));
    int NBR = 3;
    vector<long> BORNES = CreateBorne(Dico, NBR, SHIFT);

    BOARD Board = initBoard(8, 9);

    ActionPossible();

    int Joueur = 0;
    string mot = choixAction(Board, Joueur, Dico, Bornes, SHIFT, NBR);
    if (mot == "-STOP-")
    {
        cout << "Fin du jeu, " << joueur1 << " a gagné." << endl;
    }
    return true;
}

BOARD initBoard(int W, int H)
{
    BOARD Board = {{""}, {""}};
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            Board[i][0] += piocheLettre();
        }

        for (int j = 1; j < H; j++)
        {
            Board[i].push_back("");
        }
    }
    return Board;
}

int main()
{

    lanceLeJeu("Moi", "Toi");
    return 0;
}