#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include "PriorityQueue.h"
#include "CodingTree.h"

static const size_t ASCII_SIZE = 127;

static void ctCodingTable_aux(const CodingTree* tree, BinarySequence** table,
                              BinarySequence* bin_seq);

struct coding_tree_t {
    char character;
    double frequency;

    CodingTree* left;
    CodingTree* right;
};

CodingTree* ctCreateLeaf(char c, double frequency) {
    CodingTree* leaf = malloc(sizeof(CodingTree));
    if (leaf == NULL) {
        return NULL;
    }
    leaf->left = NULL;
    leaf->right = NULL;
    leaf->character = c;
    leaf->frequency = frequency;
    return leaf;
}

CodingTree* ctMerge(CodingTree* leftTree, CodingTree* rightTree) {
    CodingTree* parent = malloc(sizeof(CodingTree));
    if (parent == NULL) {
        return NULL;
    }

    parent->right = rightTree;
    parent->left = leftTree;
    parent->frequency = leftTree->frequency + rightTree->frequency;

    return parent;
}

void ctFree(CodingTree* tree) {
    if(tree->left != NULL) {
        ctFree(tree->left);
    }
    if(tree->right != NULL) {
        ctFree(tree->right);
    }
    free(tree);
}
