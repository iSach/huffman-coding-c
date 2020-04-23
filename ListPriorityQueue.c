#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include "PriorityQueue.h"

typedef struct node_t Node;

static Node* node_create(const void* entry, double priority);

// Represents a linked node.
struct node_t {
    Node* next;
    double priority;
    const void* data;
};

struct priority_queue_t {
    // Contains the min priority element.
    Node* head;
    // Maximum queue size.
    size_t max_size;
    // Current queue size
    size_t size;
};

/**
 * Allocates and create a new node.
 * @param entry Node data
 * @param priority Node associated priority
 * @return a pointer to the new node created
 */
static Node* node_create(const void* entry, double priority) {
    Node* new_node = malloc(sizeof(Node));

    if (new_node == NULL) {
        return NULL;
    }

    new_node->data = entry;
    new_node->priority = priority;
    new_node->next = NULL;

    return new_node;
}

PriorityQueue* pqCreate(const void** entries, const double* priorities,
                        size_t length) {

    PriorityQueue* pq = malloc(sizeof(PriorityQueue));
    pq->head = NULL;
    pq->max_size = length;
    pq->size = 0;

    if (pq == NULL) {
        return NULL;
    }

    // Insert the entries.
    for (size_t i = 0; i < length; i++) {
        bool inserted = pqInsert(pq, entries[i], priorities[i]);
        if (!inserted) {
            return pq;
        }
    }

    return pq;
}

void pqFree(PriorityQueue* pQueue) {
    while (pQueue->head != NULL) {
        Node* tmp = pQueue->head;
        pQueue->head = tmp->next;
        free((void*) tmp->data);
        free(tmp);
    }

    free(pQueue);
}

bool pqInsert(PriorityQueue* pQueue, const void* entry, double priority) {
    if (pQueue->size == pQueue->max_size) {
        return false;
    }

    Node* element = node_create(entry, priority);

    if (element == NULL) {
        return false;
    }

    Node* current = pQueue->head;

    pQueue->size++;

    // first element inserted
    if (current == NULL) {
        pQueue->head = element;

        return true;
    }

    // Check if it's the new head
    if (priority <= current->priority) {
        element->next = current;
        pQueue->head = element;

        return true;
    }

    while (current->next != NULL && current->next->priority < priority) {
        current = current->next;
    }

    element->next = current->next;
    current->next = element;

    return true;
}

const void* pqExtractMin(PriorityQueue* pQueue) {
    if (pQueue->size == 0) {
        return NULL;
    }

    Node* toPop = pQueue->head;
    const void* data = toPop->data;

    pQueue->head = toPop->next;
    pQueue->size--;

    free(toPop);

    return data;
}

size_t pqSize(const PriorityQueue* pQueue) {
    return pQueue->size;
}
