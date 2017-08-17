import socket
from turno import Turno

class Client:
    sock = None
    UDP_PORT = 0
    UDP_IP = ""

    def __init__(self,turno,ip):
        self.UDP_IP = ip
        if(Turno.AMARELO==turno):
            self.UDP_PORT = 5002
        elif(Turno.VERMELHO==turno):
            self.UDP_PORT = 5001

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.enviaMensagem("conecta")

    def montaMensagem(self,mensagem):
        if(mensagem[1][1]==''):
            msg = "de "+str(mensagem[0])+" para "+str(mensagem[1][0])
        else:
            msg = "de "+str(mensagem[0])+" para "+str(mensagem[1][0])+" captura "+str(mensagem[1][1])
        return msg

    def enviaMensagem(self,mensagem):
        self.sock.sendto(mensagem.encode('utf-8'), (self.UDP_IP, self.UDP_PORT))

    def recebeMensagem(self):
        while True:
           data, addr = self.sock.recvfrom(1024)
           if(data!='' and data!='conectado' and data!='fim' and data!='ok'):
               return self.decodeMensagem(data)
           elif(data=='fim'):
               return 'fim'
           else:
               return None

    def decodeMensagem(self,mensagem):
        print "Mensagem Recebida "+str(mensagem)
        msg = mensagem.split()
        if("captura" in msg):
            return [msg[1],[msg[3],msg[5]]]
        else:
            return [msg[1],[msg[3],'']]
