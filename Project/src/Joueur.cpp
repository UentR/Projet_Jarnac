tuple<BOARD, bool> echangeLettre(BOARD Board, int Joueur,
                                 StoreLetters *LETTERS) {
  writeToDebugFile("echangeLettre", INFO_DETAIL);
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
      writeToDebugFile("LettresAEchanger.size() != NB_LETTRES_ECHANGE",
                       ALL_LOG);
    }

    nouveauVrac = retire(Board[Joueur][0], LettresAEchanger);
    if (nouveauVrac == "-") {
      cout << "Vous ne disposez pas d'au moins une de ces lettres." << endl;
      LettresAEchanger = "";
      writeToDebugFile("Choix de lettres invalides", ALL_LOG);
    }
  }

  Board[Joueur][0] = nouveauVrac;
  for (int i = 0; i < NB_LETTRES_ECHANGE; i++) {
    Board[Joueur][0] += piocheLettre(LETTERS);
  }
  shuffleBag(LETTERS, LettresAEchanger);
  writeToDebugFile("echangeLettre end", INFO_DETAIL);
  return make_tuple(Board, true);
}

BOARD piocheOuEchange(BOARD Board, int Joueur, StoreLetters *LETTERS) {
  writeToDebugFile("piocheOuEchange", INFO_DETAIL);
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
      writeToDebugFile("piocheOuEchange end avec pioche", INFO_DETAIL);
      return Board;
    } else if (RepJoueur == "E") {
      tie(Board, EchangeReussi) = echangeLettre(Board, Joueur, LETTERS);
      if (EchangeReussi) {
        writeToDebugFile("piocheOuEchange end avec echange", INFO_DETAIL);
        return Board;
      } else {
        writeToDebugFile("Echange raté", ALL_LOG);
      }
    } else {
      cout << "Choix invalide." << endl;
      writeToDebugFile("Choix invalide", ALL_LOG);
    }
  }
}

BOARD PlaceMot(BOARD Board, int Joueur, ForDict *DictHelper, bool isJarnac,
               Names *NamesHelper, StoreLetters *LETTERS) {
  writeToDebugFile("PlaceMot", INFO_DETAIL);
  string Say = "Sur quelle ligne souhaitez-vous jouer ?";
  if (isJarnac) {
    Say = "Où pensez-vous voir un mot ?";
  }
  string ligneTxt;
  string mot;
  string CurrentLigne = "";
  string diffLetter;
  string Vrac = Board[Joueur][0];
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
        writeToDebugFile("PlaceMot end avec Q", INFO_DETAIL);
        return Board;
      }
      ligneTxt = purifieNbr(ligneTxt);
      if (ligneTxt.size() == 0) {
        cout << "Numéro de ligne invalide. Tapez 'Q' pour "
                "changer d'action."
             << endl;
        ligneTxt = "";
        writeToDebugFile("Numéro de ligne invalide", ALL_LOG);
      }
    }

    NumLigne = atoi(ligneTxt.c_str());
    if (isJarnac) {
      CurrentLigne = Board[1 - Joueur][NumLigne];
    } else {
      CurrentLigne = Board[Joueur][NumLigne];
    }
    // get word
    cout << endl << "Quel mot souhaitez-vous jouer ?" << endl;
    cin >> mot;
    mot = purifie(mot);
    if (mot == "R") {
      writeToDebugFile("PlaceMot restart avec R", ALL_LOG);
      continue;
    }
    if (mot == "Q") {
      writeToDebugFile("PlaceMot end avec Q", INFO_DETAIL);
      return Board;
    }
    diffLetter =
        retire(mot, CurrentLigne);  // Lettre à ajouter pour jouer ce mot

    writeToDebugFile("diffLetter : " + diffLetter, ERROR);

    // Setup de la structure Play
    Current->Word = mot;
    Current->DLetter = diffLetter;
    if (isJarnac) {
      Current->Ligne = getLine(Board, "", Joueur);
      Current->Origin = NumLigne;
    } else {
      Current->Ligne = NumLigne;
      Current->Origin = -1;
    }

    // Jouer le mot
    tie(Board, state) =
        JouerMot(Board, Joueur, Current, NamesHelper, DictHelper, LETTERS);
  }
  writeToDebugFile("PlaceMot end", INFO_DETAIL);
  return Board;
}

tuple<BOARD, string> choixAction(BOARD Board, int Joueur, ForDict *DictHelper,
                                 Names *NamesHelper, int Tour,
                                 StoreLetters *LETTERS) {
  writeToDebugFile("choixAction", INFO_DETAIL);
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
    writeToDebugFile("ChoixUtilisateur : " + ChoixUtilisateur, ALL_LOG);
    if (ChoixUtilisateur[0] == PlacerUnMot) {
      // Si le joueur veut placer un mot
      Board = PlaceMot(Board, Joueur, DictHelper, false, NamesHelper, LETTERS);
      dejaJoue = true;
    } else if (ChoixUtilisateur[0] == TentativeJarnac and Tour != 0) {
      // Si le joueur veut crier Jarnac
      if (dejaJoue) {
        writeToDebugFile("Joueur a déjà joué un mot", ALL_LOG);
        cout << "Vous avez déjà joué un mot." << endl;
      } else {
        Board = PlaceMot(Board, Joueur, DictHelper, true, NamesHelper, LETTERS);
      }
    } else if (ChoixUtilisateur[0] == FinTour) {
      // Si le joueur veut finir son tour
      writeToDebugFile("choixAction end avec FinTour", INFO_DETAIL);
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
      writeToDebugFile("choixAction end avec Abandonner", INFO_DETAIL);
      // Si le joueur veut abandonner
      return make_tuple(Board, "-STOP-");
    } else {
      // Si le joueur a fait un choix invalide
      affichePlateaux(Board[0], Board[1], NB_LIGNES_PLATEAU, TAILLE_MAX_MOT,
                      NamesHelper->NameGame, NamesHelper->Name1,
                      NamesHelper->Name2, Joueur, false);
      cout << "Choix invalide, tapez 'M' pour voir les actions "
              "possibles."
           << endl;
    }
  }
  writeToDebugFile("choixAction end", INFO_DETAIL);
  return make_tuple(Board, "");
}