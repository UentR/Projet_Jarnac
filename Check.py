import matplotlib.pyplot as plt
import numpy as np

with open('Val.txt') as T:
    data = T.readlines()

Vals = np.ndarray(shape=len(data))
Infs = np.ndarray(shape=len(data), dtype=np.int32)
Sups = np.ndarray(shape=len(data), dtype=np.int32)
NInfs = np.ndarray(shape=len(data), dtype=np.int32)
NSups = np.ndarray(shape=len(data), dtype=np.int32)
Idxs = np.ndarray(shape=len(data), dtype=np.int32)

for idx, i in enumerate(data):
    i = i.replace("\n", "")
    Val, Inf, Sup, First = i.split(":")

    Val = int(Val)

    inf = (Val>>1)&(2**18-1)

    sup =0
    
    Vals[idx] = Val
    Infs[idx] = inf
    Sups[idx] = sup
    NInfs[idx] = Inf
    NSups[idx] = Sup
    Idx = (ord(First[0])-65)*26 + ord(First[1])-65
    Idxs[idx] = Idx

Inf = np.array((Idxs, Infs)).T
#Sup = np.array((Idxs, Sups)).T

plt.scatter(Idxs, Infs)
plt.scatter(Idxs, Sups)
plt.show()