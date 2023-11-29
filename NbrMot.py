with open("Guidé/All/Text/DicoIA.txt") as T:
    data = T.readlines()

SortedData = dict([(9, 0), (8, 0), (7, 0), (6, 0), (5, 0), (4, 0), (3, 0)])

for i in data:
    SortedData[len(i)-1] += 1

print(SortedData)       # {9: 46720, 8: 35438, 7: 22632, 6: 12111, 5: 5271, 4: 1829, 3: 491}