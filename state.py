#!/usr/bin/env python
#-*- coding: utf-8 -*-

from turno import Turno
from rules import *

class State:
    __hAmarelo = 0
    __hVermelho = 0
    ref = None
    mov = None
    __stateBoard = []



    def __init__(self,amarelos,vermelhos,ref):
        self.ref = ref
        self.__stateBoard = [amarelos,vermelhos]

    def calculateH(self):
        self.__fhAmarelo()
        self.__fhVermelho()

    def hAmarelo(self):
        return self.__hAmarelo
    def hVermelho(self):
        return self.__hVermelho

    def getState(self):
        return self.__stateBoard

    def makeChilds(self,turn):
        #Gera o movimento para todos as posicoes vizinhas possiveis dependendo do turno
        childrens = []
        board = 0 #Se turn a
        newPositions = []
        if(turn == 'v'):
            board = 1

        for piece in self.__stateBoard[board]:
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
        if(turn=='a'):
            for viz in vizinhos[piece]:
                if((viz not in self.__stateBoard[0]) and (viz not in self.__stateBoard[1])):
                    emptySpot.append([piece,[viz,'']])
                elif(viz in self.__stateBoard[0]):
                    pass
        elif(turn=='v'):
            for viz in vizinhos[piece]:
                if((viz not in self.__stateBoard[0]) and (viz not in self.__stateBoard[1])):
                    emptySpot.append([piece,[viz,'']])
                elif(viz in self.__stateBoard[1]):
                    pass

        return emptySpot

    def applyMovement(self,movement,turn):
        self.mov = list(movement)
        if(movement[1][1]!=''):
            if(turn=='a'):
                index = self.__stateBoard[1].index(movement[1][1])
                self.__stateBoard[1].remove(movement[1][1])
            elif(turn=='v'):
                index = self.__stateBoard[0].index(movement[1][1])
                self.__stateBoard[0].remove(movement[1][1])
        else:
            if(movement[0] in self.__stateBoard[0]):
                index = self.__stateBoard[0].index(movement[0])
                self.__stateBoard[0][index] = movement[1][0]
            elif(movement[0] in self.__stateBoard[1]):
                index = self.__stateBoard[1].index(movement[0])
                self.__stateBoard[1][index] = movement[1][0]

    def __verifyAllCaptures(self,turn):
        captures = []
        if(turn=='a'):
            for piece in self.__stateBoard[0]:
                for viz in vizinhos[piece]:
                    if(viz in self.__stateBoard[1]):
                        captures+=(self.__validateCapture(piece,viz))
        elif(turn=='v'):
            for piece in self.__stateBoard[1]:
                for viz in vizinhos[piece]:
                    if(viz in self.__stateBoard[0]):
                        captures+=(self.__validateCapture(piece,viz))

        return captures

    def __validateCapture(self,piece,viz):
        captures = []
        for tupl in capturas[piece]:
            if(viz in tupl):
                if((tupl[2] not in self.__stateBoard[0]) and (tupl[2] not in self.__stateBoard[1])):
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
        self.__hAmarelo+=3
        containsH1 = False
        qtdPecas  = len(self.__stateBoard[0]) - len(self.__stateBoard[1])
        for piece in self.__stateBoard[0]:
            if(piece == 'h1'):
                containsH1 = True
                break
        qtdCapturas = len(self.__verifyAllCaptures('a')) * 7

        if(containsH1):
            self.__hAmarelo+=10
        self.__hAmarelo+= qtdPecas + qtdCapturas


    def __fhVermelho(self):
        self.__hVermelho+=3
        qtdPecas  = len(self.__stateBoard[1]) - len(self.__stateBoard[0])
        distanceToH1 = self.__calculateDistanceToH1()
        qtdCapturas = len(self.__verifyAllCaptures('v')) * 7

        self.__hVermelho+=qtdPecas+qtdCapturas-distanceToH1

#t = Turno.AMARELO
#print t.value
#t = Turno.VERMELHO
#print t.value
#print t is Turno.VERMELHO
