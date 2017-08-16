#include "siege.h"

Siege::Siege(bool cor_jogador)
{  this->cor_jogador = cor_jogador;
   if(cor_jogador==PECA_VERMELHA)
      tela.imprime_colorido(VERMELHO, "\nVocê é o jogador vermelho!\n");
   else
      tela.imprime_colorido(AMARELO, "\nVocê é o jogador amarelo!\n");
   comunica = NULL;
}

/*-------------------------------------------------------------------*/
Siege::~Siege()
{  unsigned i;
   map <string, NoPosicao *>::iterator it;
   for(it=grafo.begin(); it!=grafo.end(); it++)
      delete it->second;
   for(i=0;i<vermelhas.size();i++)
      delete vermelhas[i];
   for(i=0;i<amarelas.size();i++)
      delete amarelas[i];
   if(comunica!=NULL)
      delete comunica;
}

/*-------------------------------------------------------------------*/
void Siege::le_configuracoes()
{  ifstream arq;
   char s[100];
   string str;
   arq.open("../conf");

   pf("\nCarregando configurações ...\n\n");
   arq >> str >> IP_SIMULADOR;
   getline(arq,str);
   getline(arq,str);
   if(cor_jogador==PECA_VERMELHA)
   {  sscanf(str.c_str(),"%s %u ",s,&PORTA_SIMULADOR);
      getline(arq,str);
   }
   else
   {  getline(arq,str);
      sscanf(str.c_str(),"%s %u ",s,&PORTA_SIMULADOR);
   }
   arq >> str>> PRIMEIRO_JOGADOR;

   arq.close();

   pf(" => Simulador\n");
   pf("Ip: %s\n",IP_SIMULADOR.c_str());
   pf("Porta: %u (%s)\n\n",PORTA_SIMULADOR,(cor_jogador)?"Amarelo":"Vermelho");
   pf(" => Primeiro Jogador\n");
   pf("%s\n\n",(PRIMEIRO_JOGADOR)?"Amarelo":"Vermelho");

}

/*-------------------------------------------------------------------*/
string Siege::id(char letra, unsigned short ind)
{  char s[30];
   sprintf(s,"%c%d",letra,ind);
   return string(s);
}

