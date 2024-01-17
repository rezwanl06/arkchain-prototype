#include "data_tree.h"
#include "arkblock.h"
#include <string>
#include<vector>
#include<stack>
#include <iostream>
#include <openssl/sha.h>

TreeNode::TreeNode(const ArkBlock& block) : block(block), hash(""), left(nullptr), right(nullptr) {
    calculate_hash();
}

void TreeNode::calculate_hash() {
    std::string combinedData = block.get_user_public_key() + block.get_file_contents() + block.get_timestamp() + hash;
    unsigned char hashedData[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)combinedData.c_str(), combinedData.length(), hashedData);

    char hashedDataHex[SHA256_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        snprintf(&hashedDataHex[i * 2], 3, "%02x", hashedData[i]);
    }
    hash = hashedDataHex;
}

DataTree::DataTree() {
    this -> root = nullptr;
}

void DataTree::add_entry(const ArkBlock& newBlock) {
    TreeNode* newNode = new TreeNode(newBlock);

    if (root == nullptr) {
        root = newNode;
    } else {
        std::vector<TreeNode*> nodesToMerge = {root, newNode};
        root = construct_balanced_tree(nodesToMerge, 0, 1);
    }
}

TreeNode* DataTree::merge_nodes(TreeNode* node1, TreeNode* node2) {
    TreeNode* parent = new TreeNode(ArkBlock("", "", "")); // Placeholder, not used in hashing
    parent->left = node1;
    parent->right = node2;
    parent->calculate_hash();
    return parent;
}

TreeNode* DataTree::construct_balanced_tree(const std::vector<TreeNode*>& nodes, int start, int end) {
    if (start == end) {
        return nodes[start];
    }

    int mid = (start + end) / 2;
    TreeNode* left = construct_balanced_tree(nodes, start, mid);
    TreeNode* right = construct_balanced_tree(nodes, mid + 1, end);

    return merge_nodes(left, right);
}

void DataTree::display_tree() const {
    traverse_tree(root);
}

void DataTree::traverse_tree(TreeNode* node) const {
    if (node) {
        traverse_tree(node->left);
        std::cout << "Hash: " << node->hash << std::endl;
        traverse_tree(node->right);
    }
}

bool DataTree::verify_arkblock(const ArkBlock& block, TreeNode* node) const {
    std::stack<TreeNode*> nodeStack;

    // Start with the root node
    nodeStack.push(root);

    while (!nodeStack.empty()) {
        TreeNode* current = nodeStack.top();
        nodeStack.pop();

        // Check if the current node's block is equal to the provided block
        if (current->block == block) {
            return true;
        }

        // Push the right child first, so it's processed next
        if (current->right != nullptr) {
            nodeStack.push(current->right);
        }

        // Push the left child, so it's processed after the right child
        if (current->left != nullptr) {
            nodeStack.push(current->left);
        }
    }

    // Block not found
    return false;
}

bool DataTree::validate_arkblock(const ArkBlock& block) const {
    return false;
}