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
        #self.sock.bind((self.UDP_IP, self.UDP_PORT))
        self.enviaMensagem("conecta")

    def montaMensagem(self,mensagem):
        #['',['','']
        if(mensagem[1][1]==''):
            msg = "de "+str(mensagem[0])+" para "+str(mensagem[1][0])
        else:
            msg = "de "+str(mensagem[0])+" para "+str(mensagem[1][0])+" captura "+str(mensagem[1][1])
        return msg

    def enviaMensagem(self,mensagem):
        self.sock.sendto(mensagem.encode('utf-8'), (self.UDP_IP, self.UDP_PORT))

    def recebeMensagem(self):
        #self.sock.bind((UDP_IP, UDP_PORT))
        while True:
           data, addr = self.sock.recvfrom(1024) # buffer size is 1024 bytes
           if(data!="ok" and data!='' and data!='conectado'):
               return (self.decodeMensagem(data))

    def decodeMensagem(self,mensagem):
        msg = mensagem.split()
        print msg
        if("captura" in msg):
            return [msg[1],[msg[3],msg[5]]]
        else:
            return [msg[1],[msg[3],'']]

    #enviaMensagem(montaMensagem(['a5',['b5','']]))
    #decodeMensagem("de a1 para b2")