/*-------------------------------------------------------------------*/
void Siege::monta_grafo()
{  char i;
   int j;
   string iden;
   NoPosicao *no;
   Peca *peca;

   //--- Nos ----------------------
   for(i='a'; i<='h'; i++)
   {  if(i<'e')
      {  for(j=1;j<=16;j++)
         {  iden = id(i,j);
            grafo[iden] = new NoPosicao();
            grafo[iden]->id = iden;
         }
      }
      else
      {  if(i<'h')
         {  for(j=1;j<=8;j++)
            {  iden = id(i,j);
               grafo[iden] = new NoPosicao();
               grafo[iden]->id = iden;
            }
         }
         else
         {  iden = id(i,1);
            grafo[iden] = new NoPosicao();
            grafo[iden]->id = iden;
         }
      }
   }

   //--- Adjacencias --------------
   //--- h -------------
   no = grafo["h1"];
   for(j=1;j<=8;j++)
   {  no->adj.push_back(grafo[id('g',j)]);
      no->captura.push_back(Captura(grafo[id('g',j)], grafo[id('f',j)]));
   }

   //--- g -------------
   for(j=1;j<=8;j++)
   {  NoPosicao *gprox, *gant, *f, *e, *gantant, *gproxprox;
      no = grafo[id('g',j)];

      gprox = grafo[id('g',j%8+1)];
      gant  = grafo[id('g',(j+6)%8+1)];
      gproxprox = grafo[id('g',(j+1)%8+1)];
      gantant  = grafo[id('g',(j+5)%8+1)];
      f = grafo[id('f',j)];
      e = grafo[id('e',j)];

      no->adj.push_back(gprox);
      no->adj.push_back(gant);

      no->adj.push_back(grafo["h1"]);
      no->adj.push_back(f);

      no->captura.push_back(Captura(f, e));
      no->captura.push_back(Captura(gant, gantant));
      no->captura.push_back(Captura(gprox, gproxprox));
   }

   //--- f -------------
   for(j=1;j<=8;j++)
   {  NoPosicao *e, *g, *d, *fprox, *fant;
      no = grafo[id('f',j)];
      e = grafo[id('e',j)];
      g = grafo[id('g',j)];
      d = grafo[id('d',j)];
      no->adj.push_back(e);
      no->adj.push_back(g);
      if(j%2) //impar
      {  fant = grafo[id('f',(j+6)%8+1)];
         no->adj.push_back(fant);
      }
      else
      {  fprox = grafo[id('f',j%8+1)];
         no->adj.push_back(fprox);
      }

      no->captura.push_back(Captura(g,grafo["h1"]));
      no->captura.push_back(Captura(e,d));
   }

   //--- e -------------
   for(j=1;j<=8;j++)
   {  NoPosicao *d, *f, *c, *g, *eprox, *eant;
      no = grafo[id('e',j)];
      g = grafo[id('g',j)];
      f = grafo[id('f',j)];
      d = grafo[id('d',j)];
      c = grafo[id('c',j)];

      no->adj.push_back(f);
      no->adj.push_back(d);
      if((j%2)==0) //par
      {  eant = grafo[id('e',(j+6)%8+1)];
         no->adj.push_back(eant);
      }
      else
      {  eprox = grafo[id('e',j%8+1)];
         no->adj.push_back(eprox);
      }

      no->captura.push_back(Captura(f,g));
      no->captura.push_back(Captura(d,c));
   }

   //--- d -------------
   for(j=1;j<=16;j++)
   {  NoPosicao *e, *f, *c, *b, *dprox, *dant, *dproxprox, *dantant;
      no = grafo[id('d',j)];
      e = grafo[id('e',j)];
      f = grafo[id('f',j)];
      c = grafo[id('c',j)];
      b = grafo[id('b',j)];
      dprox = grafo[id('d',j%8+1)];
      dant  = grafo[id('d',(j+6)%8+1)];
      dproxprox = grafo[id('d',(j+1)%8+1)];
      dantant  = grafo[id('d',(j+5)%8+1)];

      no->adj.push_back(e);
      no->adj.push_back(c);
      no->adj.push_back(dant);
      no->adj.push_back(dprox);

      no->captura.push_back(Captura(e,f));
      no->captura.push_back(Captura(c,b));
      no->captura.push_back(Captura(dant,dantant));
      no->captura.push_back(Captura(dprox,dproxprox));
   }

   //--- c -------------
   for(j=1;j<=16;j++)
   {  NoPosicao *d, *e, *b, *a, *cprox, *cant;
      no = grafo[id('c',j)];
      d = grafo[id('d',j)];
      e = grafo[id('e',j)];
      b = grafo[id('b',j)];
      a = grafo[id('a',j)];

      no->adj.push_back(b);
      no->adj.push_back(d);
      if(j%2) //impar
      {  cant = grafo[id('c',(j+6)%8+1)];
         no->adj.push_back(cant);
      }
      else
      {  cprox = grafo[id('c',j%8+1)];
         no->adj.push_back(cprox);
      }

      no->captura.push_back(Captura(d,e));
      no->captura.push_back(Captura(b,a));
   }

   //--- b -------------
   for(j=1;j<=16;j++)
   {  NoPosicao *a, *c, *d, *bprox, *bant;
      no = grafo[id('b',j)];
      a = grafo[id('a',j)];
      c = grafo[id('c',j)];
      d = grafo[id('d',j)];

      no->adj.push_back(c);
      no->adj.push_back(a);
      if((j%2)==0) //par
      {  bant = grafo[id('b',(j+6)%8+1)];
         no->adj.push_back(bant);
      }
      else
      {  bprox = grafo[id('b',j%8+1)];
         no->adj.push_back(bprox);
      }

      no->captura.push_back(Captura(c,d));
   }

   //--- a -------------
   for(j=1;j<=16;j++)
   {  NoPosicao *b, *c, *aprox, *aant, *aproxprox, *aantant;
      no = grafo[id('a',j)];
      b = grafo[id('b',j)];
      c = grafo[id('c',j)];
      aant = grafo[id('a',(j+6)%8+1)];
      aprox = grafo[id('b',j%8+1)];
      aproxprox = grafo[id('a',(j+1)%8+1)];
      aantant  = grafo[id('a',(j+5)%8+1)];

      no->adj.push_back(b);
      no->adj.push_back(aprox);
      no->adj.push_back(aant);

      no->captura.push_back(Captura(b,c));
      no->captura.push_back(Captura(aant,aantant));
      no->captura.push_back(Captura(aprox,aproxprox));
   }

   //--- Pecas --------------------
   for(j=1;j<=16;j++)
   {  no = grafo[id('a',j)];
      peca = new Peca(no);
      vermelhas.push_back(peca);
      no->tipo = PECA_VERMELHA;
      no->peca = peca;
   }
   for(j=1;j<=8;j++)
   {  no = grafo[id('g',j)];
      peca = new Peca(no);
      amarelas.push_back(peca);
      no->tipo = PECA_AMARELA;
      no->peca = peca;

      no = grafo[id('f',j)];
      peca = new Peca(no);
      amarelas.push_back(peca);
      no->tipo = PECA_AMARELA;
      no->peca = peca;
   }
}

/*-------------------------------------------------------------------*/
void Siege::conecta()
{  comunica = new ComunicaUDP(IP_SIMULADOR,PORTA_SIMULADOR);
}

/*-------------------------------------------------------------------*/
short int Siege::verifica_vencedor()
{  if(vermelhas.size()==0)
      return PECA_AMARELA;
   else
   {  if(grafo["h1"]->tipo==PECA_VERMELHA)
         return PECA_VERMELHA;
   }
   return SEM_VENCEDOR;
}

/*-------------------------------------------------------------------*/
bool Siege::posicao_valida(char &letra, int nro)
{  letra = tolower(letra);
   return (letra=='h' && nro==1 ||
           letra<='g' && letra>='e' && nro<=8  && nro>=1 ||
           letra<='d' && letra>='a' && nro<=16 && nro>=1);
}

