from rules import *
import Random


class Siege(object):

    __amarelos = ['g1','g2','g3','g4','g5','g6','g7','g8','f1','f2','f3','f4','f5','f6','f7','f8']
    __vermelhos = ['a1','a2','a3','a4','a5','a6','a7','a8','a9','a10','a11','a12','a13','a14','a15','a16']
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

    def __randomMovement(self,turn):
        return True

    def gameSiege(self):
        turno = 'a'
        player = self.__orderGenerator()

        while(not self.__isDone()):
            if(turno=='a'):
                turno = 'v'
                while True:
                    if(player[0]=='c'): #computador
                        mov = self.__randomMovement(turno)
                        #mov = MinMax.nextState()
                    else: #Jogador
                        mov = raw_input()
                        mov = mov.split(' ')

                    #Analisa movimento (Retorna True se for uma captura)
                    if(self.__checkMovement('a',mov[0],mov[1])):
                        break
                #Update informacoes do jogo, quantidade de turnos jogados e etc


            else(turno=='v'):
                turno = 'a'







s = Siege(0)
s.gameSiege()
