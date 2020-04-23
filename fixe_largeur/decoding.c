#include "coding.h"


static void bise_print(BinarySequence* bs) {
    for(int i = 0; i < biseGetNumberOfBits(bs); i++) {
        printf("%d", biseGetBit(bs, i));
    }
    printf("\n");
}

bool decode(const BinarySequence* source, CharVector* dest,
            const CodingTree* tree, unsigned char eof) {
    if (dest == NULL || tree == NULL)
        return NULL;

    // Iterate over the sequence bytes, stop if we reach the last byte.
    size_t current_bit = 0;
    while (current_bit < biseGetNumberOfBits(source) - 8) {
        BinarySequence* bs = biseCreate();
        for(size_t i = 0; i < 8; i++) {
            Binary bin = biseGetBit(source, current_bit + i);
            biseAddBit(bs, bin);
        }
        unsigned char c = biseGetByte(bs, 0, ZERO);

        if (c == eof)
            break;
        cvAdd(dest, (char) c);

        biseFree(bs);

        current_bit += 8;
    }

    return true;
}

bool decode2(const BinarySequence* source, CharVector* dest,
            const CodingTree* tree, unsigned char eof) {
    if (dest == NULL || tree == NULL)
        return NULL;

    // Iterate over the sequence bytes, stop if we reach the last byte.
    size_t current_bit = 0;
    while (current_bit < biseGetNumberOfBits(source) - 7) {
        Decoded d = ctDecode(tree, source, current_bit);

        // Check if we reached the end of the sequence.
        if (d.character == eof)
            break;

        printf("%c", d.character);

        // Try to add the character to the char vector.
        bool success = cvAdd(dest, d.character);
        if (!success)
            return false;

        current_bit = d.nextBit;
    }

    return true;
}