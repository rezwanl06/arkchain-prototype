#include <iostream>
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include "include/arkblock.h"

using namespace std;

int main() {
    // Generate RSA key pair
    pair<string, string> key_pair = generate_key_pair();
    string private_key_str = key_pair.first;
    string public_key_str = key_pair.second;

    // Convert keys from strings to OpenSSL structures
    BIO *private_key_bio = BIO_new(BIO_s_mem());
    BIO_puts(private_key_bio, private_key_str.c_str());
    EVP_PKEY *private_key = PEM_read_bio_PrivateKey(private_key_bio, nullptr, nullptr, nullptr);
    BIO_free(private_key_bio);

    BIO *public_key_bio = BIO_new(BIO_s_mem());
    BIO_puts(public_key_bio, public_key_str.c_str());
    EVP_PKEY *public_key = PEM_read_bio_PUBKEY(public_key_bio, nullptr, nullptr, nullptr);
    BIO_free(public_key_bio);

    // Create a block
    string timestamp1 = "2024-01-10T12:00:00";
    ArkBlock block1(public_key_str, "File Contents 1", timestamp1);

    // Sign the block
    block1.sign_block(private_key);

    // Verify the signature
    bool is_signature_valid = block1.verify_signature(public_key);

    // Display information
    std::cout << "Block 1:" << std::endl;
    std::cout << "Signature: " << block1.get_signature() << std::endl;
    std::cout << "Signature Valid: " << (is_signature_valid ? "Yes" : "No") << std::endl;

    // Cleanup
    EVP_PKEY_free(private_key);
    EVP_PKEY_free(public_key);

    return 0;
}