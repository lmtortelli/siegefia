import socket
from turno import Turno

class Client:
    sock = None

    def __init__
    def montaMensagem(self,mensagem):
        #['',['','']
        if(mensagem[1][1]==''):
            msg = "de "+str(mensagem[0])+" para "+str(mensagem[1][0])
        else:
            msg = "de "+str(mensagem[0])+" para "+str(mensagem[1][0])+" captura "+str(mensagem[1][1])
        return msg

    def enviaMensagem(self,mensagem,turno):
        UDP_IP = "127.0.0.1"
        if(Turno.AMARELO==turno):
            UDP_PORT = 5002
        elif(Turno.VERMELHO==turno):
            UDP_PORT = 5001
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.sendto(mensagem.encode('utf-8'), (UDP_IP, UDP_PORT))

    def recebeMensagem(self,turno):
        UDP_IP = "127.0.0.1"
        if(Turno.AMARELO==turno):
            UDP_PORT = 5002
        elif(Turno.VERMELHO==turno):
            UDP_PORT = 5001
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.bind((UDP_IP, UDP_PORT))
        while True:
           data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
           if(data!="ok" and data!=''):
               return (decodeMensagem(data))

    def decodeMensagem(self,mensagem):
        msg = mensagem.split()
        #print mensagem.split()
        if("captura" in msg):
            print [msg[1],[msg[3],msg[5]]]
        else:
            print [msg[1],[msg[3],'']]

    #enviaMensagem(montaMensagem(['a5',['b5','']]))
    #decodeMensagem("de a1 para b2")
