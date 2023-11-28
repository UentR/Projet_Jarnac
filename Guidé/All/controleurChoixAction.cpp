
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <random>

using namespace std;

#include "mAnagrammes.hpp"
#include "mDictionnaire.hpp"
#include "vueEnModeTexte.hpp"

using BOARD = vector<vector<string>>;

void Show(BOARD Board)
{
    for (auto i : Board)
    {
        cout << "-------" << endl;
        for (auto j : i)
        {
            cout << j << endl;
        }
    }
}

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
        for (int i = Size; i > Size - 6; i--)
        {
            if (isVowel(Rep[i]))
            {
                Vowel1 = true;
                break;
            }
            Vowel1 = false;
        }
        for (int i = Size - 7; i > Size - 12; i--)
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
    LETTERS.pop_back();
    return LETTERS[LEN];
}

void shuffleBag(string Rep)
{
    for (auto i : Rep)
    {
        LETTERS.push_back(string(1, i));
        LEN++;
    }
    random_shuffle(LETTERS.begin(), LETTERS.end());
}

BOARD echangeLettre(BOARD Board, int Joueur)
{
    if (Board[Joueur][0].size() < 3)
    {
        cout << "Vous n'avez pas assez de lettre pour en échanger." << endl;
        return Board;
    }
    string Rep = "";
    string Res;
    while (Rep == "")
    {
        cout << "Votre vrac est actuellement : " << Board[Joueur][0] << endl
             << "De quelle lettre voulez-vous vous séparer ?" << endl;
        string Rep;
        cin >> Rep;
        Rep = purifie(Rep);
        if (Rep.size() != 3)
        {
            cout << "Vous devez choisir exactement 3 lettres." << endl;
            Rep = "";
        }

        Res = retire(Board[Joueur][0], Rep);
        if (Res == "-")
        {
            cout << "Vous ne disposez pas d'au moins une de ces lettres." << endl;
            Rep = "";
        }
    }
    Board[Joueur][0] = Res;
    for (int i = 0; i < 3; i++)
    {
        Board[Joueur][0] += piocheLettre();
    }
    cout << "Here" << endl;
    shuffleBag(Rep);
    return Board;
}

void ActionPossible()
{
    cout << "Au cours du jeu il est possible de taper certaines lettres pour piloter la manche." << endl;
    cout << "    A : Abandonner la Partie" << endl;
    cout << "    C : Continuer en choisissant un mot" << endl;
    cout << "    E : Echanger trois Lettres" << endl;
    cout << "    F : Finir son tour" << endl;
    cout << "    J : crier Jarnac" << endl;
    cout << "    M : voir ce Menu" << endl;
    cout << "    P : voir le Plateau" << endl;
    cout << endl;
}

