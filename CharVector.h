/* ========================================================================= *
 * Vector implementation for holding chars
 * ========================================================================= */

#ifndef _CHAR_VECTOR_H_
#define _CHAR_VECTOR_H_

#include <stddef.h>
#include <stdbool.h>

 /* Opaque structure */
typedef struct char_vector_t CharVector;


/* ------------------------------------------------------------------------- *
 * Build an empty vector of chars with initial capacity `capacity`.
 *
 * PARAMETERS
 * capacity     The initial capacity of the vector
 *
 * NOTE
 * The returned structure should be cleaned with `cvFree` after usage
 *
 * RETURN
 * charVector   The created vector, or NULL in case of error
 * ------------------------------------------------------------------------- */
CharVector* cvCreate(size_t capacity);


/* ------------------------------------------------------------------------- *
 * Free the memory allocated for the vector of chars.
 *
 * PARAMETERS
 * charVector   The vector to free
 * ------------------------------------------------------------------------- */
void cvFree(CharVector* charVector);


/* ------------------------------------------------------------------------- *
 * Add a char `c` at the end the vector `vector`
 *
 * PARAMETERS
 * charVector   A valid pointer to the vector in which to add the character
 * c            The character to add
 *
 * RETURN
 * added        `true` if the character was added. `false` in case of error
 * ------------------------------------------------------------------------- */
bool cvAdd(CharVector* charVector, char c);


/* ------------------------------------------------------------------------- *
 * Retrieve the character at index `index` for the given vector.
 *
 * PARAMETERS
 * charVector   A valid pointer to the vector to retrieve from
 * index        The index of the char. 0 <= index <= N-1
 *              where N is the size of the vector
 *
 * RETURN
 * c            The char
 * ------------------------------------------------------------------------- */
char cvGet(const CharVector* charVector, size_t index);


/* ------------------------------------------------------------------------- *
 * Return the size of the vector (aka. the number of elements).
 *
 * PARAMETERS
 * charVector   A valid pointer to a vector
 *
 * RETURN
 * size         The size of the vector
 * ------------------------------------------------------------------------- */
size_t cvSize(const CharVector* charVector);

#endif // _CHAR_VECTOR_H_
