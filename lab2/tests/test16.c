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

  for (int i = 0; i < 1023; i++) {
    arr[i] = my_malloc(ARENA_SIZE);
  }

  freelist_print(print_object);

verify_header_count(1, 1023, 2);

  for (int i = 0; i < NUM_INTS; i++) {
    my_free(arr[i]);
  }

  freelist_print(print_object);

verify_header_count(1, 0, 2);

  return 0;
} /* main() */