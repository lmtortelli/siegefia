#include "comunicaTCP.h"

/*-------------------------------------------------------------------*/
ComunicaTCP::ComunicaTCP(unsigned int &porta, const string &jog)
{  conectado = false;
   addrlen = sizeof(remaddr);
   this->jogador = jog;
   if((fd = socket(AF_INET, SOCK_STREAM, 0))<0)
      pf("Não foi possível criar socket\n");

   memset((char *)&myaddr, 0, sizeof(myaddr));
   myaddr.sin_family = AF_INET;
   myaddr.sin_addr.s_addr =  htonl(INADDR_ANY); // qq ip
   myaddr.sin_port = htons(porta);

   if(bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr))<0)
      pf("Não foi possível executar bind\n");
   listen(fd,1);

   pf("Tentando se conectar ao jogador %s = %d...\n",jog.c_str(),fd);
   addrlen = sizeof(remaddr);
   if((novo_fd = accept(fd, (struct sockaddr *) &remaddr, &addrlen))<0)
      pf("Erro em aceitar conexão\n");

   /*recebe();
   conectado = true;
   envia("conectado");*/
   pf("Conectado ao jogador %s!\n",jog.c_str());
}

//http://www.binarytides.com/server-client-example-c-sockets-linux/
/*-------------------------------------------------------------------*/
ComunicaTCP::~ComunicaTCP()
{  close(fd);
}

/*-------------------------------------------------------------------*/
int ComunicaTCP::recebe()
{  int res;
   Tela tela;
   buf[0]=0;

   res = read(novo_fd, buf, BUFSIZE);
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
void ComunicaTCP::envia(const string &s)
{  write(novo_fd, s.c_str(), s.size());
}

