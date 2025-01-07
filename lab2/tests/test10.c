#include <stdio.h>

#include "../my_malloc.h"
#include "../printing.h"

/*
 * Ensure data is written to the correct spot (malloc only)
 */

int main()
{
  int * arr;

  arr = (int *) my_malloc(16 * sizeof(int));

  for (int i = 0; i < 16; i++) {
    arr[i] = i + 0xFF00FF00;
  }


  for (size_t * p = g_base + 2 * ALLOC_HEADER_SIZE;
    (char *) p < ((char *) g_base) + 6 * ALLOC_HEADER_SIZE;
    p++)
  {
    printf("%p\n", (void *)*p);
  }

  return 0;
} /* main() */

