#include"tela.h"

/*----------------------------------------------------------*/
// Limpa a tela
void Tela::limpa_tela()
{  system("clear");
}

/*----------------------------------------------------------*/
// Deixa os proximos textos imprimidos em negrito
void Tela::negrito()
{  printf("\e[1m");
}

/*----------------------------------------------------------*/
// Volta a imprimir os textos sem negrito
void Tela::fim_negrito()
{  printf("\e[21m");
}

/*----------------------------------------------------------*/
// Deixa os proximos textos imprimidos coloridos
void Tela::cor(unsigned cor)
{  printf("\e[38;5;%um",cor);
}

/*----------------------------------------------------------*/
// Volta a imprimir os textos na cor padrao
void Tela::fim_cor()
{  printf("\e[39m");
}

/*----------------------------------------------------------*/
// Imprime tabela de cores
void Tela::imprime_tabela_cores()
{  unsigned c;
   for(c=0;c<256;c++)
   {  cor(c);  printf("%d - Teste\n",c);  fim_cor();
   }
}

/*----------------------------------------------------------*/
// Imprime str com a cor c
void Tela::imprime_colorido(unsigned c,const char *str)
{  negrito(); cor(c); printf("%s",str); fim_cor(); fim_negrito();
}

