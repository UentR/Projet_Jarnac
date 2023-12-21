## Documentation de l'IA

#### Création de la structure en arbre :

Un [fichier](./Text/Arbre.txt) contenant l'ensemble des mots disponibles pour l'IA a été créé avec d'un côté le mot et de l'autre les lettres qu'il est possible d'ajouter à ce mot pour en créer un nouveau.
On peut ensuite facilement charger ce fichier dans une structure en arbre pour optimiser la recherche par la suite.

> ```C++
> void LoadTree(AI *AIHelper, string FileName) {
> 	ifstream File;
> 	Node *T;
> 	string letter;
> 	File.open(FileName);
> 	T = new Node;
> 	while (File >> T->Ana) {
> 		File >> letter;
> 		while (letter != ";") {
> 			T->Children[letter[0]] = AIHelper->NodeDict[Sort((T->Ana + letter))];
> 			File >> letter;
> 		}
> 		AIHelper->NodeDict[Sort(T->Ana)] = T;
> 		T = new Node;
> 	}
> 	File.close();
> }
> ```

<br>

**Regardons étapes par étapes**

1.  **Initialisation de la recherche :**

    -   On ouvre le fichier recherché, crée le premier noeud et initialise la lettre qu'on utilisera pour chercher les noeuds enfants :
        > ```C++
        > ifstream File;
        > File.open(FileName)
        > Node *T;
        > T = new Node;
        > string letter;
        > ```

2.  **Lecture ligne à ligne :**

    -   Le fichier étant de format :
        > Mot Lettre1 Lettre2 ... ;
    -   On peut mettre le premier mot de chaque ligne comme nom de notre noeud puis placer chaque lettre et son noeud correspondant dans une map :
        > ```C++
        > while (File >> T->Ana) {	// On place le premier mot dans notre noeud
        > 	File >> letter; // On initialise la première lettre
        > 	while (letter != ";") {	// Tant que nous ne sommes pas à la fin de la ligne on continue d'ajouter des enfants
        > 		T->Children[letter[0]] = AIHelper->NodeDict[Sort((T->Ana + letter))];	// Recherche du noeud enfant correspondant
        > 		File >> letter;
        > 	}
        > 	AIHelper->NodeDict[Sort(T->Ana)] = T;	// On place le noeud que l'on vient de créer dans notre map de noeud
        > 	T = new Node;	// On crée un nouveau noeud
        > }
        > ```

3.  **Zoom sur la map de noeud :**
    -   Comme plusieurs mots peuvent avoir les mêmes lettres mais dans des ordres différents ils auront aussi les mêmes enfants et formeront donc des noeuds identiques.
    -   Pour compenser ce problème on ne va garder qu'un exemple de chaque couple de mots qui ont les mêmes lettres.
    -   Cette idée marcherait pour deux IA qui jouent l'une contre l'autre car elles connaîtront les mêmes mots mais si le joueur place une des versions d'un mot que l'on a pas sauvegardé notre IA ne trouvera jamais le noeud correspondant.
    -   Pour résoudre ce problème on va trier les lettres du mot pour que chaque versions correspondent au même index.
    -   On se retrouve donc avec:
        > ```C++
        > AIHelper->NodeDict[Sort((T->Ana + letter))] // Pour chercher les enfants
        > AIHelper->NodeDict[Sort(T->Ana)] = T // Pour placer notre noeud dans le dictionnaire
        > ```

<br>

#### Recherche de mot à allonger :

La première étape de chaque tour de l'IA est de regarder les mots qu'elle peut allonger. C'est un choix arbitraire que j'ai fait après avoir comparé différentes versions d'IA. Pour ça nous allons créer un set de tous les mots à analyser que nous agrandirons à chaque noeud enfant que l'on peut atteindre avec notre vrac.

> ```C++
> set<tuple<Node *, string, string, Node *> > toAnalyze = {};
> for (Node *Word : PlayerWords) {
> 	toAnalyze.insert(make_tuple(Word, Vrac, "", Word));
> }
> while (!toAnalyze.empty()) {
> 	tie(Word, tVrac, Path, Origin) = *toAnalyze.begin();
> 	toAnalyze.erase(toAnalyze.begin());
> 	Child = false;
> 	for (char Letter : tVrac) {
> 		if (Word->Children.find(Letter) != Word->Children.end()) {
> 			Child = true;
> 			toAnalyze.insert(make_tuple(Word->Children[Letter], chRetire(tVrac, Letter), Path + Letter, Origin));
> 		}
> 	}
> 	if (!Child) {
> 	End.insert(make_tuple(Origin, Path));
> 	}
> }
> ```

<br>

**Regardons étapes par étapes**

