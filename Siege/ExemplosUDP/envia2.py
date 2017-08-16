import socket
UDP_IP = "127.0.0.1"
UDP_PORT = 0

def decideCor(turno):
    if(Turno.AMARELO==turno):
        UDP_PORT = 5002
    elif(Turno.VERMELHO==turno):
        UDP_PORT = 5001

def montaMensagem(mensagem):
    #['',['','']
    if(mensagem[1][1]==''):
        msg = "de "+str(mensagem[0])+" para "+str(mensagem[1][0])
    else:
        msg = "de "+str(mensagem[0])+" para "+str(mensagem[1][0])+" captura "+str(mensagem[1][1])
    return msg

def enviaMensagem(mensagem):
    print mensagem
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto(mensagem.encode('utf-8'), (UDP_IP, UDP_PORT))

def recebeMensagem():
    while True:
       data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
       print ("Recebeu mensagem:", data.decode('utf_8'))






enviaMensagem(montaMensagem(['a5',['b5','']]))
