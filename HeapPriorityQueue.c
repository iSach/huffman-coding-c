#include "PriorityQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct priority_queue_t {
    // Stores the entries.
    const void** heap;

    // Stores the priorities associated to the elements in heap.
    double* priorities;

    // Current number of entries.
    size_t length;

    // Maximum size. Defined by the initial amount of entries given.
    size_t max_size;
};

/**
 * Re-orders the heap, considering the left and right sub-trees are heaps
 * @param queue The queue to re-order.
 * @param node The node to start from.
 */
static void min_heapify(PriorityQueue* queue, size_t node);

/**
 * Swaps two entries.
 * @param array The entries array
 * @param a First index to swap
 * @param b Second index to swap
 */
static void swap_pointers(const void** array, size_t a, size_t b);

/**
 * Swaps two priorities.
 * @param array The priorities array
 * @param a First index to swap
 * @param b Second index to swap
 */
static void swap_priority(double* array, size_t a, size_t b);

/**
 * Swaps two elements of a queue, by swapping the corresponding elements
 * in the entries array, and in the priorities array.
 * @param queue The queue to swap elements in.
 * @param a First index to swap
 * @param b Second index to swap
 */
static void swap(PriorityQueue* queue, size_t a, size_t b);

/**
 * Returns the parent index of an element from its index;
 * @param elem_index The element index.
 * @return The parent index of the given element.
 */
static size_t parent(size_t elem_index) {
    return (size_t) floor(((double) elem_index) / 2);
}

static void swap_pointers(const void** array, size_t a, size_t b) {
    const void* temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

static void swap_priority(double* array, size_t a, size_t b) {
    double temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

static void swap(PriorityQueue* queue, size_t a, size_t b) {
    swap_pointers(queue->heap, a, b);
    swap_priority(queue->priorities, a, b);
}

static void min_heapify(PriorityQueue* queue, size_t node) {
    size_t left = 2 * node + 1;
    size_t right = 2 * node + 2;

    size_t length = queue->length;
    double* priorities = queue->priorities;

    // Compare priorities to find the lowest priority between the current node,
    // and its left and right nodes.
    size_t min_priority = node;
    if (left < length && priorities[left] < priorities[min_priority]) {
        min_priority = left;
    }

    if (right < length && priorities[right] < priorities[min_priority]) {
        min_priority = right;
    }

    if (node != min_priority) {
        swap(queue, node, min_priority);
        min_heapify(queue, min_priority);
    }
}

PriorityQueue* pqCreate(const void** entries, const double* priorities,
                        size_t length) {
    PriorityQueue* pQueue = malloc(sizeof(PriorityQueue));
    if (pQueue == NULL) {
        perror("Error in allocating memory.");
        exit(EXIT_FAILURE);
    }

    const void** heap_array = malloc(length * sizeof(void*));
    if (heap_array == NULL) {
        perror("Error in allocating memory.");
        exit(EXIT_FAILURE);
    }

    double* priorities_array = malloc(length * sizeof(double));
    if (priorities_array == NULL) {
        perror("Error in allocating memory.");
        exit(EXIT_FAILURE);
    }

    pQueue->heap = heap_array;
    pQueue->priorities = priorities_array;
    pQueue->length = length;
    pQueue->max_size = length;

    // Copy arrays.
    for (int i = 0; i < length; i++) {
        heap_array[i] = entries[i];
        priorities_array[i] = priorities[i];
    }

    // Build min-heap
    for (int i = floor(((double) length) / 2); i >= 0; i--) {
        min_heapify(pQueue, i);
    }

    return pQueue;
}

void pqFree(PriorityQueue* pQueue) {
    free(pQueue->heap);
    free(pQueue->priorities);
    free(pQueue);
}

bool pqInsert(PriorityQueue* pQueue, const void* entry, double priority) {
    /* // reallocate if needed
     if (pQueue->length + 1 > pQueue->max_size) {
         size_t new_max_size = 2 * pQueue->max_size;
         pQueue->heap = realloc(pQueue->heap, new_max_size * sizeof(void*));
         if (pQueue->heap == NULL) {
             perror("Error in reallocating memory.");
             exit(EXIT_FAILURE);
         }
         pQueue->priorities = realloc(pQueue->priorities,
                                      new_max_size * sizeof(double));
         if (pQueue->priorities == NULL) {
             perror("Error in reallocating memory.");
             exit(EXIT_FAILURE);
         }
         pQueue->max_size = new_max_size;
     }*/
    if (pQueue->length + 1 > pQueue->max_size) {
        return false;
    }

    if(entry == NULL) {
        return false;
    }

    pQueue->length++;

    int node = (int) pQueue->length - 1;

    pQueue->heap[node] = entry;
    pQueue->priorities[node] = priority;

    while (node > 0 &&
           pQueue->priorities[parent(node)] > pQueue->priorities[node]) {
        swap(pQueue, node, parent(node));
        node = parent(node);
    }

    return true;
}

const void* pqExtractMin(PriorityQueue* pQueue) {
    if (pQueue->length == 0) {
        return NULL;
    }

    const void* min_priority_elem = pQueue->heap[0];

    size_t last_index = (int) pQueue->length - 1;
    swap(pQueue, 0, last_index);
    pQueue->length--;
    min_heapify(pQueue, 0);

    return min_priority_elem;
}

size_t pqSize(const PriorityQueue* pQueue) {
    return pQueue->length;
}