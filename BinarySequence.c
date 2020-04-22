#include "BinarySequence.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

const size_t DEFAULT_SIZE = 1;
const size_t BYTE_SIZE = 8;

typedef unsigned char BITS;
struct binary_sequence_t {
    /**
     * Interface must return most significant bit first.
     */
    BITS* bits;
    size_t n_bits;
    size_t n_bytes;
};

static bool increaseSize(BinarySequence* bs, size_t new_size) {
    BITS* new_bits = realloc(bs->bits, new_size * sizeof(BITS));
    if (!new_bits) {
        return false;
    }
    bs->bits = new_bits;
    bs->n_bytes = new_size;
    return true;
}

static inline BITS setBits(BITS bits, BITS* byte, size_t n_bits, size_t start_index) {
    BITS mask = ((1 << n_bits) - 1) << start_index;
    *byte = (*byte & ~mask) | ((bits << start_index) & mask);
    return *byte;
}

static inline BITS setBit(Binary value, BITS* byte, size_t sequence_index) {
    size_t index_in_byte = BYTE_SIZE - (sequence_index % BYTE_SIZE) - 1;
    return setBits(value == ONE ? 1 : 0, byte, 1, index_in_byte);
}

static inline Binary getBit(BITS byte, size_t sequence_index) {
    size_t index_in_byte = BYTE_SIZE - (sequence_index % BYTE_SIZE) - 1;
    return ((byte >> index_in_byte) & 1) ? ONE : ZERO;
}

BinarySequence* biseCreate() {
    BinarySequence* bs = malloc(sizeof(BinarySequence));
    if (!bs) {
        return NULL;
    }

    bs->bits = malloc(sizeof(BITS) * DEFAULT_SIZE);
    if (!bs->bits) {
        free(bs);
        return NULL;
    }

    bs->n_bits = 0;
    bs->n_bytes = DEFAULT_SIZE;
    return bs;
}

bool biseAddBit(BinarySequence* bs, Binary value) {
    if (value == ERROR) {
        return false;
    }
    if (bs->n_bits / 8 == bs->n_bytes) {
        if (!increaseSize(bs, 2 * bs->n_bytes)) {
            return false;
        }
    }
    size_t byte_index = bs->n_bits / BYTE_SIZE;
    setBit(value, bs->bits + byte_index, bs->n_bits++);
    return true;
}

bool biseAddByte(BinarySequence* bs, unsigned char byte) {
    if ((bs->n_bytes * BYTE_SIZE - bs->n_bits) < BYTE_SIZE) { // not enough room for a byte
        if (!increaseSize(bs, 2 * bs->n_bytes)) {
            return false;
        }
    }
    size_t in_second = bs->n_bits % BYTE_SIZE,
           in_first = BYTE_SIZE - in_second;
    if (in_second == 0) {
        bs->bits[bs->n_bits / BYTE_SIZE] = byte;
    } else {
        BITS first_mask = (1 << in_first) - 1, second_mask = (1 << in_second) - 1;
        size_t first_byte_index = bs->n_bits / BYTE_SIZE;
        setBits((byte >> in_second) & first_mask, &(bs->bits[first_byte_index]), in_first, 0);
        setBits(byte & second_mask, &(bs->bits[first_byte_index + 10]), in_second, in_first);
    }
    bs->n_bits += BYTE_SIZE;
    return true;
}

bool biseAddSequence(BinarySequence* dest, const BinarySequence* source) {
    bool success = true;
    for(size_t i = 0; i < biseGetNumberOfBits(source); ++i) {
        success &= biseAddBit(dest, biseGetBit(source, i));
    }
    return success;
}

Binary biseRemoveBit(BinarySequence* bs) {
    if (bs->n_bits == 0) {
        return ERROR;
    }
    size_t byte_index = bs->n_bits / BYTE_SIZE;
    Binary bit = getBit(bs->bits[byte_index], --bs->n_bits);
    return bit;
}

Binary biseGetBit(const BinarySequence* bs, size_t index) {
    if (index >= bs->n_bits) {
        return ERROR;
    }
    return getBit(bs->bits[index / BYTE_SIZE], index);
}

size_t biseGetNumberOfBits(const BinarySequence* bs) {
    return bs->n_bits;
}

BinarySequence* biseCopy(const BinarySequence* bs) {
    BinarySequence* cpy = malloc(sizeof(BinarySequence));
    if (!cpy) {
        return NULL;
    }
    size_t n_useful_bytes = 1 + ((bs->n_bits - 1) / BYTE_SIZE);
    cpy->bits = malloc(sizeof(BITS) * n_useful_bytes);
    if (!cpy->bits) {
        free(cpy);
        return NULL;
    }
    cpy->n_bytes = n_useful_bytes;
    cpy->n_bits = bs->n_bits;
    memcpy(cpy->bits, bs->bits, sizeof(BITS) * n_useful_bytes);
    return cpy;
}

void biseFree(BinarySequence* bs) {
    if (!bs)
        return;
    free(bs->bits);
    free(bs);
}

unsigned char biseGetByte(const BinarySequence* bs, size_t index, Binary padding) {
    if (bs->n_bits == 0) {
        return padding == ONE ? ~0 : 0;
    }
    size_t n_bytes = biseGetNumberOfBytes(bs);
    BITS byte = bs->bits[index];
    if (index == n_bytes - 1) {
        size_t missing_bits = BYTE_SIZE - bs->n_bits % BYTE_SIZE; 
        return setBits(padding == ONE ? (1 << missing_bits) - 1 : 0, &byte, missing_bits, 0);
    } else {
        return byte;
    }
}

size_t biseGetNumberOfBytes(const BinarySequence* bs) {
    return (bs->n_bits / BYTE_SIZE) + 1;
}
