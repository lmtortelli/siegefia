/* Biblioteca para impressoes coloridas no terminal do linux
   Mais detalhes sobre formatacao no linux:
   http://misc.flogisoft.com/bash/tip_colors_and_formatting
*/
#ifndef TELA_H
#define TELA_H

#include <stdlib.h>
#include <stdio.h>

#define BRANCO 231
#define AZUL 39
#define ROSA 207
#define VERMELHO 9//196
#define VERDE 118
#define AMARELO 11 //11

class Tela
{  public:
      void limpa_tela();
      void negrito();
      void fim_negrito();
      void cor(unsigned cor);
      void fim_cor();
      void imprime_tabela_cores();
      void imprime_colorido(unsigned c,const char *str);
};

#endif // TELA_H


