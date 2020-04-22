/* ========================================================================= *
 * Interface of `BinarySequence`, a structure for handling binary codes
 * ========================================================================= */

#ifndef BINARY_SEQUENCE_H_DEFINED
#define BINARY_SEQUENCE_H_DEFINED

#include <stdlib.h>
#include <stdbool.h>

typedef struct binary_sequence_t BinarySequence;

typedef enum {ZERO, ONE, ERROR} Binary;

/* ------------------------------------------------------------------------- *
 * Create an empty binary sequence.
 *
 * RETURN
 * bs     The created empty binary sequence, or NULL in case of error
  * ------------------------------------------------------------------------- */
BinarySequence* biseCreate(void);

/* ------------------------------------------------------------------------- *
 * Add a bit at the end of the sequence.
 *
 * PARAMETERS
 * bs      A valid pointer to the binary sequence
 * value   The value of the bit
 *
 * RETURN
 * success True if the bit was successfully added, false on error
  * ------------------------------------------------------------------------- */
bool biseAddBit(BinarySequence* bs, Binary value);

/* ------------------------------------------------------------------------- *
 * Add a complete byte at the end of the sequence. Most significant bits are
 * added first.
 *
 * PARAMETERS
 * bs      A valid pointer to the binary sequence
 * byte    The byte to be added
 *
 * RETURN
 * success True if the bit was successfully added, false on error
  * ------------------------------------------------------------------------- */
bool biseAddByte(BinarySequence* bs, unsigned char byte);

/* ------------------------------------------------------------------------- *
 * Append a source sequence to another (dest).
 *
 * PARAMETERS
 * dest    A valid pointer to the destination sequence
 * source  A valid pointer to the source sequence
 *
 * RETURN
 * success True if the bit was successfully added, false on error
  * ------------------------------------------------------------------------- */
bool biseAddSequence(BinarySequence* dest, const BinarySequence* source);

/* ------------------------------------------------------------------------- *
 * Remove the last bit from the sequence, has no effect if the sequence is
 * empty and returned value is undefined.
 *
 * PARAMETERS
 * bs      A valid pointer to the binary sequence
 *
 * RETURN
 * value   The value of the bit that was removed (if the sequence wasn't empty)
  * ------------------------------------------------------------------------- */
Binary biseRemoveBit(BinarySequence* bs);

/* ------------------------------------------------------------------------- *
 * Get the bit at the given index in the binary sequence.
 *
 * PARAMETERS
 * bs      A valid pointer to the binary sequence
 * index   Index of the bit to read, should be in [0, biseGetNumberOfBits(bs)[
 *
 * RETURN
 * value   The value of the requested bit
  * ------------------------------------------------------------------------- */
Binary biseGetBit(const BinarySequence* bs, size_t index);

/* ------------------------------------------------------------------------- *
 * Return the number of bits in the binary sequence.
 *
 * PARAMETERS
 * bs      A valid pointer to the binary sequence
 *
 * RETURN
 * n_bits   The number of bits in the sequence
  * ------------------------------------------------------------------------- */
size_t biseGetNumberOfBits(const BinarySequence* bs);

/* ------------------------------------------------------------------------- *
 * Return the index'th byte of the sequence. If the byte is incomplete because
 * the sequence bit length is not a multiple of 8, the least significant bits
 * are padded witt padding.
 *
 * First bits to occur in the sequence are placed in the most significant bits
 * of each byte.
 *
 * PARAMETERS
 * bs       A valid pointer to a binary sequence
 * index    The byte index
 * padding  The value to use for padding.
 *
 * RETURN
 * byte     The byte
 * ------------------------------------------------------------------------- */
unsigned char biseGetByte(const BinarySequence* bs, size_t index, Binary padding);

/* ------------------------------------------------------------------------- *
 * Return the number of bytes in the sequence
 *
 * PARAMETERS
 * bs      A valid pointer to a binary sequence
 *
 * RETURN
 * size    The number of bytes in the sequence
 * ------------------------------------------------------------------------- */
size_t biseGetNumberOfBytes(const BinarySequence* bs);

/* ------------------------------------------------------------------------- *s
 * Duplicate the given binary sequence into another sequence containing the
 * same bits.
 *
 * NOTE
 * The returned structure should be cleaned with `biseFree`
 *
 * PARAMETERS
 * bs      A valid pointer to the binary sequence
 *
 * RETURNS
 * copied   The copied sequence
  * ------------------------------------------------------------------------- */
BinarySequence* biseCopy(const BinarySequence* bs);

/* ------------------------------------------------------------------------- *
 * Free the allocated code.
 *
 * PARAMETERS
 * bs      A pointer to the binary sequence
  * ------------------------------------------------------------------------- */
void biseFree(BinarySequence* c);

#endif // BINARY_SEQUENCE_H_DEFINED
