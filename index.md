---
jupytext:
  notebook_metadata_filter: exports, myst
  text_representation:
    extension: .md
    format_name: myst
    format_version: 0.13
kernelspec:
  display_name: C++17
  language: C++17
  name: xcpp17
---

+++ {"deletable": false, "editable": false, "nbgrader": {"cell_type": "markdown", "checksum": "41aba7a0020588de7ce1c97559c7aedd", "grade": false, "grade_id": "cell-023e4da63cee5c06", "locked": true, "schema_version": 3, "solution": false, "task": false}}

# Projet : Jeu JARNAC

JARNAC est un jeu de société pour deux personnes qui se joue avec un
sac opaque, 144 lettres et, pour chaque joueur, un plateau de 8 lignes
de 9 cases. Les lettres sont ainsi distribuées : 14 A, 4 B, 7 C, 5 D,
19 E, 2 F, 4 G, 2 H, 11 I, 1 J, 1 K, 6 L, 5 M, 9 N, 8 O, 4 P, 1 Q, 10
R, 7 S, 9 T, 8 U, 2 V, 1 W, 1 X, 1 Y et 2 Z.  La règle est décrite
dans
[ce document](https://Nicolas.Thiery.name/Enseignement/Info111/Projet-Jarnac/media/RègleJarnac.pdf).

:::{image} media/JarnacVueEnModeTexte.jpg
:alt: Une vue simple du jeu JARNAC
:::


Le but de ce projet est de développer en binôme une application pour
permettre à une ou deux personnes positionnées devant le même
ordinateur de jouer à JARNAC sans matériel physique de jeu, en
utilisant le dictionnaire de noms communs fourni dans le fichier
[dictionnaire.txt](dictionnaire.txt).

+++

## Règle de JARNAC simplifiée

Pour permettre un jeu plus fluide, et par ce que les erreurs de frappe
sont toujours possibles, il est accepté que la validation des mots par
le dictionnaire soit automatique et systématique et qu'aucune
contestation ou pénalité en cas de mot non valide ne soit gérée par
votre application.

Toutes les autres fonctionnalités du jeu doivent être implantée.

Note: comme dans le jeu de Scrabble, tous les mots sont écrits en
majuscules non accentuées.

+++

## Moteurs de jeu et Interfaces utilisateurs

Le moteur (ou modèle) d'un jeu est en charge de la gestion des données
et de la logique du jeu.

L'interface utilisateur d'une application comprend la **vue** (ce que
le joueur voit à l'écran) et le **contrôleur** (comment le joueur
interagit avec la machine).

:::{image} media/JarnacVueEnModeTexte.jpg
:alt: Une vue minimaliste fournie pour le jeu JARNAC
:::

+++

## Consignes

Votre application devra permettre à deux utilisateurs de jouer l'un
contre l'autre. La qualité du jeu (documentation, interface, ...) doit
permettre une prise en main aisée par des novices.

Votre moteur de jeu devra gérer toutes les règles du jeu de JARNAC
simplifié: structures de données, opérations sur ces structures de
données, calcul des points, etc.

Votre application devra pouvoir fonctionner a minima avec la vue
minimaliste qui vous est fournie. Une interface utilisateur est
jouable si elle permet aisément à un non initié de jouer et d'être
guidé dans le jeu.

Le moteur du jeu devra être pour commun à toutes les interfaces.

Pour une compatibilité avec la vue fournie, le plateau d'un joueur
devra être représenté par (ou pouvoir être converti en) un tableau de
chaînes de caractères, tel que la premiere chaîne représente les
lettres en vrac et les suivantes les mots déjà posés.

Vous utiliserez autant que possible la compilation séparée. À minima,
il devra y avoir un fichier pour le moteur du jeu, un autre pour la
gestion du dictionnaire, un par contrôleur et un par vues. Pour la vue
minimaliste un fichier [vueEnModeTexte.hpp](vueEnModeTexte.hpp) est fournie.

+++

## $\clubsuit$ Extensions

Une fois le cœur du projet fonctionnel, vous pourrez aborder une ou
plusieurs extensions. Nous vous en proposons deux ici, mais vous
pouvez aussi en choisir de votre cru.

### Intelligence Artificielle

Pour cette extension, nous vous proposons de développer une *IA* (pour
intelligence artificielle) pour votre jeu, c'est à dire un composant
logiciel simulant un deuxième joueur pour permettre à un utilisateur
de jouer contre l'ordinateur.

Pour JARNAC, une bonne IA doit pouvoir jouer chaque fois qu'un coup
est possible. Face à une telle IA aucun JARNAC ne serait alors
possible. Pour rendre le jeu plus intéressant face à l'IA, on peut par
exemple affaiblir cette IA en la faisant jouer avec un dictionnaire
incomplet.

### Interface graphique

+++

## Fichiers fournis

Sont fournis les fichiers suivants.

### Documentation

- [index.md](index.md): cet énoncé
- [suggestionDeProgression.md](suggestionDeProgression.md): une suggestion de progression
- [dictionnaire.txt](dictionnaire.txt): un dictionnaire

### Code

Les fichiers suivants donnent une proposition de découpage de votre
code en bibliothèques (voir la [suggestion de
progression](suggestionDeProgression.md)). Vous pouvez aussi choisir
de faire votre propre découpage.

### Bibliothèques

- [mEquivalence.cpp](mEquivalence.cpp), [mEquivalence.hpp](mEquivalence.hpp), [mEquivalence-test.cpp](mEquivalence-test.cpp)
- [mAnagrammes.cpp](mAnagrammes.cpp), [mAnagrammes.hpp](mAnagrammes.hpp), [mAnagrammes-test.cpp](mAnagrammes-test.cpp)
- [mCalculPoints.cpp](mCalculPoints.cpp), [mCalculPoints.hpp](mCalculPoints.hpp), [mCalculPoints-test.cpp](mCalculPoints-test.cpp)
- [mDictionnaire.cpp](mDictionnaire.cpp), [mDictionnaire.hpp](mDictionnaire.hpp)
- [vueEnModeTexte.cpp](vueEnModeTexte.cpp), [vueEnModeTexte.hpp](vueEnModeTexte.hpp)
- [controleurChoixAction.cpp](controleurChoixAction.cpp), [controleurChoixAction.hpp](controleurChoixAction.hpp)

#### Utilitaires

- [IllustrationInterface.cpp](IllustrationInterface.cpp)

+++

## Suggestion de progression

Le fichier [suggestionDeProgression.md](suggestionDeProgression.md) contient une suggestion de
progression pour aborder ce projet. Vous êtes libres de la suivre ou
pas.

+++

## Barème

L'évaluation de votre projet prendra en compte les éléments suivants:

### Soutenance et rapport

- Suivi des consignes
- Qualité et pertinence
- Aisance

### Règles de codage

- organisation : séparation entre codage du moteur et codage de l'interface
- organisation : fonctions courtes avec entrées/sorties pertinentes

- présentation : indentation correcte
- présentation : présence d'une compilation séparée d'au moins cinq fichiers différents
- présentation : choix pertinents des noms de variables et de fonctions
 
- qualité : lisibilité du code des fonctions
- qualité : présence de tests unitaires pour toutes les fonctions du moteur de jeu
- qualité : absence de redondance (sauf en cas de changement de
  bibliothèque)
 
- documentation : présence d'un descriptif de chaque fonction
- documentation : présence d'un mode d'emploi joueur et utilisateur

### Règles de fonctionnement du binôme

- équilibre : répartition équitable du travail au sein du binôme
- équilibre : répartition de la prise de parole lors de la soutenance
 
- sérieux : respect des délais de depôt des documents. 
- sérieux : répartition temporelle du travail (éviter le travail en dernière minute)

### Efficacité de la programmation

- points : calcul correct des points
- pioche : gestion cohérente des pioches
- confort : jouabilité (respect des règles du jeu et accompagnement des joueurs) 
 
- dictionnaire : recherche correcte d'un mot dans le dictionnaire fourni
- dictionnaire : possibilité d'employer un autre dictionnaire

- $\clubsuit$ proposition d'extension(s) (définir le jeu à plus de
  joueurs, joueur avec d'autres règles etc. )

### Gestion des interactions avec l'utilisateur :
 
- compatibilité : interaction possible avec l'interface utilisateur minimaliste fournie
- réutilisabilité : cohabitation de deux interfaces utilisateurs avec un moteur commun
- $\clubsuit$ confort de la recherche du mot pour le joueur avant validation (chevalet)

### Extensions

#### $\clubsuit$ Interface graphique

#### $\clubsuit$ Intelligence Artificielle (IA)

- Existence d'une IA qui trouve une solution s'il y en a une. Modes de
  jeu Humain contre humain, humain contre IA, IA contre IA. Affaiblir
  l'IA en la faisant jouer avec un dictionnaire incomplet.
- améliorer l'efficacité de l'IA en évitant de répéter des calculs.
- améliorer la performance de l'IA en lui faisant choisir de meilleurs coups.
