#include "comunicaUDP.h"

/*-------------------------------------------------------------------*/
ComunicaUDP::ComunicaUDP(unsigned int &porta, const string &jog)
{  conectado = false;
   addrlen = sizeof(remaddr);
   this->jogador = jog;
   if((fd = socket(AF_INET, SOCK_DGRAM, 0))<0)
      pf("Não foi possível criar socket\n");

   memset((char *)&myaddr, 0, sizeof(myaddr));
   myaddr.sin_family = AF_INET;
   myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   myaddr.sin_port = htons(porta);
   if(bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr))<0)
      pf("Não foi possível executar bind\n");

   pf("Tentando se conectar ao jogador %s ...\n",jog.c_str());
   recebe();
   conectado = true;
   envia("conectado");
   pf("Conectado ao jogador %s!\n",jog.c_str());
}

//http://www.binarytides.com/programming-udp-sockets-c-linux/
//http://homepages.dcc.ufmg.br/~loureiro/rc/resources/socket-exemplo.html

/*-------------------------------------------------------------------*/
ComunicaUDP::~ComunicaUDP()
{  close(fd);
}

/*-------------------------------------------------------------------*/
int ComunicaUDP::recebe()
{  int res;
   Tela tela;
   buf[0]=0;
   res = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
   if(res>0)
   {  buf[res]=0;
      if(!strcmp(buf,"conecta") && conectado)  return -1;
      tela.cor(VERDE); tela.negrito();
      pf("Recebeu msg de %s: %s\n",jogador.c_str(),buf);
      tela.fim_cor(); tela.fim_negrito();
   }
   return res;
}

/*-------------------------------------------------------------------*/
void ComunicaUDP::envia(const string &s)
{  sendto(fd, s.c_str(), s.size(), 0, (struct sockaddr *)&remaddr, addrlen);
}

