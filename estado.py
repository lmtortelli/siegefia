#!/usr/bin/env python
#-*- coding: utf-8 -*-

from rules import *
from siege import *

class State(object):
    __hAmarelo = 0
    __hVermelho = 0
    __ref = State()
    __h = 0
    __stateBoard = []
    __mov = []


    def __init(self,amarelos,vermelhos,ref):
        self.__ref = ref
        self.__stateBoard = [amarelos,vermelhos]

    def calculateH(self):


    def makeChilds(self,turn):
        #Gera o movimento para todos as posicoes vizinhas possiveis dependendo do turno
        childrens = []
        board = 0 #Se turn a
        newPositions = []
        if(turn == 'v'):
            board = 1

        for piece in self.__stateBoard[board]:
            newPositions.append(self.__verifyNeighborhoodPiece(turn,piece))
        newPositions.append(self.__verifyAllCaptures(turn))
        for mov in newPositions:
            newState = State(self.__stateBoard[0],self.__stateBoard[1],self)
            newState.applyMovement(mov,turn)
            newState.calculateH()
            childrens.append(newState)

        return childrens



    def applyMovement(self,movement,turn):
        self.__mov = movement
        
        if(movement[1][1]==''):
            if(turn=='a'):
                index = self.__stateBoard[1].index(movement[1][1])
                self.__stateBoard[1].remove(movement[1][1])
            elif(turn=='v'):
                index = self.__stateBoard[0].index(movement[1][1])
                self.__stateBoard[0].remove(movement[1][1])
        else:
            if(movement[0] in self.__amarelos):
                index = self.__amarelos.index(movement[0])
                self.__amarelos[index] = movement[1][0]
            elif(movement[0] in self.__vermelhos):
                index = self.__vermelhos.index(movement[0])
                self.__vermelhos[index] = movement[1][0]

    def __verifyAllCaptures(self,turn):
        captures = []
        if(turn=='a'):
            for piece in self.__amarelos:
                for viz in vizinhos[piece]:
                    if(viz in self.__vermelhos):
                        captures = self.__validateCapture(piece,viz)
        elif(turn=='v'):
            for piece in self.__vermelhos:
                for viz in vizinhos[piece]:
                    if(viz in self.__amarelos):
                        captures = self.__validateCapture(piece,viz)
        return captures

    def __validateCapture(self,piece,viz):
        captures = None
        for tupl in capturas[piece]:
            if(viz in tupl):
                if((tupl[2] not in self.__amarelos) and (tupl[2] not in self.__vermelhos)):
                    captures.append([piece,[tupl[2],tupl[1]]])
                break
        return captures



    def __fhAmarelo(self):
    def __fhVermelho(self):
