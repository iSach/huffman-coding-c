#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include "BinarySequence.h"
#include "coding.h"
#include "PriorityQueue.h"

static const size_t ASCII_SIZE = 127;

struct coding_tree_t
{
    char* character;
    int frequency;
    CodingTree* left;
    CodingTree* right;
};

CodingTree* ctCreateLeaf(char c, double frequency){
    CodingTree* leaf = malloc(sizeof(CodingTree));
    if(leaf == NULL){
        return NULL;
    }
    leaf->left = NULL;
    leaf->right = NULL;
    leaf->character = malloc(sizeof(char));
    leaf->character[0] = c;
    leaf->frequency = frequency;
    return leaf;
}

CodingTree* ctMerge(CodingTree* leftTree, CodingTree* rightTree){
    CodingTree* parent = malloc(sizeof(CodingTree));
    if(parent == NULL){
        return NULL;
    }
    parent->right = rightTree; 
    parent->left = leftTree;
    parent->frequency = leftTree->frequency + rightTree->frequency;

    char* left_char;
    char* right_char;
    
    
    parent->character = malloc(sizeof(left_char)+sizeof(right_char));
    if(parent->character == NULL){
        return NULL;
    }

    int i = 0;
    int j = 0;

    while(left_char[i] != NULL && right_char[i] != NULL){
        if(left_char[i] != NULL && right_char[i] != NULL){
            parent->character[j] = left_char[i];
            parent->character[++j] = right_char[i];
        }
        else if(left_char[i] == NULL){
            parent->character[j] = right_char[i];
        }
        else{
            parent->character[j] = left_char[i];
        }
        j++;
        i++;
    }
    return parent;
}

void ctFree(CodingTree* tree){
    free(tree->character);
    free(tree->left);
    free(tree->right);
    free(tree);
}

CodingTree* ctHuffman(const double* frequencies){
    
    PriorityQueue* Q = pqCreate(/*Je sais pas quoi mettre*/, frequencies, ASCII_SIZE);

    for(int i = 1; i < ASCII_SIZE-1; i++){
        double** left = pqExtractMin(Q);
        CodingTree* left_leaf = ctCreateLeaf(left[0][0], left[0][1]);

        double** right = pqExtractMin(Q);
        CodingTree* right_leaf = ctCreateLeaf(right[0][0], right[0][1]);

        CodingTree* parent = ctMerge(left_leaf, right_leaf);
        pqInsert(Q,parent,/*euhhh*/);
    }
    
    return pqExtractMin(Q);
}
