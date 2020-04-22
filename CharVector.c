#include "CharVector.h"

#include <stdlib.h>

struct char_vector_t
{
    size_t size;
    size_t capacity;
    char* content;
};


CharVector* cvCreate(size_t capacity)
{
    if(capacity == 0)
        capacity = 2;

    char* content = (char*)calloc(capacity, sizeof(char));
    if(!content)
        return NULL;

    CharVector* vec = (CharVector*)malloc(sizeof(CharVector));
    if(!vec)
    {
        free(content);
        return NULL;
    }
    vec->size = 0;
    vec->capacity = capacity;
    vec->content = content;

    return vec;
}


void cvFree(CharVector* charVector)
{
    if(!charVector)
        return;
    free(charVector->content);
    free(charVector);
}


bool cvAdd(CharVector* charVector, char c)
{
    if(charVector->size == charVector->capacity)
    {
        char* newContent = (char*)realloc(charVector->content,
                                          charVector->capacity*2);
        if(!newContent)
            return false;

        charVector->content = newContent;
        charVector->capacity *= 2;
    }

    charVector->content[charVector->size++] = c;
    return true;
}

char cvGet(const CharVector* charVector, size_t index)
{
    return charVector->content[index];
}

size_t cvSize(const CharVector* charVector)
{
    return charVector->size;
}