/*-------------------------------------------------------------------*/
void Siege::executa_jogada()
{  int i;
   bool flag_posicao_valida;
   char s[30];

   // --- origem -------------------------
   flag_posicao_valida = false;
   do
   {  pf("Origem: ");
      scanf("%s",s);
      if(sscanf(s,"%c%d",&letra,&nro)==2)
      {  if(posicao_valida(letra, nro))
         {  origem = grafo[id(letra,nro)];
            if(origem->tipo==vez) //tem peca vermelha na casa informada
               flag_posicao_valida = true;
         }
      }
      if(!flag_posicao_valida)
         tela.imprime_colorido(AZUL, " => Posição inválida.\n");
   } while(!flag_posicao_valida);

   // --- destino -------------------------
   flag_posicao_valida = false;
   flag_captura = false;

   do
   {  pf("Destino: ");
      scanf("%s",s);
      if(sscanf(s,"%c%d",&letra,&nro)==2)
      {  if(posicao_valida(letra,nro))
         {  destino = grafo[id(letra,nro)];
            for(i=0; i<origem->adj.size() && origem->adj[i]!=destino; i++);
            if(i<origem->adj.size()) // eh uma posicao adjacente a origem
            {  if(destino->tipo==VAZIO) // estah vazia
                  flag_posicao_valida = true;
               else
               {  if(destino->tipo==!vez) // nao estah vazia, mas tem uma amarela
                  {  for(i=0;i<origem->captura.size() && origem->captura[i].no_captura!=destino;i++);
                     if(i<origem->captura.size() && origem->captura[i].prox_no->tipo==VAZIO) // eh uma posicao de captura e o prox_no estah vazio
                     {  flag_captura = true;
                        flag_posicao_valida = true;
                        pos_captura = destino;
                        destino = origem->captura[i].prox_no;
                     }
                  }
               }
            }
            else
            {  if(destino->tipo==VAZIO) // nao eh adjacente, estah vazio, verifica se eh uma posicao apos captura
               {  for(i=0;i<origem->captura.size() && origem->captura[i].prox_no!=destino;i++);
                  if(i<origem->captura.size()) // eh posicao apos captura
                  {  if(origem->captura[i].no_captura->tipo==!vez) // tem peca a ser capturada
                     {  flag_captura = true;
                        flag_posicao_valida = true;
                        pos_captura = origem->captura[i].no_captura;
                     }
                  }
               }
            }
         }
      }
      if(!flag_posicao_valida)
         tela.imprime_colorido(AZUL, " => Posição inválida.\n");
   } while(!flag_posicao_valida);


   if(!flag_captura)
      comunica->envia("De "+origem->id+" para "+destino->id);
   else
      comunica->envia("De "+origem->id+" para "+destino->id+" captura "+pos_captura->id);
}

/*-------------------------------------------------------------------*/
void Siege::jogo()
{  int res, i;
   char s[30];
   Peca *peca;

   vez = PRIMEIRO_JOGADOR;
   while(verifica_vencedor()==SEM_VENCEDOR)
   {  if(vez==PECA_VERMELHA)
         tela.imprime_colorido(VERMELHO, "\n => Jogador Vermelho: \n");
      else
         tela.imprime_colorido(AMARELO, "\n => Jogador Amarelo: \n");

      if(vez==cor_jogador)
      {  do
         {   executa_jogada();
             while(comunica->recebe()<=0);
         } while(strcmp(comunica->buf,"ok"));
      }
      else
      {  while(comunica->recebe()<=0);
         pf("%s\n",comunica->buf);
         res = sscanf(comunica->buf,"%s %c%d %s %c%d %s %c%d",s,&letra,&nro,s,&letrades,&nrodes,s,&letracap,&nrocap);
         flag_captura = res==9;

         origem = grafo[id(letra,nro)];
         destino = grafo[id(letrades,nrodes)];
      }

      destino->peca = origem->peca;
      destino->tipo = origem->tipo;
      destino->peca->pos = destino;

      origem->peca = NULL;
      origem->tipo = VAZIO;

      if(flag_captura)
      {  pf("Captura em %s\n",pos_captura->id.c_str());
         peca = pos_captura->peca;

         if(pos_captura->tipo==PECA_AMARELA)
         {  for(i=0; i<amarelas.size() && amarelas[i]!=peca; i++);
            delete amarelas[i];
            amarelas.erase(amarelas.begin()+i);
         }
         else
         {  for(i=0; i<vermelhas.size() && vermelhas[i]!=peca; i++);
            delete vermelhas[i];
            vermelhas.erase(vermelhas.begin()+i);
         }

         pos_captura->peca = NULL;
         pos_captura->tipo = VAZIO;
      }

      vez=!vez;
   }
}

/*****************************************************************/
NoPosicao::NoPosicao()
{  tipo = VAZIO;
   peca = NULL;
}

/*****************************************************************/
Captura::Captura(NoPosicao *no_captura,NoPosicao *prox_no)
{  this->prox_no = prox_no;
   this->no_captura = no_captura;
}

/*****************************************************************/
Peca::Peca(NoPosicao *pos)
{  this->pos = pos;
}
