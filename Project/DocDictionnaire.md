# Documentation dictionnaire


### Création des bornes de recherche :

> ```C++
> int SHIFT = ceil( log(Dico.size()) / log(2) );
> long Borne = (BorneSup << SHIFT) + (BorneInf);
> ```


**Regardons étapes par étapes**

1. **Calcul du Bitshift :**

    - Approximation de la taille en binaire de la valeur maximale possible :
        > ```C++
        > log(Dico.size()) / log(2)
        > ```
    - Arrondi de la valeur :
        > ```C++
        > int SHIFT = ceil( log(Dico.size()) / log(2) );
        > ```
    - Dans le dictionnaire de mots entre 3 et 9 lettres cette valeur est de **18**, c'est celle qui sera utilisée dans les exemples. La valeur maximale est de **32** car 33\*2 > 64, la taille d'un long. Le dictionnaire peut donc avoir une taille maximum de **_8 589 934 591_** mots.
      <br>

2. **Stockage des bornes en une seule variable :**

    - On part du principe que l'on a déjà les valeurs de BorneSup et BorneInf dans des variables entières.

    - Grâce au calcul du **SHIFT** on sait quelle sera la taille maximale en binaire de BorneInf. En décalant BorneSup de **SHIFT** vers la gauche on peut stocker dans une seule variable de 64bits à la fois BorneSup puis BorneInf :

        > ```C++
        > long Borne = (BorneSup<<SHIFT) + (BorneInf)
        > ```

    - Si BorneSup est 169080 et BorneInf est 169030 comme cela est le cas pour le couple **ZY** on a :
        > ```C++
        >   BorneSup = |1|0|1|0|0|1|0|1|0|0|0|1|1|1|1|0|0|0|                        				= 169080
        >   BorneInf =                                     |1|0|1|0|0|1|0|1|0|0|0|1|0|0|0|1|1|0| 	= 169030
        >   Borne    = |1|0|1|0|0|1|0|1|0|0|0|1|1|1|1|0|0|0|1|0|1|0|0|1|0|1|0|0|0|1|0|0|0|1|1|0| 	= (169080 << 18) + (169030)
        > ```


### Décodage des bornes :

> ```C++
> long Borne = BORNES[Idx];
> int BorneInf = Borne & int(pow(2, SHIFT) - 1);
> int BorneSup = Borne >> SHIFT;
> ```


**Regardons étapes par étapes**

1.  **Calcul de l'index :**

    -   On utilise les [3](Rapport.md#démonstration) premières lettres des mots pour retrouver la position des bornes dans notre vecteur qu'on convertira en un entier.
    -   On considère ensuite ces 3 lettres comme un nombre en base 26 à convertir en base 10.
    -   Prenons par exemple _NIU_, ici on a :
        > ```C++
        > int Idx = ('N' - 65) * 26*26 + ('I' - 65) \* 26 + ('I' - 65); // 9016
        > ```

2.  **Calcul de BorneInf :**

    -   On sait que BorneInf est stocké sur les **SHIFT** premiers bits de la borne. On cherche ainsi à se débarrasser de tous les autres. On va donc utiliser l'opérateur binaire & entre Borne et (2^SHIFT) - 1 afin de ne garder que les **SHIFT** premiers bits :
        > ```C++
        >   int BorneInf = Borne & (int)(pow(2, SHIFT) - 1);
        > ```

3.  **Calcul de BorneSup :**
    -   On sait également que du 2 \* **SHIFT**-ème bit jusqu'au **SHIFT**-ème bit se trouve BorneSup :
        > ```C++
        >   int BorneSup = Borne >> SHIFT;
        > ```
