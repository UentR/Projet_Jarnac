def Sort(mot):
    return "".join(sorted(mot))


import json 
with open("Guidé/All/Text/DicoIA.txt") as T:
    data = T.readlines()

with open("Guidé/All/Text/Correspondance.json") as T:
    Corr = json.load(T)

SortedData = dict([(10, dict()), (9, dict()), (8, dict()), (7, dict()), (6, dict()), (5, dict()), (4, dict()), (3, dict())])
for value in data:
    value = value[:-1]
    Idx = len(value)
    SortedData[Idx][Sort(value)] = 1

# for key, value in SortedData.items():
#     SortedData[key] = dict(sorted(value.items(),key=lambda x:x[0]))

Alphabet = list(map(chr, range(65, 91)))

from time import perf_counter as pf


TEMP = [1]
times = dict.fromkeys(TEMP, 0)
for NBR in TEMP:
    Response = []
    deb = pf()
    for Idx in range(9, 2, -1):
        print(Idx)
        for I, value in enumerate(SortedData[Idx].keys()):
            Temp = f"{Corr[value]} "
            for letter in Alphabet:
                Mot = Sort(value+letter)
                if SortedData[Idx+1].get(Mot, 0):
                    Temp += letter + " "
            Temp += ";\n"
            Response.append(Temp)
    fin = pf() - deb
    times[NBR] = fin

print(times)

# 25 LETTERS : URATES TERSA SALE
# 23 LETTERS : SIERONT ALE
# 22 LETTERS :TOARCIEN TENTERAI TUERIONS TRIATES RESALE REMISA SIERA TROUE SAIN SERA TARE SORE TALE RADE USER NIE 

# print(SortedData)


with open("Guidé/All/Text/Arbre.txt", "w") as T:
    T.writelines(Response)