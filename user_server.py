import threading
import socket
from sys import argv

from lib.cluster.service_registry import ServiceRegistry

def handle_client(client_socket):
    try:
        # Receive data from client
        request = client_socket.recv(1024)
        message = request.decode()
        print("[Received] From Client:", message)
        response = service_registry.decode_message(message)
        # Echo the received data back to the client
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
    print("Service Registry is listening on", server_address)

    try:
        while True:
            # Wait for a connection
            client_socket, client_address = server_socket.accept()
            print("Connection from", client_address)

            # Handle client in a new thread
            client_handler = threading.Thread(target=handle_client, args=(client_socket,))
            client_handler.start()
    except KeyboardInterrupt:
        # Handle keyboard interrupt (Ctrl+C)
        print("Server shutting down...")
    finally:
        # Close the server socket
        server_socket.close()

service_registry = None

if __name__ == "__main__":
    if len(argv) != 3:
        print("Usage: python3 node.py <service_name> <service_address>")
        exit(1)
    service_registry = ServiceRegistry(argv[1], int(argv[2]))
    server_thread = threading.Thread(target=server, args=(service_registry.get_service_address(),))
    server_thread.start()