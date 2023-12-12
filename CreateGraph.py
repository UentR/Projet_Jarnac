NbrMotExiste = 0
NbrMotFaux = 0
CurrentMot = ""
TimeExiste1 = 0
TimeFaux1 = 0
TimeExiste2 = 0
TimeFaux2 = 0
TimeExiste3 = 0
TimeFaux3 = 0
AllExiste = 0
AllFaux = 0

dico = {}
with open("Project/Text/DictionnairePurified.txt") as R:
    for line in R:
        dico[line.rstrip()] = 1

Existe = False
Count = 0
with open("Project/Text/Timing.txt") as T:
    for line in T:
        if line.startswith("Word"):
            CurrentMot = line.split()[-1]
            Existe = CurrentMot in dico
            if Existe:
                NbrMotExiste += 1
            else:
                NbrMotFaux += 1
        if line.startswith("Dict1"):
            if Existe:
                TimeExiste1 += float(line.split()[-1])
            else:
                TimeFaux1 += float(line.split()[-1])
        if line.startswith("Dict2"):
            if Existe:
                TimeExiste2 += float(line.split()[-1])
            else:
                TimeFaux2 += float(line.split()[-1])
        if line.startswith("Dict3"):
            if Existe:
                TimeExiste3 += float(line.split()[-1])
            else:
                TimeFaux3 += float(line.split()[-1])
        if line.startswith("All"):
            if Existe:
                AllExiste += float(line.split()[-1])
            else:
                AllFaux += float(line.split()[-1])

print("Nombre de mots existants : ", NbrMotExiste)
print("Nombre de mots faux : ", NbrMotFaux)
print("Temps total existant : ", TimeExiste1)
print("Temps total faux : ", TimeFaux1)
print("Temps total existant : ", TimeExiste2)
print("Temps total faux : ", TimeFaux2)
print("Temps total existant : ", TimeExiste3)
print("Temps total faux : ", TimeFaux3)
print("Temps total existant : ", AllExiste)
print("Temps total faux : ", AllFaux)