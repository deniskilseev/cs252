#include "my_malloc.h"

#include <assert.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "printing.h"

/* Pointer to the location of the heap prior to any sbrk calls */
void *g_base = NULL;

/* Pointer to the end of our heap */
void *g_heap_end = NULL;

/* Pointer to the head of the free list */
header *g_freelists[NUM_LISTS] = { NULL };

/* Mutex to ensure thread safety for the freelist */
static pthread_mutex_t g_mutex = { 0 };

/*
 * Pointer to the second fencepost in the most recently allocated chunk from
 * the OS. Used for coalescing chunks
 */

header *g_last_left_fence_post = NULL;
header *g_last_right_fence_post = NULL;

static size_t g_arena_size = ARENA_SIZE;

/*
 * Direct the compiler to run the init function before running main
 * this allows initialization of required globals
 */

static void init(void) __attribute__((constructor));

/*
 * Insert a block at the beginning of the freelist.
 * The block is located after its left header, h.
 */

static void insert_free_block(header *h) {
  h->prev = NULL;
  h->next = g_freelists[ORDER(h)];
  if (g_freelists[ORDER(h)] != NULL) {
    g_freelists[ORDER(h)]->prev = h;
  }
  g_freelists[ORDER(h)] = h;
} /* insert_free_block() */

/*
 * Remove a block, h, from the free list it is in.
 */

static void remove_free_block(header *h) {
  if (h->prev != NULL) {
    h->prev->next = h->next;
  }
  if (h->next != NULL) {
    h->next->prev = h->prev;
  }
  if (g_freelists[ORDER(h)] == h) {
    g_freelists[ORDER(h)] = h->next;
  }
  h->next = NULL;
  h->prev = NULL;
} /* remove_free_block() */

/*
 * Instantiates fenceposts at the left and right side of a block.
 */

static void set_fenceposts(void *mem, size_t size) {
  header *left_fence = (header *) mem;
  header *right_fence = (header *) (((char *) mem) + size - ALLOC_HEADER_SIZE);

  SET_SIZE(left_fence, size - (2 * ALLOC_HEADER_SIZE));
  SET_SIZE(right_fence, ALLOC_HEADER_SIZE);

  SET_STATUS(left_fence, FENCEPOST);
  SET_STATUS(right_fence, FENCEPOST);

  left_fence->chunk = (header *) (((char *) left_fence) + ALLOC_HEADER_SIZE);
} /* set_fenceposts() */

/*
 * Constructor that runs before main() to initialize the library.
 */

__attribute__((constructor))void init() {
  /* Initialize all the free lists */
  for (int i = 0; i < NUM_LISTS; i++) {
    g_freelists[i] = NULL;
  }

  /* Initialize mutex for thread safety */

  pthread_mutex_init(&g_mutex, NULL);

  /* Manually set printf buffer so it won't call malloc */

  setvbuf(stdout, NULL, _IONBF, 0);

  /* Record the starting address of the heap */

  g_base = sbrk(0);

} /* init() */

/*
 * Function that takes a pointer to
 * a block and returns pointer to block's buddy.
 */

static inline header *find_buddy(header *h) {
  header * chunk_header = h->chunk;
  size_t offset = (char *) h - (char *) chunk_header;
  return (header *) ((offset ^ TRUE_SIZE(h)) + (char *) chunk_header);
} /* find_buddy() */

/*
 * Allocates memory on heap and return pointer to data field.
 */

