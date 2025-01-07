#include "part2.h"

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// The number of molecules of each type
// (O, N, O2, and N2)

// int g_num_oxygen = 0;
// int g_num_nitrogen = 0;
// int g_num_o2 = 0;
// int g_num_n2 = 0;

// Mutex locks for all elements
pthread_mutex_t g_oxygen_mutex;
pthread_mutex_t g_nitrogen_mutex;
pthread_mutex_t g_o2_mutex;
pthread_mutex_t g_n2_mutex;

// Semaphores for 
sem_t g_oxygen_created_sem;
sem_t g_nitrogen_created_sem;
sem_t g_o2_needed_sem;
sem_t g_o2_created_sem;
sem_t g_n2_needed_sem;
sem_t g_n2_created_sem;

/*
 * Create oxygen atoms. The number of atoms to create is specified by the
 * argument, which is an int cast to (void *).
 */

void *create_oxygen(void *ptr) {
  int how_many = *((int *) ptr);
  free(ptr);
  ptr = NULL;

  // Add your code to create how_many nitrogen atoms
  for (int i = 0; i < how_many; i++) {
    sem_post(&g_oxygen_created_sem);
    printf("An atom of oxygen was created.\n");
  }
  // Print this for each atom created:


  pthread_exit(0);
} /* create_oxygen() */

/*
 * Create nitrogen atoms. The number of atoms to create is specified by the
 * argument, which is an int cast to (void *).
 */

void *create_nitrogen(void *ptr) {
  int how_many = *((int *) ptr);
  free(ptr);
  ptr = NULL;

  for (int i = 0; i < how_many; i++) {
    sem_post(&g_nitrogen_created_sem);
    printf("An atom of nitrogen was created.\n");
  }
  // Print this for each atom created:

  pthread_exit(0);
} /* create_nitrogen() */

/*
 * Form N2 molecules, by combining nitrogen atoms.
 */

void *create_n2(void *ptr) {

  // Remove the following line when implementing your code

  (void) (ptr);

  while (1) {
    // sem_wait(&g_n2_needed_sem);

    for (int i = 0; i < 2; i++) {
      sem_wait(&g_nitrogen_created_sem);
    }
    // Add your code to wait for the proper signal.

    // Add your code to check if you can create an N2 molecule
    // and if so, adjust counts of N and N2

    printf("Two atoms of nitrogen combined to produce one molecule of N2.\n");

    // Add your code to signal consumers of N2
    sem_post(&g_n2_created_sem);
  }
} /* create_n2() */

/*
 * Form O2 molecules, by combining oxygen atoms.
 */

void *create_o2(void *ptr) {

  // Remove the following line when implementing your code

  (void) (ptr);

  while (1) {
    // sem_wait(&g_o2_needed_sem);

    for (int i = 0; i < 2; i++) {
      sem_wait(&g_oxygen_created_sem);
    }

    sem_post(&g_o2_created_sem);
    printf("Two atoms of oxygen combined to produce one molecule of O2.\n");
  }
} /* create_o2() */

/*
 * Form NO2 molecules, by combining N2 and O2 molecules.
 */

void *create_no2(void *ptr) {

  // Remove the following line when implementing your code

  (void) (ptr);

  while (1) {
    sem_wait(&g_n2_created_sem);
    sem_wait(&g_o2_created_sem);
    sem_wait(&g_o2_created_sem);

    printf("One molecule of N2 and two molecules of O2 combined to "
           "produce two molecules of NO2.\n");
  }
} /* create_no2() */

/*
 * Form O3 molecules, by combining O2 molecules.
 */

void *create_o3(void *ptr) {

  // Remove the following line when implementing your code

  (void) (ptr);

  while (1) {
    // Add your code to consume three O2 molecules and produce two O3 molecules
    sem_wait(&g_o2_needed_sem);
    sem_wait(&g_o2_needed_sem);
    sem_wait(&g_o2_needed_sem);
    printf("Three molecules of O2 combined to produce two molecules of O3.\n");
  }
} /* create_o3() */


/*
 * Create threads to run each chemical reaction. Wait on all threads, even
 * though many won't exit, to avoid any premature exit. The number of oxygen
 * atoms to be created is specified by the first command-line argument, and the
 * number of nitrogen atoms with the second.
 */

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Please pass two arguments.\n");
    exit(1);
  }

  pthread_mutex_init(&g_oxygen_mutex, NULL);
  pthread_mutex_init(&g_nitrogen_mutex, NULL);
  pthread_mutex_init(&g_o2_mutex, NULL);
  pthread_mutex_init(&g_n2_mutex, NULL);

  int * oxygen = (int *) malloc(sizeof(int));
  *oxygen = atoi(argv[1]);
  int * nitrogen = (int *) malloc(sizeof(int));
  *nitrogen = atoi(argv[2]);

  sem_init(&g_oxygen_created_sem, 0, 0);
  sem_init(&g_nitrogen_created_sem, 0, 0);
  sem_init(&g_o2_needed_sem, 0, 0);
  sem_init(&g_o2_created_sem, 0, 0);
  sem_init(&g_n2_needed_sem, 0, 0);
  sem_init(&g_n2_created_sem, 0, 0);

  pthread_t oxygen_thread;
  pthread_t nitrogen_thread;
  pthread_t o2_thread;
  pthread_t n2_thread;
  pthread_t no2_thread;
  pthread_t o3_thread;

  int *intPtr;

  intPtr = (int *)malloc(sizeof(int));
  intPtr = oxygen;
  pthread_create(&oxygen_thread, NULL, create_oxygen, (void *) intPtr);
  
  intPtr = (int *)malloc(sizeof(int));
  intPtr = nitrogen;
  pthread_create(&nitrogen_thread, NULL, create_nitrogen, (void *) intPtr);

  intPtr = (int *)malloc(sizeof(int));
  *intPtr = -1;
  pthread_create(&n2_thread, NULL, create_n2, (void *) intPtr);

  intPtr = (int *)malloc(sizeof(int));
  *intPtr = -1;
  pthread_create(&o2_thread, NULL, create_o2, (void *) intPtr);

  intPtr = (int *)malloc(sizeof(int));
  *intPtr = -1;
  pthread_create(&no2_thread, NULL, create_no2, (void *) intPtr);

  intPtr = (int *)malloc(sizeof(int));
  *intPtr = -1;
  pthread_create(&o3_thread, NULL, create_o3, (void *) intPtr);

  pthread_join(oxygen_thread, NULL);
  pthread_join(nitrogen_thread, NULL);
  sleep(5);
  // pthread_join(n2_thread, NULL);
  // pthread_join(o2_thread, NULL);
  // pthread_join(no2_thread, NULL);
  // pthread_join(o3_thread, NULL);

  // Add your code to create the threads.  Remember to allocate and pass the
  // arguments for create_oxygen and create_nitrogen.

  // Add your code to wait till threads are complete before main
  // continues. Unless we wait we run the risk of executing an exit which will
  // terminate the process and all threads before the threads have completed.

  exit(EXIT_SUCCESS);
}
