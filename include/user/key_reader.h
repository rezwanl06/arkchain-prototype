#ifndef KEY_READER_H
#define KEY_READER_H

#include <iostream>
#include <openssl/rsa.h>
#include <openssl/pem.h>

RSA *read_private_key(const std::string &private_key_file);

RSA *read_public_key(const std::string &public_key_file);


#endif