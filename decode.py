def Decode(Nbr):
    t = bin(Nbr)[2:]
    Existe = t[-1]
    BorneInf = int(t[-19:-1], 2)
    BorneSup = int(t[-39:-21], 2)
    print(BorneSup)

Decode(18446744072511360295)