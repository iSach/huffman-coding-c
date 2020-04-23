#include "coding.h"

bool decode(const BinarySequence* source, CharVector* dest,
            const CodingTree* tree, unsigned char eof) {
    if (dest == NULL || tree == NULL)
        return NULL;

    // Iterate over the sequence bytes, stop if we reach the last byte.
    size_t current_bit = 0;
    while (current_bit < biseGetNumberOfBits(source)) {
        Decoded d = ctDecode(tree, source, current_bit);

        // Check if we reached the end of the sequence.
        if (d.character == eof)
            break;

        // Try to add the character to the char vector.
        bool success = cvAdd(dest, d.character);
        if (!success)
            return false;

        current_bit = d.nextBit;
    }

    return true;
}
