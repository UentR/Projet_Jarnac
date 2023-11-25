---
jupytext:
  text_representation:
    extension: .md
    format_name: myst
    format_version: 0.13
kernelspec:
  display_name: C++17
  language: C++17
  name: xcpp17
---

# Suggestion de progression

% Pour compiler il faut faire pour chaque fichier.cpp:
%  
% 1.  clang++ -c fichier.cpp
%     cela créé fichier.o dans le même repertoire,
%  
% ensuite il suffit de suivre le cours
% sur la compilation séparée de info111 pour savoir
% quelles instructions taper exactement (à savoir pour l’exemple).
%  
% 1.  clang++ IllustrationInterface.o mEquivalence.o vueEnModeTexte.o -o illustration
%     clang++ mEquivalence-test.cpp.o mEquivalence.o -o testEquivalence
%     dans un premier temps pour préparer les exécutables
% 2.  ./nomExecutable pour exécuter le programme qui contient la fonction int main().
%  
% Ainsi la suite d’instructions :
%  
% clang++ -c mEquivalence.cpp
% clang++ -c mEquivalence-test.cppEnrichi.cpp
% clang++ mEquivalence.o mEquivalence-test.cppEnrichi.o -o testEquivautA
% ./testEquivautA
%  
% permet de savoir si votre test enrichi passe ou non.
%  
% et si on la complete par :
% clang++ -c IllustrationEquivalence.cpp
% clang++ -c vueEnModeTexte.cpp
% clang++ -c mCalculPoints.cpp
% clang++ mEquivalence.o IllustrationEquivalence.o vueEnModeTexte.o mCalculPoints.o -o illustre

+++

## Partie I : COMPRENDRE le trio Spécification Test Codage

:::{admonition} Exercice 1

1.  Consultez la documentation de la fonction `calculPoints` dans le
    fichier [mCalculPoints.hpp](mCalculPoints.hpp).
2.  Copiez cette documentation dans un nouveau fichier
    `mCalculPoints.cpp`.
3.  Consultez les tests de la fonction `calculPoints` dans le fichier
    [mCalculPoints-test.cpp](mCalculPoints-test.cpp).
4.  Copiez ces tests, et la fonction `main` qui les contient, dans le
    fichier `mCalculPoints.cpp`.
5.  Écrivez dans `mCalculPoints.cpp` une première implantation de
    `calculsPoints` qui renvoie toujours la constante 100, c’est à
    dire sans respecter la spécification indiquée dans l'entête ni
    dans la règle du JARNAC.
6.  Compilez le fichier `mCalculPoints.cpp`. Il vous faudra
    probablement le compléter en récupérant des lignes comme `#include
    <vector>` ou `# define CHECK ...` depuis
    [mCalculPoints-test.cpp](mCalculPoints-test.cpp).
7.  Exécutez le programme obtenu pour tester votre fonction et mettre
    en évidence que certains tests échouent.
8.  Complétez par petites étapes l'implantation de la fonction
    `calculPoints` afin qu'elle respecte sa spécification; vérifiez à
    chaque étape quels tests passent ou non; cela vous donnera des
    indications ce que vous aurez à faire.

:::

+++

## Partie II : INTERAGIR avec des fichiers

::::{admonition} Exercice 2

1.  Créer un dictionnaire de test avec seulement 20 mots dedans.

2.  À l’aide de ce dictionnaire, écrire dans le fichier
    [mDictionnaire.cpp](mDictionnaire.cpp) des tests pour les fonctions documentées
    dans le fichier [mDictionnaire.hpp](mDictionnaire.hpp).

3.  Implantez ces fonctions dans [mDictionnaire.cpp](mDictionnaire.cpp). Compilez et
    vérifiez au fur et à mesure quels tests passent ou pas.

    :::{hint} Indication
	
	Pour la fonction purification, vous pourrez vous inspirer des
    exemples ci-dessous
	
	:::

::::

```{code-cell}
char lettre = 'c';
char(lettre + 'A' - 'a')
```

```{code-cell}
'c' < 'e'
```

```{code-cell}
'b' < 'a'
```

```{code-cell}
lettre = '@';
'a' < lettre and lettre < 'z'
```