BOARD JouerMot(BOARD Board, int Joueur, int Ligne, string Mot, string diffLetter, bool isJarnac)
{

    Board[Joueur][0] = retire(Board[Joueur][0], diffLetter);
    if (!isJarnac)
    {
        cout << "HRE" << endl;
        Board[Joueur][Ligne] = Mot;
        Board[Joueur][0] += piocheLettre();
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
    return Board;
}

BOARD PlaceMot(BOARD Board, int Joueur, vector<string> Dico, vector<long> Bornes, int SHIFT, int NBR, bool isJarnac, string Name, vector<string> Names)
{
    string Say = "Sur quelle ligne souhaitez-vous jouer ?";
    if (isJarnac)
    {
        Joueur = 1 - Joueur;
        Say = "Où pensez-vous voir un mot ?";
    }
    bool Joue = true;
    string Rep;
    string mot = "-";
    string CurrentLigne = "";
    string diffLetter;
    string Vrac = Board[Joueur][0];
    bool Continue;
    int Ligne;

    affichePlateaux(Board[0], Board[1], 8, 9, Name, Names[0], Names[1]);
    while (Joue)
    {
        Rep = "";
        while (Rep == "")
        {
            cout << Say << endl;
            cin >> Rep;
            if (Rep == "Q")
            {
                Joue = false;
                break;
            }
            if (("1" > Rep) || ("9" < Rep) || (Rep.size() == 2))
            {
                cout << "Numéro de ligne invalide." << endl;
                Rep = "";
            }
        }
        Ligne = atoi(Rep.c_str());
        CurrentLigne = Board[Joueur][Ligne];
        mot = "-";
        while (mot == "-")
        {
            cout << endl
                 << "Quel mot souhaitez-vous jouer ?" << endl;
            cin >> mot;
            mot = purifie(mot);
            if (mot == "R")
            {
                break;
            }
            if (mot == "Q")
            {
                Joue = false;
                break;
            }
            diffLetter = retire(mot, CurrentLigne);
            Continue = true;
            for (auto i : diffLetter)
            {
                if (Vrac.find(i) == string::npos)
                {
                    affichePlateaux(Board[0], Board[1], 8, 9, Name, Names[0], Names[1]);
                    cout << "Mot impossible a jouer. Taper 'R' pour choisir une autre ligne. Taper 'Q' pour changer d'action." << endl;
                    mot = "-";
                    Continue = false;
                    break;
                }
            }
            if (Continue)
            {
                if (!trouve(mot, Dico, Bornes, SHIFT, NBR))
                {
                    affichePlateaux(Board[0], Board[1], 8, 9, Name, Names[0], Names[1]);
                    cout << "Mot inexistant. Taper 'R' pour choisir une autre ligne. Taper 'Q' pour changer d'action." << endl;
                    mot = "-";
                    break;
                }
                else
                {
                    Board = JouerMot(Board, Joueur, Ligne, mot, diffLetter, isJarnac);
                    affichePlateaux(Board[0], Board[1], 8, 9, Name, Names[0], Names[1]);
                }
            }
        }
    }
    return Board;
}

string choixAction(BOARD Board, int Joueur, vector<string> Dico, vector<long> Bornes, int SHIFT, int NBR, string Name, vector<string> Names)
{
    string coup;
    affichePlateaux(Board[0], Board[1], 8, 9, Name, Names[0], Names[1]);
    while (true)
    {

        cout << "Quelle action souhaitez-vous effectuer ?" << endl;
        cin >> coup;
        coup = purifie(coup);
        cout << endl;
        if (coup == "C")
        {
            Board = PlaceMot(Board, Joueur, Dico, Bornes, SHIFT, NBR, false, Name, Names);
        }
        else if (coup == "F")
        {
            return "";
        }
        else if (coup == "J")
        {
            Board = PlaceMot(Board, Joueur, Dico, Bornes, SHIFT, NBR, true, Name, Names);
        }
        else if (coup == "E")
        {
            Board = echangeLettre(Board, Joueur);
        }
        else if (coup == "P")
        {
            affichePlateaux(Board[0], Board[1], 8, 9, Name, Names[0], Names[1]);
        }
        else if (coup == "M")
        {
            ActionPossible();
        }
        else if (coup == "A")
        {
            return "-STOP-";
        }
        else
        {
            affichePlateaux(Board[0], Board[1], 8, 9, Name, Names[0], Names[1]);
            cout << "Choix invalide, tapez 'M' pour voir les actions possibles." << endl;
        }
    }
    return "";
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

bool lanceLeJeu(string joueur0, string joueur1, string Name)
{
    string nomDico = choisirDictionnaire();
    // vector<string> Dico = importeDico(choisirDictionnaire());
    vector<string> Dico = importeDico("Guidé/DictionnairePurified.txt");
    int SHIFT = ceil(log(Dico.size()) / log(2));
    int NBR = 3;
    vector<long> BORNES = CreateBorne(Dico, NBR, SHIFT);

    BOARD Board = initBoard(8, 9);

    vector<string> Names = {joueur0, joueur1};

    affichePlateaux(Board[0], Board[1], 8, 9, Name, joueur0, joueur1);
    ActionPossible();
    cout << "Appuyez sur 'entré' pour continuer" << endl;
    system("PAUSE");
    getchar();

    // ATTENDRE INPUT D'UN JOUEUR AVANT DE CONTINUER

    int Joueur = 0;
    string mot = choixAction(Board, Joueur, Dico, BORNES, SHIFT, NBR, Name, Names);
    if (mot == "-STOP-")
    {
        cout << "Fin du jeu, " << Names[1 - Joueur] << " a gagné." << endl;
    }
    Joueur = 1 - Joueur;
    string mot = choixAction(Board, Joueur, Dico, BORNES, SHIFT, NBR, Name, Names);
    if (mot == "-STOP-")
    {
        cout << "Fin du jeu, " << Names[1 - Joueur] << " a gagné." << endl;
    }

    while (mot != "-STOP-")
    {
        affichePlateaux(Board[0], Board[1], 8, 9, Name, joueur0, joueur1);
        Joueur = 1 - Joueur;
        Board[Joueur][0] += piocheLettre();
        string mot = choixAction(Board, Joueur, Dico, BORNES, SHIFT, NBR, Name, Names);
    }

    cout << "Fin du jeu, " << Names[1 - Joueur] << " a gagné." << endl;

    return true;
}

int main()
{

    lanceLeJeu("Moi", "Toi", "----");
    return 0;
}