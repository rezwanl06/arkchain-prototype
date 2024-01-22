#include <iostream>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include "include/user/key_reader.h"

int main() {
    const std::string private_key_file = "keys/private_key1.pem";
    const std::string public_key_file = "keys/public_key1.pem";

    RSA *rsa_private_key = read_private_key(private_key_file);
    RSA *rsa_public_key = read_public_key(public_key_file);

    if (rsa_private_key && rsa_public_key) {
        std::cout << "Private and public keys read successfully." << std::endl;

        // Use rsa_private_key and rsa_public_key as needed...

        RSA_free(rsa_private_key);
        RSA_free(rsa_public_key);
    } else {
        std::cerr << "Failed to read keys." << std::endl;
    }

    return 0;
}
