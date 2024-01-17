#ifndef DATA_TREE_H
#define DATA_TREE_H

#include "arkblock.h"

class TreeNode {
    public:
        ArkBlock block;
        std::string hash;
        TreeNode *left;
        TreeNode *right;

        TreeNode(const ArkBlock &block);

        void calculate_hash();
};

class DataTree {
    private:
        TreeNode *root;
    public:
        DataTree();
        TreeNode* merge_nodes(TreeNode *node1, TreeNode *node2);
        void add_entry(const ArkBlock &block);
        void display_tree() const;
        void traverse_tree(TreeNode *node) const;
        TreeNode* construct_balanced_tree(const std::vector<TreeNode*>& nodes, int start, int end);
        bool verify_arkblock(const ArkBlock &block, TreeNode *node) const;
        bool validate_arkblock(const ArkBlock &block) const;
};



#endif