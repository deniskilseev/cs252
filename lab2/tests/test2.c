#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "../my_malloc.h"

/*
 * Ensures that no memory is allocated before the program begins
 *
 * Checks for valid sbrk size allocations (multiples of ARENA_SIZE + sizeof(header))
 */

int main()
{
  assert(sbrk(0) == g_base);

  int * arr = (int *) my_malloc(ARENA_SIZE - ALLOC_HEADER_SIZE);

  assert(((char *)sbrk(0)) == ((char *)g_base) + ARENA_SIZE + (2 * ALLOC_HEADER_SIZE));

  for(int i = 0; i < NUM_LISTS; i++) {
    assert(g_freelists[i] == NULL);
  }


  return 0;
} /* main() */
