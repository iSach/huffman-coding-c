#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "CodingTree.h"
#include "CharVector.h"
#include "coding.h"

#include "PriorityQueue.h"

static const size_t ASCII_SIZE = 127;
static const size_t BUFFER_SIZE = 1024;
static const size_t CHAR_VECTOR_INIT_CAP = 100;

/* ------------------------------------------------------------------------- *
 * Parse a ascii frequency csv file.
 *
 * CSV STRUCTURE
 * Each line is a pair ascii code (in integer format)-frequency, separated
 * by a comma. There is no additionnal space before and/or after the code,
 * the comma or the frequency. The frequency is expressed as a double between
 * 0 and 1.
 * Only one blank line at the end of the file is allowed.
 *
 * PARAMETERS
 * filepath     The path to the file
 *
 * RETURN
 * frequencies  An array of size 127 with the frequency of each ascii
 *              charater or NULL in case of error.
 * ------------------------------------------------------------------------- */
static double* csvToFrequencies(const char* filepath) {
    char buffer[BUFFER_SIZE];

    FILE* fp = fopen(filepath, "r");

    double* frequencies = (double*) calloc(ASCII_SIZE, sizeof(double));
    if (!frequencies) {
        fclose(fp);
        return NULL;
    }

    char character;
    char* nextPart;

    // Read line by line
    while (fgets(buffer, BUFFER_SIZE, fp)) {
        // Parse char and frequency
        character = (char) strtol(buffer, &nextPart, 10);
        if (buffer == nextPart) // Could not parse char ascii code
        {
            free(frequencies);
            fclose(fp);
            return NULL;
        }

        while (!isdigit(*nextPart) &&
               (*nextPart) != '.') // Skip space and comma
            nextPart++;

        frequencies[(size_t) character] = strtod(nextPart, NULL);;
    }


    fclose(fp);
    return frequencies;
}


/* ------------------------------------------------------------------------- *
 * Read the binary file `path` and store its content in a BinarySequence
 * object.
 *
 * PARAMETERS
 * path             The path to the file
 *
 * RETURN
 * binarySequence   The binary sequence, or NULL in case of error
 * ------------------------------------------------------------------------- */
static BinarySequence* readBinarySequence(const char* path) {
    FILE* binFile = fopen(path, "rb");
    if (!binFile)
        return NULL;

    BinarySequence* binSeq = biseCreate();
    if (!binSeq) {
        fclose(binFile);
        return NULL;
    }

    bool success = true;
    size_t read_size;
    unsigned char buffer[BUFFER_SIZE];
    while (success &&
           (read_size = fread(buffer, sizeof(unsigned char), BUFFER_SIZE,
                              binFile)) > 0)
        for (size_t i = 0; i < read_size && success; ++i)
            success = biseAddByte(binSeq, buffer[i]);

    fclose(binFile);
    if (!success) {
        biseFree(binSeq);
        binSeq = NULL;
    }

    return binSeq;
}


/* ------------------------------------------------------------------------- *
 * Read the ascii file `path` and store its content in a CharVector
 * object.
 *
 * PARAMETERS
 * path             The path to the file
 *
 * RETURN
 * charVector       The CharVector containing the data or NULL in case of error
 * ------------------------------------------------------------------------- */
static CharVector* readAsciiText(const char* path) {
    FILE* ascii = fopen(path, "r");
    if (!ascii)
        return NULL;

    CharVector* vec = cvCreate(CHAR_VECTOR_INIT_CAP);
    if (!vec) {
        fclose(ascii);
        return NULL;
    }

    char buffer[BUFFER_SIZE];

    size_t i;
    bool success = true;
    while (success && fgets(buffer, BUFFER_SIZE, ascii)) {
        i = 0;
        while (success && buffer[i])
            success = cvAdd(vec, buffer[i++]);
    }

    fclose(ascii);
    if (!success) {
        cvFree(vec);
        vec = NULL;
    }
    return vec;
}


