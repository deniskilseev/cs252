#include <assert.h>
#include <stdio.h>

#include "../my_malloc.h"
#include "../printing.h"

/*
 * Ensure space is allocated in the correct sizes
 */

int main()
{
  char * arr = (char *) my_malloc(1);

  size_t size = ((header *)(arr - ALLOC_HEADER_SIZE))->size - 1;

  // assert that size is a power of 2
  // assert the size is at least UNALLOC_HEADER_SIZE
  
  assert(1 << LOG_2(size) == size);
  assert((size) == UNALLOC_HEADER_SIZE);

  arr = (char *) my_malloc(17);

  size = ((header *)(arr - ALLOC_HEADER_SIZE))->size - 1;

  assert(1 << LOG_2(size) == size);
  assert((size) >= UNALLOC_HEADER_SIZE);

  return 0;
} /* main() */
