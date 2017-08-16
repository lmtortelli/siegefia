#include "comunicaTCP.h"

/*-------------------------------------------------------------------*/
ComunicaTCP::ComunicaTCP(const string  &ip, unsigned int &porta)
{  int value=1;

   buf[0]=0;
   addrlen = sizeof(remaddr);

   if((fd = socket(AF_INET, SOCK_STREAM, 0))<0)
      pf("Não foi possível criar socket\n");
   //ioctl(fd, FIONBIO, &value); // para recvfrom não bloquear

   memset((char *) &remaddr, 0, sizeof(remaddr));
   remaddr.sin_family = AF_INET;
   remaddr.sin_port = htons(porta);

   pf("Tentando se conectar ao Simulador ...\n");
   while (connect(fd, (struct sockaddr *)&remaddr , sizeof(remaddr)) < 0);

   if (inet_aton(ip.c_str(), &remaddr.sin_addr)==0)
      pf("inet_aton() falhou\n");   

   /*do
   {  envia("conecta");
      recebe();      
   } while(strcmp(buf,"conectado"));*/
   pf("Conectado ao Simulador!\n");
}

/*-------------------------------------------------------------------*/
ComunicaTCP::~ComunicaTCP()
{  close(fd);
}

/*-------------------------------------------------------------------*/
int ComunicaTCP::recebe()
{  int res;
   Tela tela;
   res = read(fd, buf, BUFSIZE);
   if(res>0)
   {  buf[res]=0;
      tela.imprime_colorido(VERDE, "Recebeu msg: ");
      tela.imprime_colorido(VERDE, buf);
      pf("\n");
   }
   return res;
}

/*-------------------------------------------------------------------*/
void ComunicaTCP::envia(const string &s)
{  write(fd, s.c_str(),s.size());
}

