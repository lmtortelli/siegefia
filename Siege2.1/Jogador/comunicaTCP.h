#ifndef COMUNICATCP_H
#define COMUNICATCP_H
#include <string>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include "geral.h"
#include "tela.h"
#define BUFSIZE 100
using namespace std;

class ComunicaTCP
{  public:
      ComunicaTCP(const string  &ip, unsigned int &porta);
      ~ComunicaTCP();

      int recebe();
      void envia(const string &s);

      struct sockaddr_in myaddr;
      struct sockaddr_in remaddr;
      socklen_t addrlen;
      int fd;

      int tamanho_recebido;
      char buf[BUFSIZE+1];
};

#endif // COMUNICATCP_H

