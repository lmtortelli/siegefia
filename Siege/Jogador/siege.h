#ifndef SIEGE_H
#define SIEGE_H

#include <vector>
#include <string>
#include <map>
#include <fstream>
#include "tela.h"
#include "geral.h"
#include "comunicaTCP.h"
#include "comunicaUDP.h"

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
      Siege(bool cor_jogador);
      ~Siege();
      string id(char letra, unsigned short ind);
      void monta_grafo();
      void conecta();
      bool posicao_valida(char &letra, int nro);
      void executa_jogada();
      void jogo();
      void le_configuracoes();
      short int verifica_vencedor();

      map <string, NoPosicao *> grafo;
      vector <Peca *> vermelhas;
      vector <Peca *> amarelas;
      ComunicaUDP *comunica;
      Tela tela;
      bool vez;
      char letra, letrades, letracap;
      int nro, nrodes, nrocap;
      NoPosicao *origem, *destino, *pos_captura;
      bool flag_captura;
      string IP_SIMULADOR;
      unsigned int PORTA_SIMULADOR;
      bool PRIMEIRO_JOGADOR;
      bool cor_jogador;
};

/*--------------------------------------------*/
class NoPosicao
{  public:
      NoPosicao();
      string id;
      Peca *peca;
      vector <NoPosicao *> adj;
      vector <Captura> captura;
      unsigned short int tipo;
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


#endif // SIEGE_H

