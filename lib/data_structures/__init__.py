import hashlib
import threading
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import rsa, padding
from cryptography.hazmat.primitives import hashes

class ArkBlock:
    def __init__(self, user_public_key, file_contents, timestamp):
        self.user_public_key = user_public_key
        self.file_contents = file_contents
        self.timestamp = timestamp
        self.signature = ""
        self.hash()

    def hash(self):
        data = self.user_public_key + self.file_contents + self.timestamp
        hashed_data = hashlib.sha256(data.encode()).hexdigest()
        self.signature = hashed_data

    def sign_block(self, private_key):
        data = self.user_public_key + self.file_contents + self.timestamp
        data = data.encode()
        signature = private_key.sign(data, padding.PSS(mgf=padding.MGF1(hashes.SHA256()), salt_length=padding.PSS.MAX_LENGTH), hashes.SHA256())
        self.signature = signature.hex()

    def verify_signature(self, public_key):
        data = self.user_public_key + self.file_contents + self.timestamp
        data = data.encode()
        signature = bytes.fromhex(self.signature)
        try:
            public_key.verify(signature, data, padding.PSS(mgf=padding.MGF1(hashes.SHA256()), salt_length=padding.PSS.MAX_LENGTH), hashes.SHA256())
            return True
        except Exception as e:
            return False

    def __eq__(self, other):
        return self.file_contents == other.file_contents

    def __lt__(self, other):
        return self.signature < other.signature

    def __gt__(self, other):
        return self.signature > other.signature

# Function to generate RSA key pair and return as strings
def generate_key_pair():
    private_key = rsa.generate_private_key(public_exponent=65537, key_size=2048, backend=default_backend())
    public_key = private_key.public_key()

    private_key_pem = private_key.private_bytes(encoding=serialization.Encoding.PEM, format=serialization.PrivateFormat.TraditionalOpenSSL, encryption_algorithm=serialization.NoEncryption())
    public_key_pem = public_key.public_bytes(encoding=serialization.Encoding.PEM, format=serialization.PublicFormat.SubjectPublicKeyInfo)

    return private_key_pem.decode(), public_key_pem.decode()

class TreeNode:
    def __init__(self, block):
        self.block = block
        self.height = 1
        self.left = None
        self.right = None

class DataTree:
    def __init__(self):
        self.root = None
        self.tree_mutex = threading.Lock()

    def height(self, node):
        return node.height if node else 0

    def balance_factor(self, node):
        return self.height(node.left) - self.height(node.right) if node else 0

    def rotate_left(self, y):
        x = y.right
        T2 = x.left

        # Perform rotation
        x.left = y
        y.right = T2

        # Update heights
        y.height = max(self.height(y.left), self.height(y.right)) + 1
        x.height = max(self.height(x.left), self.height(x.right)) + 1

        return x

    def rotate_right(self, x):
        y = x.left
        T2 = y.right

        # Perform rotation
        y.right = x
        x.left = T2

        # Update heights
        x.height = max(self.height(x.left), self.height(x.right)) + 1
        y.height = max(self.height(y.left), self.height(y.right)) + 1

        return y

    def insert(self, node, new_block):
        # Standard BST insertion
        if not node:
            return TreeNode(new_block)

        if new_block < node.block:
            node.left = self.insert(node.left, new_block)
        elif new_block > node.block:
            node.right = self.insert(node.right, new_block)
        else:
            # Duplicate blocks are not allowed
            return node

        # Update height of the current node
        node.height = max(self.height(node.left), self.height(node.right)) + 1

        # Get the balance factor to check whether this node became unbalanced
        balance = self.balance_factor(node)

        # Left Heavy (LL)
        if balance > 1 and new_block < node.left.block:
            return self.rotate_right(node)

        # Right Heavy (RR)
        if balance < -1 and new_block > node.right.block:
            return self.rotate_left(node)

        # Left-Right Heavy (LR)
        if balance > 1 and new_block > node.left.block:
            node.left = self.rotate_left(node.left)
            return self.rotate_right(node)

        # Right-Left Heavy (RL)
        if balance < -1 and new_block < node.right.block:
            node.right = self.rotate_right(node.right)
            return self.rotate_left(node)

        return node

    def add_entry(self, new_block):
        with self.tree_mutex:
            self.root = self.insert(self.root, new_block)

    def display_tree(self):
        self.traverse_tree(self.root)

    def traverse_tree(self, node):
        if node:
            self.traverse_tree(node.left)
            print("Hash:", node.block.get_signature())
            self.traverse_tree(node.right)

    def verify_arkblock(self, block):
        nodeStack = []

        # Start with the root node
        nodeStack.append(self.root)

        while nodeStack:
            current = nodeStack.pop()

            # Check if the current node's block is equal to the provided block
            if current.block == block:
                return True

            # Push the right child first, so it's processed next
            if current.right:
                nodeStack.append(current.right)

            # Push the left child, so it's processed after the right child
            if current.left:
                nodeStack.append(current.left)

        # Block not found
        return False
