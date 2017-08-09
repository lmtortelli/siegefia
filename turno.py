from enum import Enum
class Turno(Enum):
    AMARELO = 0
    VERMELHO = 1
    MIN = 2
    MAX = 3

    def __invert__(self):
        if(self == Turno.AMARELO):
            return Turno.VERMELHO
        else:
            return Turno.AMARELO
