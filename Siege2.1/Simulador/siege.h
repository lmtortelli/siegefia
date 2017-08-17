#ifndef SIEGE_H
#define SIEGE_H

#include <vector>
#include <string>
#include <map>
#include <math.h>
#include <fstream>
#include <iostream>
#include "tela.h"
#include "geral.h"
#include "comunicaTCP.h"
#include "comunicaUDP.h"
extern "C" {
  #include "remoteApi/extApi.h"
}

#define VAZIO 2
#define PECA_VERMELHA 0
#define PECA_AMARELA 1
#define SEM_VENCEDOR -1

using namespace std;

class NoPosicao;
class Captura;
class Peca;

/*--------------------------------------------*/
class Siege
{  public:
      Siege();
      ~Siege();

      string id(char letra, unsigned short ind);
      void monta_grafo();
      void jogo();
      void jogo_rede();
      bool posicao_valida(char &letra, int nro);
      short int verifica_vencedor();

      void inicia_comunicacao_vrep();

      void le_configuracoes();

      map <string, NoPosicao *> grafo;
      vector <Peca *> vermelhas;
      vector <Peca *> amarelas;

      ComunicaUDP *comunica[2];
      int clientID;

      unsigned int PORTA_VREP;
      unsigned PORTA_VERMELHO;
      unsigned PORTA_AMARELO;

      string IP_VREP;

      bool PRIMEIRO_JOGADOR;
      Tela tela;
};

/*--------------------------------------------*/
class NoPosicao
{  public:
      NoPosicao();
      void setPos(float x, float y);
      string id;
      vector <NoPosicao *> adj;
      vector <Captura> captura;
      float x;          
      float y;
      unsigned short int tipo;
      Peca *peca;
};

/*--------------------------------------------*/
class Captura
{  public:
      Captura(NoPosicao *no_captura,NoPosicao *prox_no);
      NoPosicao *prox_no;
      NoPosicao *no_captura;
};

/*--------------------------------------------*/
class Peca
{  public:
      Peca(NoPosicao *pos);
      int handle;
      NoPosicao *pos;
};

/*--------------------------------------------*/
#endif // SIEGE_H


