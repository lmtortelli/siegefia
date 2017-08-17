#ifndef COMUNICATCP_H
#define COMUNICATCP_H
#include <string>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "geral.h"
#include "tela.h"
#define BUFSIZE 100
using namespace std;

class ComunicaTCP
{  public:
      ComunicaTCP(unsigned int &porta, const string &jog);
      ~ComunicaTCP();

      int recebe();
      void envia(const string &s);

      struct sockaddr_in myaddr;
      struct sockaddr_in remaddr;
      socklen_t addrlen;
      int fd;
      int novo_fd;
      string jogador;
      bool conectado;

      char buf[BUFSIZE+1];
};

#endif // COMUNICATCP_H

