#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

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

CodingTree* ctHuffman(const double* frequencies) {
    CodingTree** ascii_chars = malloc(ASCII_SIZE * sizeof(CodingTree*));

    if (ascii_chars == NULL) {
        return NULL;
    }

    for (size_t c = 0; c < ASCII_SIZE; c++) {
        CodingTree* leaf = ctCreateLeaf(c, frequencies[c]);
        ascii_chars[c] = leaf;
    }

    PriorityQueue* queue = pqCreate((const void**) ascii_chars, frequencies,
                                    ASCII_SIZE);

    while (pqSize(queue) > 1) {
        CodingTree* left_leaf = (CodingTree*) pqExtractMin(queue);
        CodingTree* right_leaf = (CodingTree*) pqExtractMin(queue);

        CodingTree* parent = ctMerge(left_leaf, right_leaf);

        pqInsert(queue, parent, parent->frequency);
    }

    CodingTree* final_tree = (CodingTree*) pqExtractMin(queue);

    pqFree(queue);
    free(ascii_chars);

    return final_tree;
}

static void bise_print(BinarySequence* bs) {
    for(int i = 0; i < biseGetNumberOfBits(bs); i++) {
        printf("%d", biseGetBit(bs, i));
    }
}

BinarySequence** ctCodingTable(const CodingTree* tree) {
    BinarySequence** table = calloc(ASCII_SIZE, sizeof(BinarySequence*));

    for(char i = 0; i < ASCII_SIZE; i++) {
        BinarySequence* sequence = biseCreate();
        biseAddByte(sequence, i);
        printf("\n");
        printf("%c\n", i);
        bise_print(sequence);
        printf("\n");
        table[i] = sequence;
    }

    return table;
}

static void ctCodingTable_aux(const CodingTree* tree, BinarySequence** table,
                              BinarySequence* bin_seq) {
    if (tree == NULL || table == NULL) return;

    // Reached a leaf
    if (tree->left == NULL && tree->right == NULL) {
        char c = tree->character;
        table[(size_t) c] = bin_seq;
    } else {
        BinarySequence* left_seq = bin_seq == NULL ? biseCreate() :
                                   biseCopy(bin_seq);
        biseAddBit(left_seq, ZERO);
        ctCodingTable_aux(tree->left, table, left_seq);

        BinarySequence* right_seq = bin_seq == NULL ? biseCreate() :
                                    biseCopy(bin_seq);
        biseAddBit(right_seq, ONE);
        ctCodingTable_aux(tree->right, table, right_seq);
        biseFree(bin_seq);
    }
}

/* ------------------------------------------------------------------------- *
 * Decode a SINGLE binary code into a SINGLE character
 *
 * PARAMETERS
 * tree             The conding tree
 * encodedSequence  The code to decode
 * start            The first bit to read to decode the character
 *
 * RETURN
 * decoded A structure containing the decoded character and the index of the
 *         next bit to read for the next character decoding.
 * ------------------------------------------------------------------------- */
Decoded ctDecode(const CodingTree* tree, const BinarySequence* encodedSequence,
                 size_t start) {
    Binary bin;
    size_t offset = 0;
    const CodingTree* node = tree;

    while (node->left != NULL && node->right != NULL) {
        bin = biseGetBit(encodedSequence, start + offset);
        if (bin == ZERO) {
            node = node->left;
        } else if (bin == ONE) {
            node = node->right;
        }
        offset++;
    }

    Decoded decoded;
    decoded.nextBit = start + offset;
    decoded.character = node->character;

    return decoded;
}
