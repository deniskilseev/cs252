#include <stdio.h>

#include "test_funcs.h"
#include "../my_malloc.h"

#define NUM_INTS (4)

/*
 * Test that the correct number of alloc, free, and fencepost blocks
 * are created for a simple job (malloc & free)
 */

int main()
{
  int * arr[NUM_INTS];

  int arena_order = LOG_2(ARENA_SIZE);

  int chunk_order = LOG_2(16 * sizeof(int) + ALLOC_HEADER_SIZE) + 1;

  int num_free = arena_order - chunk_order;

  for (int i = 0; i < NUM_INTS; i++) {
    if (i % 2) num_free--;
    arr[i] = (int *) my_malloc(16 * sizeof(int));
    verify_header_count(num_free, i + 1, 2);
  }
  printf("Part 1 passed\n");

  for (int i = 0; i < NUM_INTS - 1; i++) {
    if (!(i % 2)) num_free++;
    my_free(arr[i]);
    verify_header_count(num_free, NUM_INTS - i - 1, 2);
  }
  printf("Part 2 passed\n");
  my_free(arr[NUM_INTS - 1]);
  verify_header_count(1, 0, 2);

  printf("Part 3 passed\n");

  return 0;
} /* main() */