/* ------------------------------------------------------------------------- *
 * Read the given binary input file, decode it thanks to `tree` and save
 * the result in `outputPath`.
 *
 * PARAMETERS
 * inputPath    The path to the binary input file
 * tree         The coding tree to decode the file
 * outputPath   The path to the output file, or NULL to print on standard
 *              output
 *
 * RETURN
 * success      true in case of succes, false otherwise
 * ------------------------------------------------------------------------- */
static bool readAndDecode(const char* inputpath, const CodingTree* tree,
                          const char* outptPath, unsigned char eof) {
    FILE* output = (!outptPath) ? stdout : fopen(outptPath, "wb");

    bool success = true;

    BinarySequence* source = readBinarySequence(inputpath);
    if (!source) {
        fprintf(stderr, "Could not read binary sequence from file '%s'.\n",
                inputpath);
        fclose(output);
        return false;
    }

    CharVector* dest = cvCreate(CHAR_VECTOR_INIT_CAP);
    if (!dest) {
        fprintf(stderr, "Constructor `cvCreate` failed.\n");
        success = false;
    }

    success = success && decode(source, dest, tree, eof);

    if (!success)
        fprintf(stderr, "Could not decode binary sequence from file '%s'.\n",
                inputpath);
    else
        for (size_t i = 0; i < cvSize(dest); i++)
            fprintf(output, "%c", cvGet(dest, i));


    biseFree(source);
    cvFree(dest);
    fclose(output);
    return success;
}


/* ------------------------------------------------------------------------- *
 * Read the given ascii input file, encode it thanks to `tree` and save
 * the result in `outputPath`.
 *
 * PARAMETERS
 * inputPath    The path to the ascii input file
 * tree         The coding tree to encode the file
 * outputPath   The path to the output file, or NULL to print on standard
 *              output
 *
 * RETURN
 * success      true in case of succes, false otherwise
 * ------------------------------------------------------------------------- */
static bool readAndEncode(const char* inputpath, const CodingTree* tree,
                          const char* outptPath, bool debug,
                          unsigned char eof) {
    FILE* output = (!outptPath) ? stdout : fopen(outptPath, "wb");

    bool success = true;

    CharVector* source = readAsciiText(inputpath);
    if (!source) {
        fprintf(stderr, "Could not read ascii text from file '%s'.\n",
                inputpath);
        fclose(output);
        return false;
    }


    BinarySequence* dest = biseCreate();
    if (!dest) {
        fprintf(stderr, "Constructor `biseCreate` failed.\n");
        success = false;
    }

    success = success && encode(source, dest, tree, eof);

    if (!success)
        fprintf(stderr, "Could not encode text from file '%s'.\n",
                inputpath);
    else {
        if (debug) {
            Binary bit;
            for (size_t i = 0; i < biseGetNumberOfBits(dest); i++) {
                bit = biseGetBit(dest, i);
                switch (bit) {
                    case ZERO:
                        fprintf(output, "%c", '0');
                        break;
                    case ONE:
                        fprintf(output, "%c", '1');
                        break;
                    default:
                        fprintf(output, "ERROR");
                }
            }

        } else {
            for (size_t i = 0; i < biseGetNumberOfBytes(dest); i++) {
                unsigned char currByte = biseGetByte(dest, i, ZERO);
                fwrite(&currByte, sizeof(unsigned char), 1, output);
            }

        }
        fprintf(output, "\n");
    }


    biseFree(dest);
    cvFree(source);
    fclose(output);
    return success;
}

