#include <iostream>
#include "geral.h"
#include "siege.h"
#include "tela.h"

using namespace std;

int main(int narg,char **argc)
{  int cor;
   Tela tela;
   tela.fim_cor();
   tela.fim_negrito();

   if(narg!=2 || sscanf(argc[1],"%d",&cor)!=1 || (cor!=1 && cor!=0))
      pf("./IA <cor_jogador>\ncor_jogador = 0 para vermelho e 1 para amarelo\n");
   else
   {  Siege s(cor);
      s.le_configuracoes();
      s.monta_grafo();
      s.conecta();
      s.jogo();
   }
   return 0;
}
