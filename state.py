#!/usr/bin/env python
#-*- coding: utf-8 -*-

from turno import Turno
from rules import *

class State:
    ref = None
    mov = None
    h = [0,0]
    __stateBoard = []



    def __init__(self,amarelos,vermelhos,ref):
        self.ref = ref
        self.__stateBoard = [amarelos,vermelhos]
        if(ref!=None):
            self.h = ref.h[:]

    def calculateH(self):
        self.__fhAmarelo()
        self.__fhVermelho()

    def hAmarelo(self):
        return self.h[0]
    def hVermelho(self):
        return self.h[1]

    def getState(self):
        return self.__stateBoard
    def __analiseAlfaBetaPruding(self,state,minMax,turn):
        if(minMax is Turno.MIN):
            if(state.h[turn.value] < self.h[turn.value]):
                return False
        else:
            if(state.h[turn.value] > self.h[turn.value]):
                return False
        return True


    def makeChilds(self,turn,minMax ):
        #Gera o movimento para todos as posicoes vizinhas possiveis dependendo do turno
        childrens = []
        newPositions = []

        for piece in self.__stateBoard[turn.value]:
            newPositions+=self.__verifyNeighborhoodPiece(turn,piece)
        newPositions+=self.__verifyAllCaptures(turn)


        for mov in newPositions:
            newState = State(self.__stateBoard[0][:],self.__stateBoard[1][:],self)
            newState.applyMovement(mov[:],turn)
            newState.calculateH()
            childrens.append(newState)
            #print newState.hAmarelo()
            #print newState.hVermelho()
            newState = None


        return childrens

    def __verifyNeighborhoodPiece(self,turn,piece):
        emptySpot = []

        for viz in vizinhos[piece]:
            if((viz not in self.__stateBoard[Turno.AMARELO.value]) and (viz not in self.__stateBoard[Turno.VERMELHO.value])):
                emptySpot.append([piece,[viz,'']])
            elif(viz in self.__stateBoard[turn.value]):
                pass
        '''
        if(turn==Turno.AMARELO):
            for viz in vizinhos[piece]:
                if((viz not in self.__stateBoard[0]) and (viz not in self.__stateBoard[1])):
                    emptySpot.append([piece,[viz,'']])
                elif(viz in self.__stateBoard[0]):
                    pass
        elif(turn==Turno.VERMELHO):
            for viz in vizinhos[piece]:
                if((viz not in self.__stateBoard[0]) and (viz not in self.__stateBoard[1])):
                    emptySpot.append([piece,[viz,'']])
                elif(viz in self.__stateBoard[1]):
                    pass
        '''

        return emptySpot

    def applyMovement(self,movement,turn):
        self.mov = list(movement)

        if(movement[1][1]!=''):
            self.__stateBoard[(~turn).value].remove(movement[1][1])
        else:
            index = self.__stateBoard[turn.value].index(movement[0])
            self.__stateBoard[turn.value][index] = movement[1][0]


    def __verifyAllCaptures(self,turn):
        captures = []

        for piece in self.__stateBoard[turn.value]:
            for viz in vizinhos[piece]:
                if(viz in self.__stateBoard[(~turn).value]):
                    captures+=(self.__validateCapture(piece,viz))
        '''
        if(turn==Turno.AMARELO):
            for piece in self.__stateBoard[0]:
                for viz in vizinhos[piece]:
                    if(viz in self.__stateBoard[1]):
                        captures+=(self.__validateCapture(piece,viz))

        elif(turn==Turno.VERMELHO):
            for piece in self.__stateBoard[1]:
                for viz in vizinhos[piece]:
                    if(viz in self.__stateBoard[0]):
                        captures+=(self.__validateCapture(piece,viz))
        '''
        return captures

    def __validateCapture(self,piece,viz):
        captures = []
        for tupl in capturas[piece]:
            if(viz in tupl):
                if((tupl[2] not in self.__stateBoard[Turno.AMARELO.value]) and (tupl[2] not in self.__stateBoard[Turno.VERMELHO.value])):
                    captures.append([piece,[tupl[2],tupl[1]]])
                break
        return captures

    def __calculateDistanceToH1(self):
        distances = []
        for piece in self.__stateBoard[1]:
            estados = vizinhos[piece]
            novosEstados = []
            findH1 = False
            if(piece is 'h1'):
                findH1 = True
            distance = 0
            while(not findH1):
                distance+=1
                for viz in estados:
                    if(viz is 'h1'):
                        findH1 = True
                        break
                    novosEstados+=vizinhos[viz]
                estados = []
                estados = novosEstados
                novosEstados = []
            distances.append(distance)

        return distances[0]
    def __fhAmarelo(self):
        hAmarelo= 0
        containsH1 = False
        qtdPecas  = (len(self.__stateBoard[0]) - len(self.__stateBoard[1]))*10
        for piece in self.__stateBoard[0]:
            if(piece == 'h1'):
                containsH1 = True
                break
        qtdCapturas = len(self.__verifyAllCaptures(Turno.AMARELO)) * 15

        if(containsH1):
            hAmarelo+=20

        self.h[Turno.AMARELO.value]+= qtdPecas + qtdCapturas + hAmarelo



    def __fhVermelho(self):
        hVermelho=0
        qtdPecas  = (len(self.__stateBoard[1]) - len(self.__stateBoard[0]))*10
        distanceToH1 = self.__calculateDistanceToH1()
        qtdCapturas = len(self.__verifyAllCaptures(Turno.VERMELHO)) * 15

        self.h[Turno.VERMELHO.value]+=qtdPecas+qtdCapturas + hVermelho - (distanceToH1*10)

#t = Turno.AMARELO
#print t.value
#t = Turno.VERMELHO
#print t.value
#print t is Turno.VERMELHO
