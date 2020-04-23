/* ========================================================================= *
 * Coding tree interface.
 *
 * NOTE
 * - Leaves at the same depth must be ordered by increasing frequency from
 *   left to right (as in Figure 1(b) of the statement).
 * - Going left correspond to `0`, going right correspond to `1`.
 * ========================================================================= */

#ifndef _CODING_TREE_H_
#define _CODING_TREE_H_

#include <stddef.h>

#include "BinarySequence.h"

/* Opaque structure */
typedef struct coding_tree_t CodingTree;

typedef struct decoded_t {
    char character;
    size_t nextBit;
} Decoded;

/* ------------------------------------------------------------------------- *
 * Build a coding tree which only containts one leaf.
 *
 * PARAMETERS
 * c            The char to store
 * freqency     The frequency associated to the char is some given language
 *
 * NOTE
 * The returned structure should be cleaned with `ctFree` after usage.
 *
 * RETURN
 * tree         The created leaf structure, or NULL in case of error
 * ------------------------------------------------------------------------- */
CodingTree* ctCreateLeaf(char c, double frequency);


/* ------------------------------------------------------------------------- *
 * Merge two coding trees into one.
 *
 * PARAMETERS
 * leftTree     The first coding tree
 * rightTree    The second coding tree
 *
 * RETURN
 * tree         The resulting tree, or NULL in case of error
 * ------------------------------------------------------------------------- */
CodingTree* ctMerge(CodingTree* leftTree, CodingTree* rightTree);


/* ------------------------------------------------------------------------- *
 * Free the memory allocated for the coding tree structure.
 *
 * PARAMETERS
 * tree   The coding tree structure
 * ------------------------------------------------------------------------- */
void ctFree(CodingTree* tree);


/* ------------------------------------------------------------------------- *
 * Create the optimal coding tree with the Huffman algorithm.
 *
 * PARAMETERS
 * frequencies  An array of size 127, such that frequencies[i] is the frequency
 *              of the ith ascii character
 *
 * NOTE
 * Characters with zero frequency must be part of the tree
 *
 * RETURN
 * tree         The coding tree, or NULL in case of error
 * ------------------------------------------------------------------------- */
CodingTree* ctHuffman(const double* frequencies);


/* ------------------------------------------------------------------------- *
 * Return an array of size 127 which maps ascii character their corresponding
 * code.
 *
 * PARAMETERS
 * tree     The coding tree
 *
 * NOTE
 * The returned array must be freed properly (use `biseFree` on each element)
 * and `free` for the array
 *
 * RETURN
 * table    An array of binary sequences representing the codes. Each code
 *          must be freed, as well as the array must be freed after usage
 * ------------------------------------------------------------------------- */
BinarySequence** ctCodingTable(const CodingTree* tree);

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
                 size_t start);


#endif // _CODING_TREE_H_
