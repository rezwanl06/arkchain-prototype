#ifndef USER_H
#define USER_H

#include<iostream>
#include "../data_structures/data_tree.h"
#include "../data_structures/arkblock.h"


class User {
    private:
        std::string username;
        std::string public_key;
        std::string private_key;
        int port;
        DataTree *data_tree;
    public:
        User(std::string username, std::string public_key, std::string private_key, int port);
        void create_arkblock(std::string file_contents, std::string timestamp);
        void request_arkblock(std::string file_contents);
        void add_block_to_data_tree(const ArkBlock &block);
        void print_data_tree();
        bool verify_arkblock(const ArkBlock &block);
};





#endif