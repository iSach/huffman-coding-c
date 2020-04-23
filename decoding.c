#include "coding.h"

bool decode(const BinarySequence* source, CharVector* dest,
            const CodingTree* tree, unsigned char eof) {
    if(dest == NULL || tree == NULL)
        return NULL;

    size_t current_bit = 0;
    while (current_bit < biseGetNumberOfBits(source) - 7) {
        Decoded d = ctDecode(tree, source, current_bit);
        if (d.character == eof)
            break;
        bool success = cvAdd(dest, d.character);
        if(!success)
            return false;
        current_bit = d.nextBit;
    }

    return true;
}
