#ifndef COMUNICAUDP_H
#define COMUNICAUDP_H
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

class ComunicaUDP
{  public:
      ComunicaUDP(const string  &ip, unsigned int &porta);
      ~ComunicaUDP();

      int recebe();
      void envia(const string &s);

      struct sockaddr_in myaddr;
      struct sockaddr_in remaddr;
      socklen_t addrlen;
      int fd;

      int tamanho_recebido;
      char buf[BUFSIZE+1];
};

#endif // COMUNICAUDP_H

