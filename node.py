import socket
import threading
from sys import argv

from lib.user.user import User
from lib.user.key_reader import *


def process_message(message):
    if message == "Registration successful" or message == "Unegistration successful":
        return None

def handle_client(client_socket):
    try:
        # Receive data from client
        request = client_socket.recv(1024)
        while request:
            message = request.decode()
            print("[Received] From Client:", message)

            response = process_message(message)

            if response is not None:
                client_socket.send(response.encode())
            

    except ConnectionResetError:
        # Handle the case when the client disconnects abruptly (ConnectionResetError)
        print("Client disconnected.")
    except Exception as e:
        print("Exception:", e)
    finally:
        # Close the connection when done
        client_socket.close()

def server(port):
    # Create a TCP/IP socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Bind the socket to the port
    server_address = ('localhost', port)
    server_socket.bind(server_address)

    # Listen for incoming connections
    server_socket.listen(5)
    print("Server is listening on", server_address)

    try:
        while True:
            # Wait for a connection
            client_socket, client_address = server_socket.accept()

            # Handle client in a new thread
            client_handler = threading.Thread(target=handle_client, args=(client_socket,))
            client_handler.start()
    except KeyboardInterrupt:
        # Handle keyboard interrupt (Ctrl+C)
        print("Server shutting down...")
    finally:
        # Close the server socket
        server_socket.close()


def client(port, message):
    # Create a TCP/IP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Connect the socket to the server
    server_address = ('localhost', port)
    client_socket.connect(server_address)

    try:
        # Send data to server
        print("[Sent] To Server:", message)
        client_socket.sendall(message.encode())

        # Receive data from server
        response = client_socket.recv(1024)
        print("[Received] From Server:", response.decode())

    finally:
        # Close the connection when done
        client_socket.close()


if __name__ == "__main__":
    # Start server in a new thread
    if (len(argv) != 5):
        print("Usage: python3 node.py <username> <public_key_file> <private_key_file> <port>")
        exit(1)
    priv_key = read_private_key(argv[3])
    pub_key = read_public_key(argv[2])
    user = User(argv[1], pub_key, priv_key, int(argv[4]))
    server_thread = threading.Thread(target=server, args=(user.get_port(),))
    server_thread.start()

    # Start client
    client(4000, f"REGISTER {user.get_username()} {user.get_port()}")
