#include "comunicaUDP.h"

/*-------------------------------------------------------------------*/
ComunicaUDP::ComunicaUDP(const string  &ip, unsigned int &porta)
{  int value=1;

   buf[0]=0;
   addrlen = sizeof(remaddr);

   if((fd = socket(AF_INET, SOCK_DGRAM, 0))<0)
      pf("Não foi possível criar socket\n");
   ioctl(fd, FIONBIO, &value); // para recvfrom não bloquear

   memset((char *) &remaddr, 0, sizeof(remaddr));
   remaddr.sin_family = AF_INET;
   remaddr.sin_port = htons(porta);
   if (inet_aton(ip.c_str(), &remaddr.sin_addr)==0)
      pf("inet_aton() falhou\n");

   pf("Tentando se conectar ao Simulador ...\n");
   do
   {  envia("conecta");
      recebe();
   } while(strcmp(buf,"conectado"));
   pf("Conectado ao Simulador!\n");
}

/*-------------------------------------------------------------------*/
ComunicaUDP::~ComunicaUDP()
{  close(fd);
}

/*-------------------------------------------------------------------*/
int ComunicaUDP::recebe()
{  int res;
   Tela tela;
   res = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
   if(res>0)
   {  buf[res]=0;
      tela.imprime_colorido(VERDE, "Recebeu msg: ");
      tela.imprime_colorido(VERDE, buf);
      pf("\n");
   }
   return res;
}

/*-------------------------------------------------------------------*/
void ComunicaUDP::envia(const string &s)
{  sendto(fd, s.c_str(), s.size(), 0, (struct sockaddr *)&remaddr, addrlen);
}

