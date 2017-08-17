#!/usr/bin/env python
#-*- coding: utf-8 -*-

from state import State
from client import Client
from turno import Turno
from rules import *
import sys

import random



class Siege(object):

    #__amarelos = ['g1','g2','g3','g4','g5','g6','g7','g8','f1','f2','f3','f4','f5','f6','f7','f8']
    #__vermelhos = ['a1','a2','a3','a4','a5','a6','a7','a8','a9','a10','a11','a12','a13','a14','a15','a16']
    __vermelhos = ['d1', 'd3', 'd5', 'd7', 'd9', 'd11', 'd13', 'd15']
    __amarelos = ['g2', 'g1', 'g3', 'g4', 'g5', 'g6', 'g7', 'g8']
    #__amarelos = ['f1']
    __board = [__amarelos,__vermelhos]
    __capturas = []
    __turno = None
    __ipServer = ''

    def __init__(self,turno,ip):
        self.__ipServer = ip
        if(turno=='0'):
            self.__turno = Turno.AMARELO
        elif(turno=='1'):
            self.__turno = Turno.VERMELHO

    def __isDone(self):
        if('h1' in self.__vermelhos):
            return True
        elif(len(self.__vermelhos)==0):
            return True
        else:
            return False


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
        for piece in self.__board[turn.value]:
            for viz in vizinhos[piece]:
                if(viz in self.__board[(~turn).value]):
                    self.__validateCapture(piece,viz)

    # Resposnsavel por verificar se o movimento foi uma captura e tambem deleta a peca capturada
    def checkCapture(self,turn,movement):
        boolReturn = False

        # Verificando se foi uma captura para deixar continuar outras capturas
        boolReturn = not movement[1][0] in vizinhos[movement[0]]
        if(boolReturn):
            self.__board[(~turn).value].remove(movement[1][1])

        #print self.__amarelos
        #print self.__vermelhos

        return boolReturn

    #Aplica a modificacao do movimento no tabuleiro
    def applyMovement(self,movement,turn):
        index = self.__board[turn.value].index(movement[0])
        self.__board[turn.value][index] = movement[1][0]

    #Apos a primeira captura e necessario verificar se a nova posicao da peca gera uma nova captura alista de todas as captudas anteriormente disponiveis
    #Segue intacta
    def __nextCapture(self,turn,pos):
        for viz in vizinhos[pos]:
            if(viz in self.__board[(~turn).value]):
                self.__validateCapture(pos,viz)

    def __receiveMensage(self,client,turno):
        mov = None
        while(mov!='fim' and not self.__isDone() ):
            mov = client.recebeMensagem()
            if(mov!=None and mov!='fim'):
                self.applyMovement(mov,~turno)
                self.checkCapture(~turno,mov)

    def minMax(self,turn):

        plays = 3
        states = []
        newStates = []
        initialState = State(self.__amarelos[:],self.__vermelhos[:],None)
        initialState.calculateH()
        initialState.turnoRef = turn
        states.append(initialState)
        for i in range(plays):
            if(i%2==0):
                for state in states:
                    newStates+= state.makeChilds(turn,Turno.MAX)
                newStates.sort()
                choiceState = newStates[len(newStates)-1]
                states=[]
            else:
                for state in newStates:
                    states+= state.makeChilds(~turn,Turno.MIN)
                newStates = []
                states.sort()
                choiceState = states[0]

        for i in range(plays-1):
            choiceState = choiceState.ref

        return choiceState.mov


    #Metodo principal do jogo
    def gameSiege(self):
        turno = self.__turno
        client = Client(turno,self.__ipServer)
        while(not self.__isDone()):

            if(turno==Turno.AMARELO):
                self.__receiveMensage(client,turno)

            self.__capturas = []
            mov = self.minMax(turno) #minMax / Aleatorio e talz
            client.enviaMensagem(client.montaMensagem(mov))

            print "##################"
            print turno
            print mov
            self.applyMovement(mov,turno)
            captureBool = self.checkCapture(turno,mov)
            if(captureBool):
                self.__verifyAllCaptures(turno)

                print "##################"
                print "MASSACRE"

                while len(self.__capturas)>0:
                    mov = self.__capturas.pop()
                    print mov
                    client.enviaMensagem(client.montaMensagem(mov))
                    self.applyMovement(mov,turno)
                    self.checkCapture(turno,mov)
                    self.__verifyAllCaptures(turno)
            client.enviaMensagem('fim')


            if(turno==Turno.VERMELHO):
                self.__receiveMensage(client,turno)

            #Descomentar para jogar internamente
            #turno = ~turno

        print self.__amarelos
        print self.__vermelhos






# get argument list using sys module

args = str(sys.argv)
s = Siege(str(sys.argv[1]),str(sys.argv[2]))

s.gameSiege()

#s.minMax('a')
