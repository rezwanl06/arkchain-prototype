#ifndef DATA_TREE_H
#define DATA_TREE_H

#include "arkblock.h"

struct TreeNode {
    ArkBlock block;
    std::string hash;
    TreeNode *left;
    TreeNode *right;

    TreeNode(ArkBlock block);

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
};



#endif