## Partie III : CODER avec un cadre

:::{admonition} Exercice 3

1.  En procédant comme dans l'exercice 1, implanter dans un fichier
    `mAnagrammes.cpp` les fonctions dont vous trouverez la
    documentation dans [mAnagrammes.hpp](mAnagrammes.hpp) et les tests dans
    [mAnagrammes-test.cpp](mAnagrammes-test.cpp).

:::

+++

## Partie IV : compilation séparée

:::{caution} 

Nous verrons la compilation séparée en Semaine 10. Si vous abordez
cette partie du projet avant cela, passez pour le moment directement à
aux parties V et suivantes

:::

:::{admonition} Exercice 4

1.  En vous inspirant du TP de la Semaine 10, nettoyez les fichiers
    `mCalculPoints.cpp`, `Dictionnaire.cpp`,
    `mAnagrammes.cpp` pour en faire des bibliothèques, avec
    leurs fichiers d'entêtes et de tests respectifs. Veillez à
    compiler et vérifier la bonne exécution des tests.

2.  Consultez le programme [ExempleInterface.cpp](ExempleInterface.cpp). Quelles
    bibliothèques utilise-t-il?
	
3.  Compilez ce programme avec ces bibliothèques et exécutez le.

:::

:::{hint} Indication: compilation automatisée

Compiler à la main des programmes devient vite fastidieux.  Nous vous
avons fourni un fichier [Makefile](Makefile) qui permet d'automatiser la
procédure avec la commande `make`. Essayez la commande suivante pour
faire un peu de nettoyage :

```
make clean
```

Puis:

```
make ExempleInterface
```

Vous pouvez consulter le [Makefile](Makefile) pour voir comment est configurée
la compilation pour le programme `IllustrationInterface`. Par la
suite, vous pourrez, si vous le souhaitez, adapter le `Makefile` pour
vos propres programmes.

:::

+++

## Partie V : COMPRENDRE le trio Spécification-Test-Codage : L’écriture d’un test fiable

:::{admonition} Exercice 4

Un plateau est affiché sur une grille de hauteur $h$ et largeur $l$.
Seuls sont affichés les $h$ premiers mots du plateau, tronqués à leur
$l$ premières lettres, ainsi que le vrac.

Deux plateaux sont dits **équivalents** s'ils ont la même
représentation pour le joueur; c'est-à-dire:

- leur vrac est le même à permutation près
- les $l$ premiers mots sont identiques et dans le même ordre, en
  considérant les mots trop courts (deux lettres ou moins) ou trop
  long (l+1 lettres ou plus) comme s'ils étaient vides.

Il sera utile par la suite de déterminer si deux plateaux sont
équivalents ou non. Cet exercice va vous guider pour spécifier, tester
puis implanter ce test d'équivalence.

1.  Consultez le programme [IllustrationInterface.cpp](IllustrationInterface.cpp). Quelles
    bibliothèques utilise-t-il?
	
2.  Compilez ce programme avec ces bibliothèques et exécutez le.

3.  Comparez l’affichage écran de ce fichier avec le code des tests
    dans le fichier [mEquivalence-test.cpp](mEquivalence-test.cpp).

4.  Complétez [mEquivalence-test.cpp](mEquivalence-test.cpp) avec un un test enrichi pour la
    fonction equivautA.

5.  Implantez la fonction `equivautA` dans le fichier
    [mEquivalence.cpp](mEquivalence.cpp), jusqu'à ce qu’elle passe les deux tests.

:::


## Partie VI : CODER en réutilisant

:::{admonition} Exercice 11

Utilisez toutes les biliothèques précédentes pour coder votre jeu de
JARNAC en utilisant la vue utilisateur fournie. Vous pourrez par
exemple implanter dans [controleurChoixAction.cpp](controleurChoixAction.cpp) un contrôleur tel
que spécifié [controleurChoixAction.hpp](controleurChoixAction.hpp), puis combiner le tout. Il
vous faudra aussi compléter le modèle.

:::

+++

## Partie VII : CODER librement

:::{admonition} Exercice 12

Complétez votre code pour proposer les extensions du jeu (autre
interface utilisateur, IA, etc. )

:::
