#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include "BinarySequence.h"
#include "coding.h"
#include "PriorityQueue.h"

typedef struct element_t Element;

struct priority_queue_t
{
    size_t length;
    Element* head;
    Element* current;
    size_t size;
};

// Implémentation des éléments qui constitueront notre liste
struct element_t{
    Element* next;
    double priority;
    int index;
    const void* data;
};

Element* elementCreate(double priority, int index, const void* entry){
    Element* element = malloc(sizeof(Element));
    if(element == NULL){
        return NULL;
    }
    element->data = malloc(sizeof(void*));
    if(element->data == NULL){
        return NULL;
    }
    element->data = entry;
    element->index = index;
    element->priority = priority;
    element->next = malloc(sizeof(Element));
    element->next = NULL;
    return element;
}

// Implémentation de PriorityQueue

PriorityQueue* pqCreate(const void** entries, const double* priorities, size_t length){
    PriorityQueue* pq = malloc(sizeof(PriorityQueue));
    if(pq == NULL){
        return NULL;
    }
    int min = 99999;
    int index = 0;
    for(int i = 0; i < length; i++){
        if(priorities[i] < min){
            min = priorities[i];
            index = i;
        }
    }
    int min_prec = min;
    pq->head = elementCreate(min, index, entries[index]);
    pq->current = elementCreate(min, index, entries[index]);
    pq->size = 1;
    for(int j = 1; j < length; j++){ //On doit faire ce propriéte pour les length-1 éléments restants de priorities
        min = 99999;
        /*
        Il va falloir faire attention à plusieurs choses :
            -On ne veut pas rentrer plusieurs fois la même priority[i] dans notre PriorityQueue
            -Il faut faire attention à toujours rentrer un élément de priorité plus grande ou égale à l'élément précédent
            et donc logiquement aux éléments d'avant aussi
        */
        for(int i = 0; i < length; i++){
            if(priorities[i] < min && priorities[i] > pq->current->priority){
                min = priorities[i];
                index = i;
            }
            else if(priorities[i] < min && priorities[i] == pq->current->priority && pq->current->index < i){
                min = priorities[i];
                index = i;
            }
        }
        Element* next = elementCreate(min, index, entries[index]);
        if(j == 1){   // à la première étape il faut d'abord initialiser le next de la tête
            pq->current = next;
            pq->head->next = pq->current;
            pq->size++;
        }
        else{
            pq->current->next = next;
            pq->current = next;
            pq->size++;
        }
    }
    return pq;
}

void pqFree(PriorityQueue* pQueue){
    while(pQueue->head != NULL){
        Element* tmp = pQueue->head;
        pQueue->head = pQueue->head->next;
        free(tmp);
    }
    free(pQueue);
}

bool pqInsert(PriorityQueue* pQueue, const void* entry, double priority){
    if(pQueue->size == pQueue->length){
        return false;
    }
    Element* element = elementCreate(priority,pQueue->size + 1, entry);
    if(element== NULL){
        return false;
    }
    Element* tmp = malloc(sizeof(Element));
    if(tmp == NULL){
        return false;
    }
    tmp = pQueue->head;
    if(pQueue->head->priority > priority){
        element->next = pQueue->head;
        pQueue->head = element;
    }
    else{
        while(tmp->priority <= priority && tmp->next != NULL){
            tmp = tmp->next;
        }
        element->next = tmp->next;
        tmp->next = element;
    }
    pQueue->size++;
    return true;
}

const void* pqExtractMin(PriorityQueue* pQueue){
    if(pQueue->size == 0){
        return NULL;
    }
    void* data = malloc(sizeof(void*));
    if(data == NULL){
        return NULL;
    }
    data = pQueue->head->data;
    Element* tmp = pQueue->head;
    pQueue->head = pQueue->head->next;
    free(tmp);
    pQueue->size--;
    return data;
}

size_t pqSize(const PriorityQueue* pQueue){
    return pQueue->size;
}
