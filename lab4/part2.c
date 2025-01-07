#include "part2.h"

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

// Semaphores for
sem_t g_oxygen_created_sem = {0};
sem_t g_nitrogen_created_sem = {0};
sem_t g_o2_needed_sem = {0};
sem_t g_o2_created_sem = {0};
sem_t g_n2_needed_sem = {0};
sem_t g_n2_created_sem = {0};

/*
 * Create oxygen atoms. The number of atoms to create is specified by the
 * argument, which is an int cast to (void *).
 */

void *create_oxygen(void *ptr) {
  int how_many = *((int *) ptr);
  free(ptr);
  ptr = NULL;

  for (int i = 0; i < how_many; i++) {
    sem_post(&g_oxygen_created_sem);
    printf("An atom of oxygen was created.\n");
  }

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

  pthread_exit(0);
} /* create_nitrogen() */

/*
 * Form N2 molecules, by combining nitrogen atoms.
 */

void *create_n2(void *ptr) {

  free(ptr);
  ptr = NULL;

  while (1) {

    for (int i = 0; i < 2; i++) {
      sem_wait(&g_nitrogen_created_sem);
    }

    printf("Two atoms of nitrogen combined to produce one molecule of N2.\n");
    sem_post(&g_n2_created_sem);

  }
} /* create_n2() */

/*
 * Form O2 molecules, by combining oxygen atoms.
 */

void *create_o2(void *ptr) {

  free(ptr);
  ptr = NULL;

  while (1) {

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

  free(ptr);
  ptr = NULL;

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

  free(ptr);
  ptr = NULL;

  while (1) {
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

  int oxygen = 0;
  oxygen = atoi(argv[1]);
  int nitrogen = 0;
  nitrogen = atoi(argv[2]);

  sem_init(&g_oxygen_created_sem, 0, 0);
  sem_init(&g_nitrogen_created_sem, 0, 0);
  sem_init(&g_o2_needed_sem, 0, 0);
  sem_init(&g_o2_created_sem, 0, 0);
  sem_init(&g_n2_needed_sem, 0, 0);
  sem_init(&g_n2_created_sem, 0, 0);

  pthread_t oxygen_thread = {0};
  pthread_t nitrogen_thread = {0};
  pthread_t o2_thread = {0};
  pthread_t n2_thread = {0};
  pthread_t no2_thread = {0};
  pthread_t o3_thread = {0};

  int * int_ptr = (int *)malloc(sizeof(int));
  *int_ptr = oxygen;
  pthread_create(&oxygen_thread, NULL, create_oxygen, (void *) int_ptr);

  int_ptr = (int *)malloc(sizeof(int));
  *int_ptr = nitrogen;
  pthread_create(&nitrogen_thread, NULL, create_nitrogen, (void *) int_ptr);

  int_ptr = (int *)malloc(sizeof(int));
  *int_ptr = -1;
  pthread_create(&n2_thread, NULL, create_n2, (void *) int_ptr);

  int_ptr = (int *)malloc(sizeof(int));
  *int_ptr = -1;
  pthread_create(&o2_thread, NULL, create_o2, (void *) int_ptr);

  int_ptr = (int *)malloc(sizeof(int));
  *int_ptr = -1;
  pthread_create(&no2_thread, NULL, create_no2, (void *) int_ptr);

  int_ptr = (int *)malloc(sizeof(int));
  *int_ptr = -1;
  pthread_create(&o3_thread, NULL, create_o3, (void *) int_ptr);

  pthread_join(oxygen_thread, NULL);
  pthread_join(nitrogen_thread, NULL);
  sleep(5);

  exit(EXIT_SUCCESS);
} /* main() */