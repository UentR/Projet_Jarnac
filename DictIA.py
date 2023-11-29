def sortLetters(Mot):
    return "".join(sorted(Mot))



with open("Guidé/All/Text/DictionnairePurified.txt") as T:
    data = T.readlines()

Anagrames = list()
Mot = list()

Total = dict()

for Idx, i in enumerate(data):
    I = sortLetters(i[:-1])
    if I in Anagrames:
        pass
    else:
        Total[I] = i[:-1]
    # print(Idx/1700)


import json
with open("Guidé/All/Text/Correspondance.json","w") as T:
    json.dump(Total, T, indent=2)