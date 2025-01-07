#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "test_funcs.h"
#include "../my_malloc.h"
#include "../printing.h"

#define NUM_INTS (8)


void *thread_1(void *);
void *thread_2(void *);

/*
 * Test the thread-safety (malloc only)
 */

int main()
{
  int *arr1[NUM_INTS];
  int *arr2[NUM_INTS];


  pthread_t ptr1;
  pthread_t ptr2;

  pthread_create(&ptr1, NULL, thread_1, arr1);
  pthread_create(&ptr2, NULL, thread_2, arr2);

  pthread_join(ptr1, NULL);
  pthread_join(ptr2, NULL);

  for (int i = 0; i < NUM_INTS; i++) {
    assert(*arr1[i] == ~*arr2[i]);
  }
  return 0;
} /* main() */


void *thread_1(void *arrp) {
  int **arr = (int **)arrp;
  for (int i = 0; i < NUM_INTS; i++) {
    arr[i] = my_malloc(sizeof(int));
    *arr[i] = 0xaaaaaaaa;
  }
  return NULL;
}

void *thread_2(void *arrp) {
  int **arr = (int **)arrp;
  for (int i = 0; i < NUM_INTS; i++) {
    arr[i] = my_malloc(sizeof(int));
    *arr[i] = 0x55555555;
  }
  return NULL;
}