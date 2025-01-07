#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "command.h"
#include "single_command.h"

command_t *g_current_command = NULL;
single_command_t *g_current_single_command = NULL;

int yyparse(void);

/*
 *  Prints shell prompt
 */

void print_prompt() {
  if (isatty(0)) {
    printf("myshell>");
    fflush(stdout);
  }
} /* print_prompt() */

/*
 *  Display signal number which was called
 */

extern void disp() {
  printf("\n");
  print_prompt();
} /* disp() */

/*
 *  Signal handler for zombie processes.
 */

extern void zombie(int signum) {
  (void) signum;
  pid_t child_pid = -1;
  while ((child_pid = waitpid(-1, NULL, WNOHANG)) > 0) {
    // printf("%d exited.\n", child_pid);
  }
} /* zombie() */

/*
 *  This main is simply an entry point for the program which sets up
 *  memory for the rest of the program and the turns control over to
 *  yyparse and never returns
 */

int main() {

  struct sigaction sa = {0};
  sa.sa_handler = disp;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGINT, &sa, NULL)) {
    perror("sigaction");
    exit(2);
  }

  struct sigaction sa_zombie = {0};
  sa_zombie.sa_handler = zombie;
  sigemptyset(&sa_zombie.sa_mask);
  sa_zombie.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa_zombie, NULL)) {
    perror("sigaction");
    exit(2);
  }

  g_current_command = (command_t *)malloc(sizeof(command_t));
  g_current_single_command =
        (single_command_t *)malloc(sizeof(single_command_t));

  create_command(g_current_command);
  create_single_command(g_current_single_command);

  print_prompt();
  yyparse();
} /* main() */
