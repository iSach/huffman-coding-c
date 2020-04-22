/* ========================================================================= *
 * Minimum Priority Queue interface.
 * ========================================================================= */

#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include <stddef.h>
#include <stdbool.h>

/* Opaque structure */
typedef struct priority_queue_t PriorityQueue;


/* ------------------------------------------------------------------------- *
 * Build a priority queue with the given entries. The capacity of the queue
 * will bounded by the given number of entries.
 *
 * PARAMETERS
 * entries      The entries to store
 * priorities   The priorities corresponding to the entries
 *              priorities[i] is the priority of entries[i]
 * length       The number of entries
 *
 * NOTE
 * The returned structure should be cleaned with `pqFree` after usage.
 * The priority queue will only read the given arrays.
 *
 * RETURN
 * pq           The priority queue, or NULL in case of error.
 * ------------------------------------------------------------------------- */
PriorityQueue* pqCreate(const void** entries, const double* priorities,
                        size_t length);


/* ------------------------------------------------------------------------- *
 * Free the memory allocated for the priority queue.
 *
 * NOTE
 * The (remaining) entries will not be freed. Only the memory dedicated to
 * the priority queue structure.
 *
 * PARAMETERS
 * pQueue       A pointer to the empty priority queue to free
 * ------------------------------------------------------------------------- */
void pqFree(PriorityQueue* pQueue);


/* ------------------------------------------------------------------------- *
 * Insert `entry` into the priority queue with priority `priority`.
 *
 * PARAMETERS
 * pQueue       A valid pointer to the priority queue
 * entry        A pointer to the object to store
 * priority     The priority of the entry
 *
 * RETURN
 * inserted     true is the entry was correctly inserted, false otherwise
 * ------------------------------------------------------------------------- */
bool pqInsert(PriorityQueue* pQueue, const void* entry, double priority);


/* ------------------------------------------------------------------------- *
 * Extract and return the entry with minimum priority.
 *
 * PARAMETERS
 * pQueue       A valid pointer to a priority queue
 *
 * RETURN
 * entry        The entry of minimum priority of NULL in case of error
 * ------------------------------------------------------------------------- */
const void* pqExtractMin(PriorityQueue* pQueue);


/* ------------------------------------------------------------------------- *
 * Return the current number of entries stored in  the priority queue.
 *
 * PARAMETERS
 * pQueue       A valid pointer to a priority queue
 *
 * RETURN
 * size        The size of the priority queue
 * ------------------------------------------------------------------------- */
size_t pqSize(const PriorityQueue* pQueue);



#endif // _PRIORITY_QUEUE_H_
