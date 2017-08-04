from rules import *
import random



class Siege(object):

    __amarelos = ['g1','g2','g3','g4','g5','g6','g7','g8','f1','f2','f3','f4','f5','f6','f7','f8']
    __vermelhos = ['a1','a2','a3','a4','a5','a6','a7','a8','a9','a10','a11','a12','a13','a14','a15','a16']

    __capturas = []

    __players = 0

    def __init__(self,players):
        self.__players = players

    def __isDone(self):
        if('h1' in self.__vermelhos):
            return True
        elif(len(self.__vermelhos)==0):
            return True
        else:
            return False

    def __checkMovement(self,turn,source,dest):
        if(turn=='a'):#Turno Amarelo
            print "OK"
            #vefica se e movimento normal (vizinhos)
            #vefica se e movimento de captura



    def __orderGenerator(self):
        if(self.__players==1):
            return random.shuffle(['p','c'])
        else:
            return ['c','c']

    def __movGenerator(self,turn):
        #return self.__minMax(turn)
        return self.__randomMovement(turn)

    def __randomMovement(self,turn):
        emptySpot = []
        rList = []
        findMovement = False
        while(not findMovement):
            self.__capturas = []
            emptySpot = []

            if(turn=='a'):
                piece = self.__amarelos[random.randint(0, len(self.__amarelos)-1)]
                for viz in vizinhos[piece]:
                    if((viz not in self.__amarelos) and (viz not in self.__vermelhos)):
                        emptySpot.append(viz)
                    elif(viz in self.__amarelos):
                        pass
                    elif(viz in self.__vermelhos):
                        self.__validateCapture(piece,viz)
            elif(turn=='v'):
                piece = self.__vermelhos[random.randint(0, len(self.__vermelhos)-1)]
                for viz in vizinhos[piece]:
                    if((viz not in self.__amarelos) and (viz not in self.__vermelhos)):
                        emptySpot.append(viz)
                    elif(viz in self.__vermelhos):
                        pass
                    elif(viz in self.__amarelos):
                        self.__validateCapture(piece,viz)

            if(len(self.__capturas)!=0):
                findMovement = True
                rList = [piece,self.__capturas.pop()]
            elif(len(emptySpot)!=0):
                findMovement = True
                rList = [piece,[emptySpot.pop(),'']]

        return rList

    def __validateCapture(self,piece,viz):
        emptySpot = None
        for tupl in capturas[piece]:
            if(viz in tupl):
                if((tupl[2] not in self.__amarelos) and (tupl[2] not in self.__vermelhos)):
                    self.__capturas.append([tupl[2],tupl[1]])
                break


    # Resposnsavel por verificar se o movimento foi uma captura e tambem deleta a peca capturada
    def __checkCapture(self,turn,movement):
        boolReturn = False
        # Verificando se foi uma captura para deixar continuar outras capturas
        if(movement[1][0] in vizinhos[movement[0]]):
            boolReturn = False
        else:
            boolReturn = True
            print self.__amarelos
            print self.__vermelhos

            if(turn=='a'):
                index = self.__vermelhos.index(movement[1][1])
                self.__vermelhos.remove(movement[1][1])
            elif(turn=='v'):
                index = self.__amarelos.index(movement[1][1])
                self.__amarelos.remove(movement[1][1])

        return boolReturn

    def __applyMovement(self,movement):
        if(movement[0] in self.__amarelos):
            index = self.__amarelos.index(movement[0])
            self.__amarelos[index] = movement[1][0]
        elif(movement[0] in self.__vermelhos):
            index = self.__vermelhos.index(movement[0])
            self.__vermelhos[index] = movement[1][0]



    def gameSiege(self):
        turno = 'v'
        player = self.__orderGenerator()

        while(not self.__isDone()):

            if(turno=='a'):
                self.__capturas = []
                mov = self.__movGenerator(turno) #minMax / Aleatorio e talz
                print mov
                self.__applyMovement(mov)
                if(self.__checkCapture(turno,mov)==True):
                    print "CAPTUROU" #Montar sistema pos primeira captura

                turno = 'v'

            else:
                if(turno=='v'):
                    self.__capturas = []
                    mov = self.__movGenerator(turno) #minMax / Aleatorio e talz
                    print mov
                    self.__applyMovement(mov)
                    if(self.__checkCapture(turno,mov)==True):
                        print "CAPTUROU" #Montar sistema pos primeira captura

                    turno = 'a'

        print self.__amarelos
        print self.__vermelhos





s = Siege(0)
s.gameSiege()
