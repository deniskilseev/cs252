#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "test_funcs.h"
#include "../my_malloc.h"
#include "../printing.h"

#define NUM_INTS (50)

/*
 * make sbrk thread safe by creating a threadsafe wrapper
 */

static pthread_mutex_t g_mutex_sbrk;

void *__real_sbrk(intptr_t increment);


void *__wrap_sbrk(intptr_t increment)
{
  pthread_mutex_lock(&g_mutex_sbrk);
  void* ret = __real_sbrk(increment);
  pthread_mutex_unlock(&g_mutex_sbrk);
  return ret;
} /* __wrap_sbrk() */


/*
 * end sbrk thread safe wrapper
 */

void *sbrk_thread(void *);
void *malloc_thread(void *);

/*
 * Test the thread-safety (malloc & sbrk)
 */

int main()
{

  pthread_mutex_init(&g_mutex_sbrk, NULL);


  int *arr_sbrk[NUM_INTS];
  int *arr_malloc[NUM_INTS];

  pthread_t sbrk_ptr;
  pthread_t malloc_ptr;

  pthread_create(&sbrk_ptr, NULL, (void *)sbrk_thread, arr_sbrk);
  pthread_create(&malloc_ptr, NULL, (void *) malloc_thread, arr_malloc);

  pthread_join(sbrk_ptr, NULL);
  pthread_join(malloc_ptr, NULL);

  for (int i = 0; i < NUM_INTS; i++) {
    assert(*arr_sbrk[i] == ~*arr_malloc[i]);
  }

  return 0;
} /* main() */


void *sbrk_thread(void *arrp) {
  int **arr = (int **)arrp;
  for (int i = 0; i < NUM_INTS; i++) {
    arr[i] = sbrk(sizeof(int));
    *arr[i] = 0xaaaaaaaa;
  }
  return NULL;
}

void *malloc_thread(void *arrp) {
  int **arr = (int **)arrp;
  for (int i = 0; i < NUM_INTS; i++) {
    arr[i] = my_malloc(sizeof(int));
    *arr[i] = 0x55555555;
  }
  return NULL;
}