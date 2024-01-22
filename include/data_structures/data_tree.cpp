#include <string>
#include<vector>
#include<stack>
#include <iostream>
#include <openssl/sha.h>

#include "data_tree.h"
#include "arkblock.h"

TreeNode::TreeNode(const ArkBlock& block) : block(block), left(nullptr), right(nullptr) {}

TreeNode* DataTree::get_root() const {
    return this -> root;
}

DataTree::DataTree() : root(nullptr) {}

int DataTree::height(TreeNode* node) {
    return (node == nullptr) ? 0 : node->height;
}

int DataTree::balance_factor(TreeNode* node) {
    return (node == nullptr) ? 0 : height(node->left) - height(node->right);
}

TreeNode* DataTree::rotate_left(TreeNode* y) {
    TreeNode* x = y->right;
    TreeNode* T2 = x->left;

    // Perform rotation
    x->left = y;
    y->right = T2;

    // Update heights
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    return x;
}

TreeNode* DataTree::rotate_right(TreeNode* x) {
    TreeNode* y = x->left;
    TreeNode* T2 = y->right;

    // Perform rotation
    y->right = x;
    x->left = T2;

    // Update heights
    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    return y;
}

TreeNode* DataTree::insert(TreeNode* node, const ArkBlock& newBlock) {
    // Standard BST insertion
    if (node == nullptr) {
        return new TreeNode(newBlock);
    }

    if (newBlock < node->block) {
        node->left = insert(node->left, newBlock);
    } else if (newBlock > node->block) {
        node->right = insert(node->right, newBlock);
    } else {
        // Duplicate blocks are not allowed
        return node;
    }

    // Update height of the current node
    node->height = 1 + std::max(height(node->left), height(node->right));

    // Get the balance factor to check whether this node became unbalanced
    int balance = balance_factor(node);

    // Left Heavy (LL)
    if (balance > 1 && newBlock < node->left->block) {
        return rotate_right(node);
    }

    // Right Heavy (RR)
    if (balance < -1 && newBlock > node->right->block) {
        return rotate_left(node);
    }

    // Left-Right Heavy (LR)
    if (balance > 1 && newBlock > node->left->block) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    // Right-Left Heavy (RL)
    if (balance < -1 && newBlock < node->right->block) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

void DataTree::add_entry(const ArkBlock& newBlock) {
    std::lock_guard<std::mutex> lock(tree_mutex);
    root = insert(root, newBlock);
}

TreeNode* DataTree::merge_nodes(TreeNode* node1, TreeNode* node2) {
    TreeNode* parent = new TreeNode(ArkBlock("", "", "")); // Placeholder, not used in hashing
    parent->left = node1;
    parent->right = node2;
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
        std::cout << "Hash: " << node->block.get_signature() << std::endl;
        traverse_tree(node->right);
    }
}

bool DataTree::verify_arkblock(const ArkBlock& block) const {
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

DataTree::~DataTree() {
    this -> clear_tree();
}

void DataTree::clear_tree() {
    if (!root) {
            return;
        }

        std::stack<TreeNode*> nodeStack;
        TreeNode* current = root;

        while (current || !nodeStack.empty()) {
            while (current) {
                nodeStack.push(current);
                current = current->left;
            }

            current = nodeStack.top();
            nodeStack.pop();

            TreeNode* temp = current->right;
            delete current;

            current = temp;
        }

        root = nullptr;
}