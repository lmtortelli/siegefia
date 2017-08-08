#!/usr/bin/env python
#-*- coding: utf-8 -*-

from state import State
from rules import *

import random



class Siege(object):

    __amarelos = ['g1','g2','g3','g4','g5','g6','g7','g8','f1','f2','f3','f4','f5','f6','f7','f8']
    __vermelhos = ['a1','a2','a3','a4','a5','a6','a7','a8','a9','a10','a11','a12','a13','a14','a15','a16']

    __capturas = []


    def __isDone(self):
        if('h1' in self.__vermelhos):
            return True
        elif(len(self.__vermelhos)==0):
            return True
        else:
            return False

    #Somente responsavel por gerar a movimentacao
    def __movGenerator(self,turn):
        #return self.__minMax(turn)
        return self.__randomMovement(turn)

        #Realiza a validacao da captura dado uma peca e seu vizinho (de cores contrarias) Assim se cosneguir aplica ruma captura ira retornar
        #Dentro da lista de captura todos os movimentos necessarios
    def __validateCapture(self,piece,viz):
        emptySpot = None
        for tupl in capturas[piece]:
            if(viz in tupl):
                if((tupl[2] not in self.__amarelos) and (tupl[2] not in self.__vermelhos)):
                    self.__capturas.append([piece,[tupl[2],tupl[1]]])
                break

    #VErifica todas as capturas possiveis dado time Amarelo ou Vermelho (Etapa necessaria para avaliar se existe massacres disponíveis)
    def __verifyAllCaptures(self,turn):
        self.__capturas = []
        if(turn=='a'):
            for piece in self.__amarelos:
                for viz in vizinhos[piece]:
                    if(viz in self.__vermelhos):
                        self.__validateCapture(piece,viz)
        elif(turn=='v'):
            for piece in self.__vermelhos:
                for viz in vizinhos[piece]:
                    if(viz in self.__amarelos):
                        self.__validateCapture(piece,viz)

    #Verifica vizinhos de uma determinada peca
    def __verifyNeighborhoodPiece(self,turn,piece):
        emptySpot = []

        if(turn=='a'):
            for viz in vizinhos[piece]:
                if((viz not in self.__amarelos) and (viz not in self.__vermelhos)):
                    emptySpot.append([piece,[viz,'']])
                elif(viz in self.__amarelos):
                    pass
        elif(turn=='v'):
            piece = self.__vermelhos[random.randint(0, len(self.__vermelhos)-1)]
            for viz in vizinhos[piece]:
                if((viz not in self.__amarelos) and (viz not in self.__vermelhos)):
                    emptySpot.append([piece,[viz,'']])
                elif(viz in self.__vermelhos):
                    pass
        return emptySpot



    def __randomMovement(self,turn):
        emptySpot = []
        rList = []
        findMovement = False
        while(not findMovement):
            self.__capturas = []
            emptySpot = []
            self.__verifyAllCaptures(turn)

            if(turn=='a'):
                piece = self.__amarelos[random.randint(0, len(self.__amarelos)-1)]
                emptySpot = self.__verifyNeighborhoodPiece(turn,piece)
            else:
                piece = self.__vermelhos[random.randint(0, len(self.__vermelhos)-1)]
                emptySpot = self.__verifyNeighborhoodPiece(turn,piece)

            if(len(self.__capturas)!=0):
                findMovement = True
                rList = self.__capturas.pop()
            elif((len(emptySpot))!=0):
                findMovement = True
                rList = emptySpot.pop()

        return rList

        #Verifica vizinhos dado uma determina peca no tabuleiro e seu time (Amarelo ou Vermelho())


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

    #Aplica a modificacao do movimento no tabuleiro
    def __applyMovement(self,movement):
        if(movement[0] in self.__amarelos):
            index = self.__amarelos.index(movement[0])
            self.__amarelos[index] = movement[1][0]
        elif(movement[0] in self.__vermelhos):
            index = self.__vermelhos.index(movement[0])
            self.__vermelhos[index] = movement[1][0]

    #Apos a primeira captura e necessario verificar se a nova posicao da peca gera uma nova captura alista de todas as captudas anteriormente disponiveis
    #Segue intacta
    def __nextCapture(self,turn,pos):
        if(turn=='a'):
            for viz in vizinhos[pos]:
                if(viz in self.__vermelhos):
                    self.__validateCapture(pos,viz)
        elif(turn=='v'):
            for viz in vizinhos[pos]:
                if(viz in self.__amarelos):
                    self.__validateCapture(pos,viz)

    def minMax(self,turn):

        depth = 1
        states = []
        newStates = []
        states.append(State(self.__amarelos,self.__vermelhos,None))
        for i in range(depth):
            for state in states:
                newStates = state.makeChilds(turn)

            if(turn=='a'):
                turn = 'v'
            else:
                turn = 'a'
            print len(newStates)
            states = []
            for state in newStates:
                states = state.makeChilds(turn)
        print len(states)
        state = states[0]

        print state.ref
        print state.ref.ref
        print state.ref.ref.ref

        #movimentEscolhido = states.sort(min)
        #return movimentEscolhido



    #Metodo principal do jogo
    def gameSiege(self):
        turno = 'v'

        while(not self.__isDone()):
            if(turno=='a'):
                self.__capturas = []
                mov = self.__movGenerator(turno) #minMax / Aleatorio e talz
                print "##################"
                print "AMARELO"
                print mov
                self.__applyMovement(mov)
                if(self.__checkCapture(turno,mov)==True):
                    self.__verifyAllCaptures(turno)

                    print "##################"
                    print "MASSACRE"

                    while len(self.__capturas)>0:
                        mov = self.__capturas.pop()
                        self.__applyMovement(mov)
                        self.__checkCapture(turno,mov)
                        self.__verifyAllCaptures(turno)
                turno = 'v'

            elif(turno=='v'):
                self.__capturas = []
                mov = self.__movGenerator(turno) #minMax / Aleatorio e talz

                print "##################"
                print "VERMELHO"
                print mov

                self.__applyMovement(mov)
                if(self.__checkCapture(turno,mov)==True):
                    self.__verifyAllCaptures(turno)
                    print "##################"
                    print "MASSACRE"
                    while len(self.__capturas)>0:
                        mov = self.__capturas.pop()
                        self.__applyMovement(mov)
                        self.__checkCapture(turno,mov)
                        self.__verifyAllCaptures(turno)
                turno = 'a'

        print self.__amarelos
        print self.__vermelhos





s = Siege()
#s.gameSiege()

s.minMax('a')
