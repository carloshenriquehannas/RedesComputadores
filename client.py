import socket

# Crie um objeto de socket do lado do cliente
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Conecte-se ao servidor
server_address = ('localhost', 25459)  # Use o endereço e a porta do servidor
client_socket.connect(server_address)

i = 0

while True:

    mensagem = client_socket.recv(1024).decode()
    print(mensagem)

    if "Sua vez de jogar" in mensagem:
        letra = input("Digite uma letra do alfabeto: ").upper()
        client_socket.sendall(letra.encode())

    i += 1
    if i >= 6:
        break

# Feche o socket do cliente
client_socket.close()
