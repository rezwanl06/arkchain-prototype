#ifndef DATA_TREE_H
#define DATA_TREE_H

#include "arkblock.h"

class TreeNode {
    public:
        ArkBlock block;
        int height;  // Height of the subtree rooted at this node
        TreeNode* left;
        TreeNode* right;

        TreeNode(const ArkBlock& block);
};

class DataTree {
    private:
        TreeNode* root;
        std::mutex tree_mutex;

        TreeNode* insert(TreeNode* node, const ArkBlock& newBlock);
        TreeNode* merge_nodes(TreeNode* node1, TreeNode* node2);
        int height(TreeNode* node);
        int balance_factor(TreeNode* node);
        TreeNode* rotate_left(TreeNode* y);
        TreeNode* rotate_right(TreeNode* x);

    public:
        DataTree();
        TreeNode* get_root() const;
        void add_entry(const ArkBlock& block);
        void display_tree() const;
        void traverse_tree(TreeNode* node) const;
        TreeNode* construct_balanced_tree(const std::vector<TreeNode*>& nodes, int start, int end);
        bool verify_arkblock(const ArkBlock& block) const;
        ~DataTree();
        void clear_tree();
};



#endif