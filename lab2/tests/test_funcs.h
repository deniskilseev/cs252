#ifndef TEST_FUNCS_H
#define TEST_FUNCS_H

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>


#include "../my_malloc.h"
#include "../printing.h"

static inline header * right_neighbor(header * h)
{
  // printf("status:  %d\n", STATUS(h));
  if (STATUS(h) == FENCEPOST) {
    return (header *) (((char *) h) + ALLOC_HEADER_SIZE);
  } else {
    return (header *) (((char *) h) + TRUE_SIZE(h));
  }
} /* right_neighbor() */

/*
 * Checks the heap to ensure the correct number of each type of block
 *
 * Only works on non-interrupted address spaces
 */

void verify_header_count(int num_free, int num_alloc, int num_fence){
  header *h = (header *) g_base;
  header *heap_current = (header *) sbrk(0);
  int true_free = 0;
  int true_alloc = 0;
  int true_fence = 0;
  do {
    // printf("heap current: %p\n", heap_current);
    // printf("h: %p\n", h);
    print_object(h);
    if (h->size & ALLOCATED) {
      true_alloc++;
    } else if (h->size & FENCEPOST) {
      true_fence++;
    } else {
      true_free++;
    }
    if (TRUE_SIZE(h) == 0) {
      break;
    }
  } while ((h = right_neighbor(h)) <= g_last_right_fence_post);
  printf("g_heap_end: ");
  print_pointer(g_heap_end);
  puts("");
  if ((num_fence != true_fence) ||
      (num_alloc != true_alloc) ||
      (num_free != true_free)) {
    fprintf(stderr, "Free:\n\tExpected %d\n\tGot %d\n", num_free, true_free);
    fprintf(stderr, "Alloc:\n\tExpected %d\n\tGot %d\n", num_alloc, true_alloc);
    fprintf(stderr, "Fence:\n\tExpected %d\n\tGot %d\n", num_fence, true_fence);
    assert(false);
  }
} /* verify_header_count() */

#endif
