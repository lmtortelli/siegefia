#!/usr/bin/env python
#-*- coding: utf-8 -*-

from rules import *

class State:
    __hAmarelo = 0
    __hVermelho = 0
    ref = None
    __h = 0
    mov = None
    __stateBoard = []



    def __init__(self,amarelos,vermelhos,ref):
        self.ref = ref
        self.__stateBoard = [amarelos,vermelhos]

    def calculateH(self):
        return 0

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



    def __fhAmarelo(self):
        return 0
    def __fhVermelho(self):
        return 0
