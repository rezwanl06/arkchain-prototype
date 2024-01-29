import socket
import threading

def handle_client(client_socket):
    try:
        # Receive data from client
        request = client_socket.recv(1024)
        print("[Received] From Client:", request.decode())

        # Echo the received data back to the client
        client_socket.send(request)
    except Exception as e:
        print("Exception:", e)
    finally:
        client_socket.close()

def server():
    # Create a TCP/IP socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Bind the socket to the port
    server_address = ('localhost', 8888)
    server_socket.bind(server_address)

    # Listen for incoming connections
    server_socket.listen(5)
    print("Server is listening on", server_address)

    try:
        while True:
            # Wait for a connection
            client_socket, client_address = server_socket.accept()
            print("Connection from", client_address)

            # Handle client in a new thread
            client_handler = threading.Thread(target=handle_client, args=(client_socket,))
            client_handler.start()
    finally:
        server_socket.close()

def client():
    # Create a TCP/IP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Connect the socket to the server
    server_address = ('localhost', 8888)
    client_socket.connect(server_address)

    try:
        # Send data to server
        message = "Hello, server!"
        client_socket.sendall(message.encode())

        # Receive data from server
        response = client_socket.recv(1024)
        print("[Received] From Server:", response.decode())
    finally:
        client_socket.close()

if __name__ == "__main__":
    # Start server in a new thread
    server_thread = threading.Thread(target=server)
    server_thread.start()

    # Start client
    client()
