#include <stdio.h>

#include "test_funcs.h"
#include "../my_malloc.h"
#include "../printing.h"

#define NUM_INTS (ARENA_SIZE)

/*
 * Test a lot of allocations
 */

int main()
{
  int * arr[NUM_INTS];

  for (int i = 0; i < NUM_INTS; i++) {
    arr[i] = my_malloc(i + 1);
  }

  freelist_print(print_object);

  verify_header_count(LOG_2(NUM_INTS) - 4, NUM_INTS, 2);

  for (int i = 0; i < NUM_INTS; i++) {
    my_free(arr[i]);
  }

  freelist_print(print_object);

  verify_header_count(1, 0, 2);

  return 0;
} /* main() */