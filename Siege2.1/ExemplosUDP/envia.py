import socket
UDP_IP = "127.0.0.1"
UDP_PORT = 5001
mens = "jog1!"
print("Envia mensagem:",mens)
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(mens.encode('utf-8'), (UDP_IP, UDP_PORT))
