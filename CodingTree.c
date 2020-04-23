#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "PriorityQueue.h"
#include "CodingTree.h"

static const size_t ASCII_SIZE = 128;

struct coding_tree_t {
    char* character;
    int frequency; // ???
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
    leaf->character = malloc(sizeof(char));
    leaf->character[0] = c;
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

    char* left_char;
    char* right_char;

    parent->character = malloc(sizeof(left_char) + sizeof(right_char));
    if (parent->character == NULL) {
        return NULL;
    }

    int i = 0;
    int j = 0;

    while (left_char[i] != '\0' && right_char[i] != '\0') {
        if (left_char[i] != '\0' && right_char[i] != '\0') {
            parent->character[j] = left_char[i];
            parent->character[++j] = right_char[i];
        } else if (left_char[i] == '\0') {
            parent->character[j] = right_char[i];
        } else {
            parent->character[j] = left_char[i];
        }
        j++;
        i++;
    }
    return parent;
}

void ctFree(CodingTree* tree) {
    free(tree->character);
    free(tree->left);
    free(tree->right);
    free(tree);
}

CodingTree* ctHuffman(const double* frequencies) {
    int** ascii_chars = malloc(ASCII_SIZE * sizeof(int*));
    for (int i = 0; i < ASCII_SIZE; i++) {
        ascii_chars[i] = &i;
    }

    PriorityQueue* Q = pqCreate((const void**) ascii_chars, frequencies,
                                ASCII_SIZE);

    for (int i = 0; i < ASCII_SIZE - 1; i++) {
        int** left = (int**) pqExtractMin(Q);
        CodingTree* left_leaf = ctCreateLeaf(left[0][0], left[0][1]);

        int** right = (int**) pqExtractMin(Q);
        CodingTree* right_leaf = ctCreateLeaf(right[0][0], right[0][1]);

        CodingTree* parent = ctMerge(left_leaf, right_leaf);
        pqInsert(Q, parent, parent->frequency);
    }

    return (CodingTree*) pqExtractMin(Q);
}

BinarySequence** ctCodingTable(const CodingTree* tree) {
    return NULL;
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
    Decoded d;
    return d;
}