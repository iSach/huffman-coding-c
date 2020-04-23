/* ========================================================================= *
 * Coding and decoding interface
 * ========================================================================= */

#ifndef _CODING_H_
#define _CODING_H_

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

#include "BinarySequence.h"
#include "CodingTree.h"
#include "CharVector.h"

/* ------------------------------------------------------------------------- *
 * Encode an ascii encoded text using the given coding tree.
 *
 * PARAMETERS
 * source     A vector containing the characters to encode.
 * dest       A binary sequence where to write the encoded text.
 * tree       The coding tree to use for encoding. All characters found in 
 *  		  source should be contained in this tree.
 * eof        The eof file character to add at the end of the sequence to 
 *			  indicate the end of the encoded content. 
 *
 * RETURN
 * success    True on success, false on error
 * ------------------------------------------------------------------------- */
bool encode(const CharVector* source, BinarySequence* dest, const CodingTree* tree, unsigned char eof);

/* ------------------------------------------------------------------------- *
 * Decode an encoded text using the given coding tree. 
 *
 * PARAMETERS
 * source     The binary sequence to decode.
 * dest       A vector where to write the decoded characters.
 * tree       The coding tree to use for encoding. All codes found in 
 *            source should have a corresponding decoding path in this tree.
 * eof        The character indicating the end of the encoded content. When
 *            reached, the decode function should ignore the remaining bits
 *            in source.
 *
 * RETURN
 * success    True on success, false on error
 * ------------------------------------------------------------------------- */
bool decode(const BinarySequence* source, CharVector* dest, const CodingTree* tree, unsigned char eof);

#endif // _CODING_H_
