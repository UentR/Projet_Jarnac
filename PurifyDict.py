with open("dictionnaire.txt") as T:
    data = T.readlines()

NewData = []
for i in data:
    if 3 <= len(i) - 1 <= 9:
        NewData.append(i)

print((len(NewData)-len(data))/len(data)*100)

with open("Guidé/All/Text/DictionnairePurified.txt", "w") as T:
    T.writelines(NewData)