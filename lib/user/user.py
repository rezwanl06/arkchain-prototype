import hashlib
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import padding
from cryptography.hazmat.primitives.asymmetric import rsa

from lib.data_structures.arkblock import ArkBlock
from lib.data_structures.data_tree import DataTree

class User:
    def __init__(self, username, public_key, private_key, port):
        self.username = username
        self.public_key = public_key
        self.private_key = private_key
        self.port = port
        self.data_tree = DataTree()

    def create_arkblock(self, file_contents, timestamp):
        new_block = ArkBlock(self.public_key, file_contents, timestamp)

        private_key_bytes = self.private_key.encode()
        user_private_key = serialization.load_pem_private_key(private_key_bytes, password=None, backend=default_backend())

        new_block.sign_block(user_private_key)

        self.add_block_to_data_tree(new_block)

    def add_block_to_data_tree(self, block):
        self.data_tree.add_entry(block)

    def request_arkblock(self, file_contents):
        pass  # Implementation of requesting arkblock

    def print_data_tree(self):
        self.data_tree.display_tree()

    def verify_arkblock(self, block):
        return self.data_tree.verify_arkblock(block)

    def get_public_key(self):
        return self.public_key

    def get_username(self):
        return self.username

    def get_port(self):
        return self.port
