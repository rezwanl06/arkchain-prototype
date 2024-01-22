#include <iostream>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include "key_reader.h"

RSA *read_private_key(const std::string &private_key_file) {
    FILE *private_key_fp = fopen(private_key_file.c_str(), "rb");
    if (!private_key_fp) {
        std::cerr << "Error opening private key file." << std::endl;
        return nullptr;
    }

    RSA *rsa_private_key = PEM_read_RSAPrivateKey(private_key_fp, nullptr, nullptr, nullptr);

    fclose(private_key_fp);

    if (!rsa_private_key) {
        std::cerr << "Error reading private key." << std::endl;
    }

    return rsa_private_key;
}

RSA *read_public_key(const std::string &public_key_file) {
    FILE *public_key_fp = fopen(public_key_file.c_str(), "rb");
    if (!public_key_fp) {
        std::cerr << "Error opening public key file." << std::endl;
        return nullptr;
    }

    RSA *rsa_public_key = PEM_read_RSAPublicKey(public_key_fp, nullptr, nullptr, nullptr);

    fclose(public_key_fp);

    if (!rsa_public_key) {
        std::cerr << "Error reading public key." << std::endl;
    }

    return rsa_public_key;
}