void * my_malloc(size_t size) {

  pthread_mutex_lock(&g_mutex);

  if (size == 0) {
    pthread_mutex_unlock(&g_mutex);
    return NULL;
  }

  int size_to_allocate = size + ALLOC_HEADER_SIZE;

 /*
  * Checks what order needs to be allocated
  */
  int needed_order = (int)LOG_2(size_to_allocate);

 /*
  * Branching that executes when size is not a direct log_2.
  */
  if (size_to_allocate != 1 << needed_order) {
    needed_order += 1;
  }

  void * return_pointer = NULL;

 /*
  * Branching to check whether there's a block of needed size.
  * If so, return pointer to that block.
  * If not, check whether possible to split anything.
  * If nothing to split, allocate and check again.
  */

  if (g_freelists[needed_order] != NULL) {
    header * return_block = g_freelists[needed_order];
    SET_STATUS(return_block, ALLOCATED);
    remove_free_block(return_block);
    return_pointer = (void *) ((char *) return_block +
                               (size_t) ALLOC_HEADER_SIZE);
  }
  else {

    int found_appropriate = 0;
    int order_appropriate = -1;

    header *block = NULL;

    size_t block_size = 0;

    while (found_appropriate != 1) {

      for (int order = needed_order; order < NUM_LISTS; order++) {
        if (g_freelists[order] != NULL) {
          found_appropriate = 1;
          order_appropriate = order;
          break;
        }
      }

      if (found_appropriate == 1) {
        block = g_freelists[order_appropriate];
      }
      else {
        header * before_call_last_left_fence_post = g_last_left_fence_post;

        int size_allocated = 0;

        if (g_last_left_fence_post != NULL) {
          g_last_left_fence_post = sbrk(
            TRUE_SIZE(before_call_last_left_fence_post) + 2 * ALLOC_HEADER_SIZE
          );
          size_allocated = (TRUE_SIZE(before_call_last_left_fence_post));
        }
        else {
          g_last_left_fence_post = sbrk(g_arena_size + 2 * ALLOC_HEADER_SIZE);
          size_allocated = g_arena_size;
        }
        if (g_last_left_fence_post == (void *) -1) {
          g_last_left_fence_post = before_call_last_left_fence_post;
          errno = ENOMEM;
          pthread_mutex_unlock(&g_mutex);
          return NULL;
        }

        if (g_heap_end + 1 == g_last_left_fence_post) {
          g_heap_end = sbrk(0) - 1;
          size_t new_size = size_allocated * 2;
          g_last_left_fence_post = before_call_last_left_fence_post;
          char * glf = (char *) g_last_left_fence_post;
          g_last_right_fence_post = (header *)( glf +
                                                new_size +
                                                ALLOC_HEADER_SIZE);

          set_fenceposts(g_last_left_fence_post,
                         new_size + 2 * ALLOC_HEADER_SIZE);

          SET_SIZE(g_last_left_fence_post, new_size);

          block = (header *) ((char *) g_last_right_fence_post -
                              (size_t) size_allocated);

          g_last_right_fence_post->chunk = g_last_left_fence_post->chunk;
          block->chunk = g_last_left_fence_post->chunk;
        }
        else {
          g_heap_end = sbrk(0) - 1;
          set_fenceposts(g_last_left_fence_post,
                         size_allocated + 2 * ALLOC_HEADER_SIZE);
          char * glf = (char *)g_last_left_fence_post;
          g_last_right_fence_post = (header *) ( glf +
                                                 (size_t) size_allocated +
                                                 ALLOC_HEADER_SIZE);
          block = (header *) ((char *) g_last_left_fence_post +
                              (size_t) ALLOC_HEADER_SIZE);
          g_last_right_fence_post->chunk = block;
          block->chunk = block;
        }

        SET_STATUS(block, UNALLOCATED);
        SET_SIZE(block, size_allocated);
        header * buddy = find_buddy(block);

        while ((TRUE_SIZE(block) == TRUE_SIZE(buddy)) &&
               (STATUS(buddy) == UNALLOCATED)) {

          remove_free_block(buddy);
          if ((char *) buddy < (char *) block) {
            header * temp = block;
            block = buddy;
            buddy = temp;
          }
          SET_SIZE(block, TRUE_SIZE(block) << 1);
          SET_STATUS(block, UNALLOCATED);
          buddy = find_buddy(block);
        }

        insert_free_block(block);
      }
    }

   /*
    * int to check whether the allocated chunk was removed as the free block.
    */
    int were_splitting = 0;

    block_size = TRUE_SIZE(block);

   /*
    * Split block until needed size is reached.
    */

    while ((TRUE_SIZE(block) > MIN_SIZE) &&
           (TRUE_SIZE(block) >> 1 >= 1 << needed_order)) {
      were_splitting = 1;
      remove_free_block(block);

      block_size = block_size / 2;
      SET_SIZE(block, block_size);

      header *buddy = find_buddy(block);
      SET_SIZE(buddy, block_size);
      SET_STATUS(buddy, UNALLOCATED);
      buddy->chunk = block->chunk;
      insert_free_block(buddy);
    }

    if (were_splitting == 0) {
      remove_free_block(block);
    }

    SET_STATUS(block, ALLOCATED);
    return_pointer = (void *) ((char *) block + (size_t)ALLOC_HEADER_SIZE);
  }

  pthread_mutex_unlock(&g_mutex);
  return return_pointer;
} /* my_malloc() */

/*
 * Frees block that was allocated by my_malloc.
 */

void my_free(void * p) {

  pthread_mutex_lock(&g_mutex);

  if (p == NULL) {
    pthread_mutex_unlock(&g_mutex);
    return;
  }

  header * block = (header *)((char *) p - ALLOC_HEADER_SIZE);

  if (STATUS(block) == UNALLOCATED) {
    assert(false);
    pthread_mutex_unlock(&g_mutex);
    return;
  }

  SET_STATUS(block, UNALLOCATED);

  header * buddy = find_buddy(block);

  while ((TRUE_SIZE(block) == TRUE_SIZE(buddy)) &&
          (STATUS(buddy) == UNALLOCATED)) {
    remove_free_block(buddy);
    if ((char *) buddy < (char *) block) {
      header * temp = block;
      block = buddy;
      buddy = temp;
    }
    SET_SIZE(block, TRUE_SIZE(block) << 1);
    SET_STATUS(block, UNALLOCATED);
    buddy = find_buddy(block);
  }
  insert_free_block(block);
  pthread_mutex_unlock(&g_mutex);
  return;
} /* my_free() */

/*
 * Calls malloc and sets each byte of
 * the allocated memory to a value
 */

void *my_calloc(size_t nmemb, size_t size) {
  return memset(my_malloc(size * nmemb), 0, size * nmemb);
} /* my_calloc() */

/*
 * Reallocates an allocated block to a new size and
 * copies the contents to the new block.
 */

void *my_realloc(void *ptr, size_t size) {
  void *mem = my_malloc(size);
  memcpy(mem, ptr, size);
  my_free(ptr);
  return mem;
} /* my_realloc() */