1. **Initialisation de notre set :**

    - On va placer tous les mots de l'IA (ou du joueur en cas de recherche de Jarnac) dans un tuple contenant le mot à analyser, le vrac à cette étape, le chemin pour arriver à cette étape, le mot d'origine.
        > ```C++
        > set<tuple<Node *, string, string, Node *> > toAnalyze = {};
        > for (Node *Word : PlayerWords) {
        > 	toAnalyze.insert(make_tuple(Word, Vrac, "", Word));
        > }
        > ```

2. **Parcours de notre set :**

    - On récupère les données du premier élément de notre set.
    - On parcours notre vrac à cette étape.
    - Si une des lettres permet d'atteindre un noeud enfant alors on l'ajoute à notre set avec notre vrac privé de la lettre, le chemin jusqu'à ce noeud avec la nouvelle lettre et le noeud sur le plateau qui permet d'arriver jusque ici.
    - Si aucune lettre ne permet d'atteindre un noeud enfant alors on a atteint le bout de cette branche et on place notre noeud dans un différent set qu'on utilisera simplement pour trouver le chemin le plus long.
    - On a donc :
        > ```C++
        > while (!toAnalyze.empty()) { // Tant qu'il reste des mots à analyser on continue
        > 	tie(Word, tVrac, Path, Origin) = *toAnalyze.begin(); // On récupère le premier élément de notre set
        > 	toAnalyze.erase(toAnalyze.begin()); // On l'enlève du set
        > 	Child = false; // On utilisera cette variable pour savoir si des noeuds enfants on été atteint
        > 	for (char Letter : tVrac) { // On parcourt notre vrac
        > 		if (Word->Children.find(Letter) != Word->Children.end()) { // Si un atteint un noeud enfant
        > 		Child = true;
        > 		toAnalyze.insert(make_tuple(Word->Children[Letter], chRetire(tVrac, Letter), Path + Letter, Origin));	// On place toutes les informations nécessaires à la recherche
        > 		}
        > 	}
        > 	if (!Child) { // Si aucun enfant n'a été atteint
        > 		End.insert(make_tuple(Origin, Path)); // On place le noeud sur le plateau et le chemin dans un autre set
        > 	}
        > }
        > ```

3. **Choix du noeud :**
    - Il s'agit finalement simplement de trouver le chemin le plus long dans notre set final et de renvoyer le mot correspondant sur le plateau et ce chemin.

<br>

#### Recherche de mot à créer :

On va regarder toutes les permutations de notre vrac dans un ordre croissant d'éléments afin de trouver le plus petit mot que l'on peut placer sur le plateau (ou dans un ordre décroissant d'élément en cas de Jarnac pour trouver le plus grand mot que l'on peut voler).

> ```C++
> set<string> FoundWords = {};
> string SortedWord;
> bool StopSearch = false;
>
> // Cas très rare mais si le vrac fait plus de 9 lettres cela ne sert à rien
> // de chercher les permutations de 10 lettres car aucun mot de 10 lettres
> // ne pourra être placer
> int Len = min((int)Vrac.length(), 9);
> int I;
> for (int i = 3; i <= Len; i++) {
> 	if (Jarnac) {
> 		I = (Len + 3) - i;
> 	} else {
> 		I = i;
> 	}
> 	set<string> Permutations = DictPermutations(Vrac, I);
> 	for (string Permutation : Permutations) {
> 		if (Words.find(Permutation) != Words.end()) {
> 		FoundWords.insert(Words[Permutation]);
> 		StopSearch = true;
> 		}
> 	}
> 	if (StopSearch) {
> 		break;
> 	}
> }
> ```

<br>

**Regardons étapes par étape**

1. **Parcours des longueurs de permutations :**

    - On parcourt la longueur du vrac (en cas de Jarnac dans un ordre décroissant).
    - On crée toutes les permutations possibles à cette longueur.
        > ```C++
        > for (int i = 3; i <= Len; i++) {
        > 	if (Jarnac) {
        > 	    I = (Len + 3) - i;
        > 	} else {
        > 		I = i;
        > 	}
        > 	set<string> Permutations = DictPermutations(Vrac, I);
        > }
        > ```

2. **Parcours des permutations :**
    - On parcourt toutes les permutations et on s'arrête si des mots ont été trouvé.
        > ```C++
        > for (string Permutation : Permutations) { // Parcours des permutations
        > 	if (Words.find(Permutation) != Words.end()) { // Verification de la validité du mot dans notre dictionnaire
        > 		FoundWords.insert(Words[Permutation]); // S'il existe l'ajouter à notre set
        > 		StopSearch = true; // On n'a pas à regarder des permutations de taille différente, on a déjà notre solution optimale
        > 	}
        > }
        > ```
