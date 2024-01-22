#include<iostream>
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

#include "user.h"

using namespace std;

User::User(string username, string public_key, string private_key, int port) :
    username(username), public_key(public_key), private_key(private_key), port(port) {
        this -> data_tree = new DataTree();
}

void User::create_arkblock(string file_contents, string timestamp) {
    ArkBlock *new_block = new ArkBlock(public_key, file_contents, timestamp);

    BIO *private_key_bio = BIO_new(BIO_s_mem());
    BIO_puts(private_key_bio, private_key.c_str());
    EVP_PKEY *user_private_key = PEM_read_bio_PrivateKey(private_key_bio, nullptr, nullptr, nullptr);
    BIO_free(private_key_bio);

    new_block -> sign_block(user_private_key);

    EVP_PKEY_free(user_private_key);

    this -> add_block_to_data_tree(*new_block);
}

void User::add_block_to_data_tree(const ArkBlock &block) {
    data_tree -> add_entry(block);
}

void User::request_arkblock(string file_contents) {

}

void User::print_data_tree() {
    data_tree -> display_tree();
}

bool User::verify_arkblock(const ArkBlock &block) {
    return this -> data_tree -> verify_arkblock(block);
}