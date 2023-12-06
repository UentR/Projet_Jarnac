def Decode(Nbr):
    t = bin(Nbr)[2:]
    Existe = t[-1]
    BorneInf = int(t[-19:-1], 2)
    BorneSup = int(t[-39:-21], 2)
    print(BorneSup)

def Convert(str):
    return (ord(str[0])-65)*26**2 +  (ord(str[1])-65)*26 + ord(str[2])-65

print(Convert("BITA"))
Decode(4760553023)