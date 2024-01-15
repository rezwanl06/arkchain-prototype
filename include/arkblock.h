#ifndef ARKBLOCK_H
#define ARKBLOCK_H

#include <iostream>
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>


class Block {
private:
    std::string user_public_key;
    std::string file_contents;
    std::string timestamp;
    std::string signature;

public:
    Block(const std::string &user_public_key, const std::string &file_contents, const std::string &timestamp);
    void hash();
    void sign_block(EVP_PKEY *private_key);
    bool verify_signature(EVP_PKEY *public_key);
    std::string get_signature() const;
};

std::pair<std::string, std::string> generate_key_pair();

#endif