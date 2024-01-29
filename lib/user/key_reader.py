from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization

def read_private_key(private_key_file):
    with open(private_key_file, "rb") as private_key_fp:
        try:
            private_key = serialization.load_pem_private_key(private_key_fp.read(), password=None, backend=default_backend())
            return private_key
        except Exception as e:
            print("Error reading private key:", e)
            return None

def read_public_key(public_key_file):
    with open(public_key_file, "rb") as public_key_fp:
        try:
            public_key = serialization.load_pem_public_key(public_key_fp.read(), backend=default_backend())
            return public_key
        except Exception as e:
            print("Error reading public key:", e)
            return None
