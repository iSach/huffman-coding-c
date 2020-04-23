#include "coding.h"

bool encode(const CharVector* source, BinarySequence* dest,
            const CodingTree* tree, unsigned char eof) {

    BinarySequence** table = ctCodingTable(tree);
    if (!table)
        return false;

    bool success = true;
    char c;
    for (size_t i = 0; i < cvSize(source); i++) {
        c = cvGet(source, i);
        if (c < 0 || c >= 127) // Filtering out non-ascii
            continue;
        success &= biseAddSequence(dest, table[(size_t) c]);
    }

    // add end of file code
    success &= biseAddSequence(dest, table[eof]);

    for (size_t i = 0; i < 127; i++)
        biseFree(table[i]);
    free(table);

    return success;
}
