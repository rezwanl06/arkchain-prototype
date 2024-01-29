import hashlib
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
