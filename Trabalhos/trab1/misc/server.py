import socket
import os

# Crie um objeto de socket do lado do servidor
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Associe o socket a um endereço e porta
server_address = ('localhost', 25459)  # Use o endereço e a porta desejados
server_socket.bind(server_address)

# Comece a ouvir por conexões de clientes
server_socket.listen(2)  # Aceita até 2 clientes

print("Aguardando conexões...")

# Espere pelos dois jogadores se conectarem
player1_socket, player1_address = server_socket.accept()
print(f"Jogador 1 ({player1_address}) conectado.")

player2_socket, player2_address = server_socket.accept()
print(f"Jogador 2 ({player2_address}) conectado.")

# Envie uma mensagem inicial aos jogadores
player1_socket.sendall("Vamos começar.".encode())
player2_socket.sendall("Vamos começar.\nEspere o jogador 1 definir as regras do jogo!".encode())

i = 0

# Loop principal do jogo
while True:

    if i % 2 == 0:
        print('Aguardando cliente 1 escrever.')
        player1_socket.sendall("Sua vez de jogar.".encode())
        letra_jogador1 = player1_socket.recv(2).decode()
        player1_socket.sendall(f"Você enviou a letra {letra_jogador1}".encode())
        
        pipe_fd = os.open("pipe.txt", os.O_WRONLY)
        os.write(pipe_fd, letra_jogador1.encode())
        os.close(pipe_fd)

    else:
        print('Aguardando cliente 2 escrever.\n')
        player2_socket.sendall("Sua vez de jogar.".encode())
        letra_jogador2 = player2_socket.recv(2).decode()
        player2_socket.sendall(f"Você enviou a letra {letra_jogador2}".encode())

        pipe_fd = os.open("pipe.txt", os.O_WRONLY)
        os.write(pipe_fd, letra_jogador2.encode())
        os.close(pipe_fd)

    i += 1
    if i >= 6:
        break

# Feche os sockets do jogador 1 e jogador 2
player1_socket.close()
player2_socket.close()

# Feche o socket do servidor
server_socket.close()
