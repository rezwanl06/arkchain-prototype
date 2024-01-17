#ifndef ARKBLOCK_H
#define ARKBLOCK_H

#include <iostream>
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

class ArkBlock {
    private:
        std::string user_public_key;
        std::string file_contents;
        std::string timestamp;
        std::string signature;

    public:
        ArkBlock(const std::string &user_public_key, const std::string &file_contents, const std::string &timestamp);
        void hash();
        void sign_block(EVP_PKEY *private_key);
        bool verify_signature(EVP_PKEY *public_key) const;
        std::string get_signature() const;
        std::string get_timestamp() const;
        std::string get_user_public_key() const;
        std::string get_file_contents() const;
        bool operator==(const ArkBlock &other) const;
        bool operator<(const ArkBlock &other) const;

};

std::pair<std::string, std::string> generate_key_pair();

#endif