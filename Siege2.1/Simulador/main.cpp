#include <stdio.h>
#include "siege.h"

int main(int argc, char *argv[])
{  Siege s;
   s.le_configuracoes();
   s.inicia_comunicacao_vrep();
   s.monta_grafo();
   //s.jogo();
   s.jogo_rede();
   return 0;
}
