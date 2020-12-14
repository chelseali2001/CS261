/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Chelsea Li
 * Email: lichel@oregonstate.edu
 */

#include <stdlib.h>

#include "pq.h"
#include "dynarray.h"

/*
 * Works Cited:
 * https://www.geeksforgeeks.org/binary-heap/
 */

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq {
  struct dynarray* da_void;
  struct dynarray* da_int;
};


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
  struct pq* pq = malloc(sizeof(struct pq));
  pq->da_void = NULL;
  pq->da_int = NULL;
  return pq;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
  dynarray_free(pq->da_void);
  dynarray_free(pq->da_int);
  free(pq);
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
  if (dynarray_size(pq->da_void) > 0)
    return 0;
  
  return 1;
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
  if (pq->da_void == NULL) {
    pq->da_void = dynarray_create();
    pq->da_int = dynarray_create();
  }
  
  int* p = malloc(sizeof(int));
  *p = priority;
  
  int node = dynarray_size(pq->da_void);
  dynarray_insert(pq->da_void, value);
  dynarray_insert(pq->da_int, p);
  
  while (node > 0 && *((int*)dynarray_get(pq->da_int, (node-1)/2)) > *((int*)dynarray_get(pq->da_int, node))) {
    void* temp_void = dynarray_get(pq->da_void, (node-1)/2);
    dynarray_set(pq->da_void, (node-1)/2, value);
    dynarray_set(pq->da_void, node, temp_void);
    
    int* temp_int = dynarray_get(pq->da_int, (node-1)/2);
    dynarray_set(pq->da_int, (node-1)/2, p);
    dynarray_set(pq->da_int, node, temp_int);
    
    node = (node - 1) / 2;
  }
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
  return dynarray_get(pq->da_void, 0);
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
  return *((int*)dynarray_get(pq->da_int, 0));
}


//Helper function for pq_remove_first to reorganize the binary heap
void pq_min(struct pq* pq, int n) {
  int left = n * 2 + 1;
  int right = n * 2 + 2;
  int low_pri = n;
  int size = dynarray_size(pq->da_void);
  
  if (left < size && *((int*)dynarray_get(pq->da_int, left)) < *((int*)dynarray_get(pq->da_int, n)))
    low_pri = left;
    
  if (right < size && *((int*)dynarray_get(pq->da_int, right)) < *((int*)dynarray_get(pq->da_int, low_pri)))
    low_pri = right;
  
  if (low_pri != n) {
    void* temp1v = dynarray_get(pq->da_void, n);
    void* temp2v = dynarray_get(pq->da_void, low_pri);
    dynarray_set(pq->da_void, n, temp2v);
    dynarray_set(pq->da_void, low_pri, temp1v);
    
    int* temp1i = dynarray_get(pq->da_int, n);
    int* temp2i = dynarray_get(pq->da_int, low_pri);
    dynarray_set(pq->da_int, n, temp2i);
    dynarray_set(pq->da_int, low_pri, temp1i);

    pq_min(pq, low_pri);
  }
}

/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
  void* val = dynarray_get(pq->da_void, 0);
  int size = dynarray_size(pq->da_void) - 1;
  int* temp = dynarray_get(pq->da_int, size);
 
  void* temp_void1 = dynarray_get(pq->da_void, 0);
  void* temp_void2 = dynarray_get(pq->da_void, size);
  dynarray_set(pq->da_void, 0, temp_void2);
  dynarray_set(pq->da_void, size, temp_void1);
  dynarray_remove(pq->da_void, size);
  
  int* temp_int1 = dynarray_get(pq->da_int, 0);
  int* temp_int2 = dynarray_get(pq->da_int, size);
  dynarray_set(pq->da_int, 0, temp_int2);
  dynarray_set(pq->da_int, size, temp_int1);
  free(dynarray_get(pq->da_int, size));
  dynarray_remove(pq->da_int, size);

  pq_min(pq, 0);
  
  return val;
}