// test
int main() {
    size_t len = 5;
    int* numbers = malloc(len * sizeof(int));
    for (int i = 0; i < len; i++) {
        numbers[i] = i + 1;
    }

    const int** entries = malloc(len * sizeof(int*));
    for (int i = 0; i < len; i++) {
        entries[i] = &(numbers[i]);
    }

    double* prior = malloc(len * sizeof(double));
    prior[0] = 2.1;
    prior[1] = 7.3;
    prior[2] = 4.5;
    prior[3] = 1.7;
    prior[4] = 3.6;

    PriorityQueue* queue = pqCreate((const void**) entries, prior, len);

    // Extracting 4
    int* a = (int*) pqExtractMin(queue);
    printf("%d\n", *a);

    // Extracting 1
    a = (int*) pqExtractMin(queue);
    printf("%d\n", *a);

    // insert 7
    int new = 7;
    pqInsert(queue, &new, 3.7);

    // extract 5
    a = (int*) pqExtractMin(queue);
    printf("%d\n", *a);

    // extract 7
    a = (int*) pqExtractMin(queue);
    printf("%d\n", *a);

    // extract 3
    a = (int*) pqExtractMin(queue);
    printf("%d\n", *a);

    // extract 2
    a = (int*) pqExtractMin(queue);
    printf("%d\n", *a);

    a = (int*) pqExtractMin(queue);
    printf("%d\n", (a != NULL ? *a : -1));
}


/* ------------------------------------------------------------------------- *
 * NAME
 * huffman
 *
 * SYNOPSIS
 * huffman [-e] [-d] [-f] [-o outputPath] textPath csvPath
 *
 * DESCRIPTION
 * Encode (resp. decode) a given plain (resp. binary) text with the optimal
 * coding tree corresponding to some language.
 *
 * -e               Encode the text (optional). By default, the text is decoded
 * -d               Debug flag (optional). Runs the code in debug mode.
 * -f <eofChar>     Ascii integer code for the end of file character (optional).
 *                  By default, using File Separator (FS, value 28) ASCII
 *                  character.
 * -o <outptPath>   Specify the output path (optional). By default, the text
 *                  is printed on the standard output
 * textPath         The path to the plain/binary text to encode/decode
 * csvPath          The path to the CSV file containing the frequency of the
 *                  ascii characters for a given language
 *
 * RETURN
 * EXIT_SUCCESS|EXIT_FAILURE
 * ------------------------------------------------------------------------- */
int main2(int argc, char** argv) {

    /* ---------------------- PARSING COMMAND LINE ARGS --------------------- */
    if (argc < 2 || argc > 7) {
        fprintf(stderr, "USAGE: %s [-e] [-d] [-f <eofChar>] [-o <outptPath>] "
                        "<textPath> <csvPath>\n", argv[0]);
        return EXIT_FAILURE;
    }

    bool decode = true;
    bool debug = false;
    unsigned char eofChar = (char) 28;
    const char* outputPath = NULL;
    const char* textPath = NULL;
    const char* csvPath = NULL;

    int i = 0;
    while (++i < argc) {
        if (strcmp(argv[i], "-e") == 0) {
            decode = false;
        } else if (strcmp(argv[i], "-d") == 0) {
            debug = true;
        } else if (strcmp(argv[i], "-o") == 0) {
            outputPath = argv[++i];
        } else if (strcmp(argv[i], "-f") == 0) {
            eofChar = (unsigned char) strtol(argv[++i], NULL, 10);
        } else if (!textPath) {
            textPath = argv[i];
        } else
            csvPath = argv[i];
    }

    if (!textPath || !csvPath) {
        fprintf(stderr, "USAGE: %s [-e] [-d] [-f <eofChar>] [-o <outptPath>]"
                        " <textPath> <csvPath>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* ---------------------------- BUILDING TREE --------------------------- */
    double* frequencies = csvToFrequencies(csvPath);
    if (!frequencies) {
        fprintf(stderr, "Could not parse CSV. Either the format is not valid "
                        "or there was a memory error. Aborting.\n");
        return EXIT_FAILURE;
    }

    CodingTree* huffmanTree = ctHuffman(frequencies);

    printf("a\n");

    /* ----------------------------- (DE)CODING ----------------------------- */
    bool success;
    if (decode)
        success = readAndDecode(textPath, huffmanTree, outputPath, eofChar);
    else
        success = readAndEncode(textPath, huffmanTree, outputPath, debug,
                                eofChar);


    free(frequencies);
    ctFree(huffmanTree);
    if (!success) {
        fprintf(stderr, "Some error occured. Aborting.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
