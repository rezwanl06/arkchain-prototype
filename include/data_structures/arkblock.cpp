#include "arkblock.h"

using namespace std;

ArkBlock::ArkBlock(const string &user_public_key, const string &file_contents, const string &timestamp) : 
    user_public_key(user_public_key),
    file_contents(file_contents),
    timestamp(timestamp),
    signature("") {
        this->hash();
}

void ArkBlock::hash() {
    string data = user_public_key + file_contents + timestamp;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char *)data.c_str(), data.length(), hash);

    char hash_hex[SHA256_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        snprintf(&hash_hex[i * 2], 3, "%02x", hash[i]);
    }
    signature = hash_hex;
}

void ArkBlock::sign_block(EVP_PKEY *private_key) {
    string data = user_public_key + file_contents + timestamp;
    size_t signature_length;
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();

    EVP_DigestSignInit(ctx, nullptr, EVP_sha256(), nullptr, private_key);

    EVP_DigestSignUpdate(ctx, data.c_str(), data.length());
    EVP_DigestSignFinal(ctx, nullptr, &signature_length);

    std::vector<unsigned char> signature_buffer(signature_length);
    EVP_DigestSignFinal(ctx, signature_buffer.data(), &signature_length);

    std::vector<char> signature_hex(signature_length * 2 + 1);
    for (size_t i = 0; i < signature_length; i++) {
        snprintf(&signature_hex[i * 2], 3, "%02x", signature_buffer[i]);
    }
    signature = signature_hex.data();

    EVP_MD_CTX_free(ctx);
}

bool ArkBlock::verify_signature(EVP_PKEY *public_key) const {
    string data = user_public_key + file_contents + timestamp;
    vector<unsigned char> signature_buffer(signature.length() / 2);
    size_t signature_length = signature.length() / 2;

    for (size_t i = 0; i < signature_length; i++) {
        sscanf(signature.substr(i * 2, 2).c_str(), "%02x", &signature_buffer[i]);
    }

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestVerifyInit(ctx, nullptr, EVP_sha256(), nullptr, public_key);

    EVP_DigestVerifyUpdate(ctx, data.c_str(), data.length());
    int success = EVP_DigestVerifyFinal(ctx, signature_buffer.data(), signature_length);

    EVP_MD_CTX_free(ctx);
    return (success == 1);
}

bool ArkBlock::operator==(const ArkBlock &other) const {
    return this -> file_contents == other.get_file_contents();
}

bool ArkBlock::operator<(const ArkBlock &other) const {
    return this -> signature < other.signature;
}

bool ArkBlock::operator>(const ArkBlock &other) const {
    return this -> signature > other.signature;
}

// Getter methods for demonstration purposes
string ArkBlock::get_signature() const {
    return signature;
}

string ArkBlock::get_user_public_key() const {
    return user_public_key;
}

string ArkBlock::get_file_contents() const {
    return file_contents;
}

string ArkBlock::get_timestamp() const {
    return timestamp;
}

// Function to generate RSA key pair and return as strings
pair<string, string> generate_key_pair() {
    EVP_PKEY *pkey = EVP_PKEY_new();
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);

    EVP_PKEY_keygen_init(ctx);
    EVP_PKEY_keygen(ctx, &pkey);

    BIO *private_key_bio = BIO_new(BIO_s_mem());
    PEM_write_bio_PrivateKey(private_key_bio, pkey, nullptr, nullptr, 0, nullptr, nullptr);

    BIO *public_key_bio = BIO_new(BIO_s_mem());
    PEM_write_bio_PUBKEY(public_key_bio, pkey);

    char *private_key_buffer;
    size_t private_key_length = BIO_get_mem_data(private_key_bio, &private_key_buffer);

    char *public_key_buffer;
    size_t public_key_length = BIO_get_mem_data(public_key_bio, &public_key_buffer);

    pair<string, string> key_pair;
    key_pair.first = string(private_key_buffer, private_key_length);
    key_pair.second = string(public_key_buffer, public_key_length);

    BIO_free_all(private_key_bio);
    BIO_free_all(public_key_bio);
    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(ctx);

    return key_pair;
}
