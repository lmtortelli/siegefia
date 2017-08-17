#include "siege.h"

Siege::Siege()
{   tela.fim_cor();
    tela.fim_negrito();
}

/*-------------------------------------------------------------------*/
Siege::~Siege()
{  unsigned i;
   map <string, NoPosicao *>::iterator it;
   for(i=0;i<vermelhas.size();i++)
      delete vermelhas[i];
   for(i=0;i<amarelas.size();i++)
      delete amarelas[i];
   for(it=grafo.begin();it!=grafo.end();it++)
      delete it->second;
}

/*-------------------------------------------------------------------*/
string Siege::id(char letra, unsigned short ind)
{  char s[30];
   sprintf(s,"%c%d",letra,ind);
   return string(s);
}

/*-------------------------------------------------------------------*/
void Siege::jogo()
{  int vez, nro, i;   
   char s[30], letra;
   bool flag_posicao_valida, flag_captura;
   NoPosicao *origem, *destino, *pos_captura;
   float pos[3];
   Peca *peca;

   vez = PRIMEIRO_JOGADOR;
   while(verifica_vencedor()==SEM_VENCEDOR)
   {  if(vez==PECA_VERMELHA)
         tela.imprime_colorido(VERMELHO, "\n => Jogador Vermelho: \n");
      else
         tela.imprime_colorido(AMARELO, "\n => Jogador Amarelo: \n");

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

      //--- movimento ---------------------------
      pf("Movimenta de %s para %s\n",origem->id.c_str(),destino->id.c_str());
      destino->peca = origem->peca;
      destino->tipo = origem->tipo;
      destino->peca->pos = destino;

      origem->peca = NULL;
      origem->tipo = VAZIO;

      pos[2] = Z0;
      pos[0] = destino->x;
      pos[1] = destino->y;
      simxSetObjectPosition(clientID, destino->peca->handle, -1, pos, simx_opmode_oneshot_wait);

      if(flag_captura)
      {  pf("Captura em %s\n",pos_captura->id.c_str());
         peca = pos_captura->peca;
         simxRemoveObject(clientID, peca->handle, simx_opmode_oneshot);

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

/*-------------------------------------------------------------------*/
void Siege:: jogo_rede()
{  int vez, nro, nrodes,i;
   unsigned int nro_jogadas=1;
   bool nova_jogada=1;
   char letra,letrades;
   bool flag_origem_valida, flag_destino_valido, flag_captura;
   NoPosicao *origem, *destino, *pos_captura;
   float pos[3];
   Peca *peca;
   char s[20];

   comunica[0] = new ComunicaUDP(PORTA_VERMELHO,"Vermelho");
   comunica[1] = new ComunicaUDP(PORTA_AMARELO,"Amarelo");

   vez = PRIMEIRO_JOGADOR;
   while(verifica_vencedor()==SEM_VENCEDOR)
   {  if(nova_jogada)
      {  tela.negrito();
         if(vez==PECA_VERMELHA)
         {  tela.cor(VERMELHO);
            pf("\n %u => Jogador Vermelho: \n",nro_jogadas);
         }
         else
         {  tela.cor(AMARELO);
            pf("\n %u => Jogador Amarelo: \n",nro_jogadas);
         }
         tela.fim_cor(); tela.fim_negrito();
         while(comunica[vez]->recebe()<=0);
      }

      //-----------------------------------------------
      do
      {  flag_origem_valida = false;
         flag_destino_valido = false;
         flag_captura = false;

         if(sscanf(comunica[vez]->buf,"%s %c%d %s %c%d",s,&letra,&nro,s,&letrades,&nrodes)==6)
         {  pf("Origem: %c%d\n",letra,nro);
            pf("Destino: %c%d\n",letrades,nrodes);

            // --- origem -------------------------
            if(posicao_valida(letra, nro))
            {  origem = grafo[id(letra,nro)];
               if(origem->tipo==vez) //tem peca vermelha na casa informada
                  flag_origem_valida = true;
            }

            // --- destino -------------------------
            if(posicao_valida(letrades,nrodes))
            {  destino = grafo[id(letrades,nrodes)];
               for(i=0; i<origem->adj.size() && origem->adj[i]!=destino; i++);
               if(i<origem->adj.size()) // eh uma posicao adjacente a origem
               {  if(destino->tipo==VAZIO) // estah vazia
                     flag_destino_valido = true;
                  else
                  {  if(destino->tipo==!vez) // nao estah vazia, mas tem uma amarela
                     {  for(i=0;i<origem->captura.size() && origem->captura[i].no_captura!=destino;i++);
                        if(i<origem->captura.size() && origem->captura[i].prox_no->tipo==VAZIO) // eh uma posicao de captura e o prox_no estah vazio
                        {  flag_captura = true;
                           flag_destino_valido = true;
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
                           flag_destino_valido = true;
                           pos_captura = origem->captura[i].no_captura;
                        }
                     }
                  }
               }
            }
         }
         if(!flag_origem_valida || !flag_destino_valido || (!nova_jogada && flag_captura==false))
         {  comunica[vez]->envia("Posição inválida");
            tela.imprime_colorido(AZUL, " => Posição inválida.\n");
            while(comunica[vez]->recebe()<=0);
         }
      } while(!flag_origem_valida || !flag_destino_valido || (!nova_jogada && flag_captura==false));
      //-----------------------------------------------
      comunica[vez]->envia("ok");
      if(!flag_captura)
         comunica[!vez]->envia("De "+origem->id+" para "+destino->id);
      else
         comunica[!vez]->envia("De "+origem->id+" para "+destino->id+" captura "+pos_captura->id);

      //--- movimento ---------------------------
      pf("Movimenta de %s para %s\n",origem->id.c_str(),destino->id.c_str());
      destino->peca = origem->peca;
      destino->tipo = origem->tipo;
      destino->peca->pos = destino;

      origem->peca = NULL;
      origem->tipo = VAZIO;

      pos[2] = Z0;
      pos[0] = destino->x;
      pos[1] = destino->y;
      simxSetObjectPosition(clientID, destino->peca->handle, -1, pos, simx_opmode_oneshot_wait);

      if(flag_captura)
      {  pf("Captura em %s\n",pos_captura->id.c_str());
         peca = pos_captura->peca;
         simxRemoveObject(clientID, peca->handle, simx_opmode_oneshot);

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
      //--- Verifica capturas consecutivas -----
      while(comunica[vez]->recebe()<=0);
      if(strcmp(comunica[vez]->buf,"fim")==0)
      {  comunica[!vez]->envia("fim");
         vez=!vez;
         nro_jogadas++;
         nova_jogada=1;
      }
      else
         nova_jogada=0;
      //else captura consecutiva recebe continua
   }

   tela.negrito();
   if(verifica_vencedor()==PECA_AMARELA)
   {  tela.cor(AMARELO);
      pf("\n\n---------------------------------------------\n   Amarelo Venceu !!!!! Nro Jogadas: %u\n---------------------------------------------\n\n",nro_jogadas-1);
   }
   else
   {  tela.cor(VERMELHO);
      pf("\n\n---------------------------------------------\n  Vermelho Venceu !!!!! Nro Jogadas: %u\n---------------------------------------------\n\n",nro_jogadas-1);
   }
   tela.fim_cor(); tela.fim_negrito();

   delete comunica[0];
   delete comunica[1];
}

/*-------------------------------------------------------------------*/
bool Siege::posicao_valida(char &letra, int nro)
{  letra = tolower(letra);
   return (letra=='h' && nro==1 ||
           letra<='g' && letra>='e' && nro<=8  && nro>=1 ||
           letra<='d' && letra>='a' && nro<=16 && nro>=1);
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
void Siege::monta_grafo()
{  char i, s[50];
   int j, res;
   string iden;
   NoPosicao *no;
   int nro_handles;
   int *handles;
   float ang16, ang8, cang, sang, ang, r, pos[3];
   Peca *peca;

   //--- Nos ----------------------
   pf("Criando Nodos ... \n");
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
   pf("Criando Lista de Adjacencias ... \n");
   //--- h -------------
   no = grafo["h1"];
   for(j=1;j<=8;j++)
   {  no->adj.push_back(grafo[id('g',j)]);
      no->captura.push_back(Captura(grafo[id('g',j)], grafo[id('f',j)]));
   }

   //--- g -------------
   for(j=1;j<=8;j++)
   {  NoPosicao *gprox, *gant, *f, *e, *gantant, *gproxprox, *h, *gat;
      no = grafo[id('g',j)];

      gprox = grafo[id('g',j%8+1)];
      gant  = grafo[id('g',(j+6)%8+1)];
      gproxprox = grafo[id('g',(j+1)%8+1)];
      gantant  = grafo[id('g',(j+5)%8+1)];
      f = grafo[id('f',j)];
      e = grafo[id('e',j)];
      h = grafo["h1"];
      gat = grafo[id('g',(j+3)%8+1)];

      no->adj.push_back(gprox);
      no->adj.push_back(gant);

      no->adj.push_back(h);
      no->adj.push_back(f);

      no->captura.push_back(Captura(f, e));
      no->captura.push_back(Captura(gant, gantant));
      no->captura.push_back(Captura(gprox, gproxprox));
      no->captura.push_back(Captura(h,gat));
   }

   //--- f -------------
   for(j=1;j<=8;j++)
   {  NoPosicao *e, *g, *d, *fprox, *fant;
      no = grafo[id('f',j)];
      e = grafo[id('e',j)];
      g = grafo[id('g',j)];
      d = grafo[id('d',2*j-1)];
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
      d = grafo[id('d',2*j-1)];
      c = grafo[id('c',2*j-1)];

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
      c = grafo[id('c',j)];
      b = grafo[id('b',j)];
      dprox = grafo[id('d',j%16+1)];
      dant  = grafo[id('d',(j+14)%16+1)];
      dproxprox = grafo[id('d',(j+1)%16+1)];
      dantant  = grafo[id('d',(j+13)%16+1)];

      no->adj.push_back(c);
      no->adj.push_back(dant);
      no->adj.push_back(dprox);

      if(j%2) //impar
      {  e = grafo[id('e',(j+1)/2)];
         f = grafo[id('f',(j+1)/2)];
         no->adj.push_back(e);
         no->captura.push_back(Captura(e,f));
      }
      no->captura.push_back(Captura(c,b));
      no->captura.push_back(Captura(dant,dantant));
      no->captura.push_back(Captura(dprox,dproxprox));
   }

   //--- c -------------
   for(j=1;j<=16;j++)
   {  NoPosicao *d, *e, *b, *a, *cprox, *cant;
      no = grafo[id('c',j)];
      d = grafo[id('d',j)];
      b = grafo[id('b',j)];
      a = grafo[id('a',j)];

      no->adj.push_back(b);
      no->adj.push_back(d);
      if(j%2) //impar
      {  cant = grafo[id('c',(j+14)%16+1)];
         no->adj.push_back(cant);
         e = grafo[id('e',(j+1)/2)];
         no->captura.push_back(Captura(d,e));
      }
      else
      {  cprox = grafo[id('c',j%16+1)];
         no->adj.push_back(cprox);
      }
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
      {  bant = grafo[id('b',(j+14)%16+1)];
         no->adj.push_back(bant);
      }
      else
      {  bprox = grafo[id('b',j%16+1)];
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
      aant = grafo[id('a',(j+14)%16+1)];
      aprox = grafo[id('a',j%16+1)];
      aproxprox = grafo[id('a',(j+1)%16+1)];
      aantant  = grafo[id('a',(j+13)%16+1)];

      no->adj.push_back(b);
      no->adj.push_back(aprox);
      no->adj.push_back(aant);

      no->captura.push_back(Captura(b,c));
      no->captura.push_back(Captura(aant,aantant));
      no->captura.push_back(Captura(aprox,aproxprox));
   }

   //--- Pecas --------------------
   pf("Criando Peças ... \n");
   for(j=1;j<=16;j+=2)
   {  no = grafo[id('d',j)];
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

//      no = grafo[id('f',j)];
//      peca = new Peca(no);
//      amarelas.push_back(peca);
//      no->tipo = PECA_AMARELA;
//      no->peca = peca;
   }

   pf("Obtendo Handles do VREP ... \n");
   simxGetObjectHandle(clientID, (const simxChar*) "PecaVermelha", (simxInt *) &(vermelhas[0]->handle), simx_opmode_oneshot_wait);
   simxGetObjectHandle(clientID, (const simxChar*) "PecaAmarela", (simxInt *) &(amarelas[0]->handle), simx_opmode_oneshot_wait);

   for(i=1;i<8;i++)
   {  sprintf(s,"PecaVermelha%d",i-1);
      res=simxGetObjectHandle(clientID, (const simxChar*) s, (simxInt *) &(vermelhas[i]->handle), simx_opmode_oneshot_wait);
      if(res!=0)
      {  simxCopyPasteObjects(clientID,  (simxInt *) &(vermelhas[0]->handle), 1,  (simxInt **) &handles, &nro_handles, simx_opmode_oneshot_wait);
         vermelhas[i]->handle = *handles;
      }
      sprintf(s,"PecaAmarela%d",i-1);
      res=simxGetObjectHandle(clientID, (const simxChar*) s, (simxInt *) &(amarelas[i]->handle), simx_opmode_oneshot_wait);
      if(res!=0)
      {  simxCopyPasteObjects(clientID,  (simxInt *) &(amarelas[0]->handle), 1,  (simxInt **) &handles, &nro_handles, simx_opmode_oneshot_wait);
         amarelas[i]->handle = *handles;
      }
   }

   //--- Posicoes -----------------
   pf("Definindo coordenadas do VREP ... \n");
   grafo["h1"]->setPos(X0,Y0);
   ang16 = M_PI/8.0-0.009;
   ang8 = M_PI/4.0-0.01;
   r = 0.4;

   ang = -M_PI/2.0-0.018;
   for(i=0;i<8;i++)
   {  cang = cos(ang);
      sang = sin(ang);
      grafo[id('g',i+1)]->setPos(cang*r+X0,sang*r+Y0);
      grafo[id('f',i+1)]->setPos(cang*r*1.8+X0,sang*r*1.8+Y0);
      grafo[id('e',i+1)]->setPos(cang*r*2.6+X0,sang*r*2.6+Y0);
      if(i==2)   ang8 += 0.01;
      if(i==3)   ang8 += 0.02;
      if(i==4)   ang8 -= 0.01;
      if(i==5)   ang8 += 0.01;
      if(i==7)   ang8 -= 0.01;
      if(i==4)  r*=0.98;
      ang += ang8;
   }

   r = 0.4;
   ang = -M_PI/2.0-0.018;
   for(i=0;i<16;i++)
   {  cang = cos(ang);
      sang = sin(ang);

      grafo[id('d',i+1)]->setPos(cang*r*3.5+X0,sang*r*3.5+Y0);
      grafo[id('c',i+1)]->setPos(cang*r*4.3+X0,sang*r*4.3+Y0);
      grafo[id('b',i+1)]->setPos(cang*r*5.1+X0,sang*r*5.1+Y0);
      grafo[id('a',i+1)]->setPos(cang*r*5.9+X0,sang*r*5.9+Y0);

      if(i==3)   ang16 += 0.02;
      if(i==4)   ang16 -= 0.01;
      if(i==5)   ang16 += 0.01;
      if(i==7)   ang16 -= 0.01;
      if(i==10)  ang16 -= 0.01;
      if(i==10)  ang16 += 0.01;
      if(i==11)  ang16 -= 0.01;
      if(i==12)  ang16 += 0.01;
      if(i==4)  r*=0.98;
      if(i==8)  r*=0.98;
      if(i==10)  r*=0.98;
      if(i==13)  r*=1.01;
      if(i==14)  r*=1.02;
      ang += ang16;
   }


   //---------------------------------
   pf("Movendo as peças para a posição correta no VREP ... \n");
   for(i=0;i<8;i++)
   {  peca = vermelhas[i];
      pos[2] = Z0;
      pos[0] = peca->pos->x;
      pos[1] = peca->pos->y;
      simxSetObjectPosition(clientID, peca->handle, -1, pos, simx_opmode_oneshot_wait);
   }

   for(i=0;i<8;i++)
   {  peca = amarelas[i] ;
      pos[2] = Z0;
      pos[0] = peca->pos->x;
      pos[1] = peca->pos->y;
      simxSetObjectPosition(clientID, peca->handle, -1, pos, simx_opmode_oneshot_wait);
   }
}

/*-------------------------------------------------------------------*/
void Siege::inicia_comunicacao_vrep()
{  pf("Tentando se conectar ao VREP ...\n");
   do
   {  clientID = simxStart((simxChar*)IP_VREP.c_str(),PORTA_VREP,true,true,2000,5);
   } while (clientID ==-1);
   pf("Conectado ao VREP!\n");
}

/*-------------------------------------------------------------------*/
void Siege::le_configuracoes()
{  ifstream arq;
   char s[100];
   string str;
   arq.open("../conf");

   pf("Carregando configurações ...\n\n");
   arq >> str >> IP_VREP;
   getline(arq,str);
   getline(arq,str);
   sscanf(str.c_str(),"%s %u ",s,&PORTA_VREP);
   getline(arq,str);

   getline(arq,str);
   sscanf(str.c_str(),"%s %u ",s,&PORTA_VERMELHO);
   getline(arq,str);

   getline(arq,str);
   sscanf(str.c_str(),"%s %u ",s,&PORTA_AMARELO);
   getline(arq,str);

   arq >> str>> PRIMEIRO_JOGADOR;

   arq.close();

   pf(" => VREP\n");
   pf("Ip: %s\n",IP_VREP.c_str());
   pf("Porta: %u\n\n",PORTA_VREP);
   pf(" => Jogador Vermelho\n");
   pf("Porta: %u\n\n",PORTA_VERMELHO);
   pf(" => Jogador Amarelo\n");
   pf("Porta: %u\n\n",PORTA_AMARELO);
   pf(" => Primeiro Jogador\n");
   pf("%s\n\n",(PRIMEIRO_JOGADOR)?"Amarelo":"Vermelho");
}

/*****************************************************************/
NoPosicao::NoPosicao()
{  tipo = VAZIO;
   peca = NULL;
}

/*-------------------------------------------------------------------*/
void NoPosicao::setPos(float x, float y)
{  this->x = x;
   this->y = y;
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




