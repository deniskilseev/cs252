/*
 * CS252: Shell project
 *
 * Template file.
 * You will need to add more code here to execute the command table.
 *
 * NOTE: You are responsible for fixing any bugs this code may have!
 *
 * DO NOT PUT THIS PROJECT IN A PUBLIC REPOSITORY LIKE GIT. IF YOU WANT
 * TO MAKE IT PUBLICALLY AVAILABLE YOU NEED TO REMOVE ANY SKELETON CODE
 * AND REWRITE YOUR PROJECT SO IT IMPLEMENTS FUNCTIONALITY DIFFERENT THAN
 * WHAT IS SPECIFIED IN THE HANDOUT. WE OFTEN REUSE PART OF THE PROJECTS FROM
 * SEMESTER TO SEMESTER AND PUTTING YOUR CODE IN A PUBLIC REPOSITORY
 * MAY FACILITATE ACADEMIC DISHONESTY.
 */

#include "command.h"
#include "single_command.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "shell.h"

/*
 *  Initialize a command_t
 */

void create_command(command_t *command) {
  command->single_commands = NULL;

  command->out_file = NULL;
  command->in_file = NULL;
  command->err_file = NULL;

  command->append_out = false;
  command->append_err = false;
  command->background = false;

  command->num_single_commands = 0;
} /* create_command() */

/*
 *  Insert a single command into the list of single commands in a command_t
 */

void insert_single_command(command_t *command, single_command_t *simp) {
  if (simp == NULL) {
    return;
  }

  command->num_single_commands++;
  int new_size = command->num_single_commands * sizeof(single_command_t *);
  command->single_commands = (single_command_t **)
                              realloc(command->single_commands,
                                      new_size);
  command->single_commands[command->num_single_commands - 1] = simp;
} /* insert_single_command() */

/*
 *  Free a command and its contents
 */

void free_command(command_t *command) {
  for (int i = 0; i < command->num_single_commands; i++) {
    free_single_command(command->single_commands[i]);
  }

  if (command->out_file) {
    free(command->out_file);
    command->out_file = NULL;
  }

  if (command->in_file) {
    free(command->in_file);
    command->in_file = NULL;
  }

  if (command->err_file) {
    free(command->err_file);
    command->err_file = NULL;
  }

  command->append_out = false;
  command->append_err = false;
  command->background = false;

  free(command);
} /* free_command() */

/*
 *  Print the contents of the command in a pretty way
 */

void print_command(command_t *command) {
  printf("\n\n");
  printf("              COMMAND TABLE                \n");
  printf("\n");
  printf("  #   single Commands\n");
  printf("  --- ----------------------------------------------------------\n");

  // iterate over the single commands and print them nicely
  for (int i = 0; i < command->num_single_commands; i++) {
    printf("  %-3d ", i );
    print_single_command(command->single_commands[i]);
  }

  printf( "\n\n" );
  printf( "  Output       Input        Error        Background\n" );
  printf( "  ------------ ------------ ------------ ------------\n" );
  printf( "  %-12s %-12s %-12s %-12s\n",
            command->out_file?command->out_file:"default",
            command->in_file?command->in_file:"default",
            command->err_file?command->err_file:"default",
            command->background?"YES":"NO");
  printf( "\n\n" );
} /* print_command() */

/*
 *  Execute a command
 */

void execute_command(command_t *command) {
  // Don't do anything if there are no single commands
  if (command->single_commands == NULL) {
    print_prompt();
    return;
  }

  // Print contents of Command data structure
  // print_command(command);

  // Save default I/O
  int default_in = dup(0);
  int default_out = dup(1);
  int default_err = dup(2);

  char * write_file_name = NULL;
  char * read_file_name = NULL;
  char * err_file_name = NULL;

  int ret;
  int fdout;
  int fdin;
  int fderr;

  if (command->out_file != NULL) {
    write_file_name = command->out_file;
  }
  if (command->in_file != NULL) {
    read_file_name = command->in_file;
    fdin = open(read_file_name, O_RDONLY);
  }
  else {
    fdin = dup(default_in);
  }
  
  if (command->err_file != NULL) {
    err_file_name = command->err_file;
    if (command->append_err) {
      fderr = open(err_file_name, O_WRONLY | O_CREAT | O_APPEND, 0666);
    }
    else {
      fderr = open(err_file_name, O_WRONLY | O_CREAT, 0666);
    }
    dup2(fderr, 2);
  }
  else {
    fderr = dup(default_err);
  }
  
  for (int i = 0; i < command->num_single_commands; i++) {
    dup2(fdin, 0);
    close(fdin);
    if (strcmp(command->single_commands[i]->arguments[0], "exit") == 0) {
      exit(0);
    }
    if (i == command->num_single_commands - 1) {
      if (write_file_name != NULL) {
        if (command->append_out) {
          fdout = open(write_file_name, O_WRONLY | O_CREAT | O_APPEND, 0666);
        }
        else {
          fdout = creat(write_file_name, 0666);
        }
      }
      else {
        fdout = dup(default_out);
      }
    }
    else {
      int fdpipe[2];
			pipe(fdpipe);
			fdout = fdpipe[1];
			fdin = fdpipe[0];
    }

    dup2(fdout, 1);
    close(fdout);

    if (strcmp(command->single_commands[i]->arguments[0], "setenv") == 0) {
      char * argument_one = command->single_commands[i]->arguments[1];
      char * argument_two = command->single_commands[i]->arguments[2];
      setenv(argument_one, argument_two, 1);
      continue;
    }
    else if (strcmp(command->single_commands[i]->arguments[0], "unsetenv") == 0) {
      char * argument_one = command->single_commands[i]->arguments[1];
      unsetenv(argument_one);
      continue;
    }
    else if (strcmp(command->single_commands[i]->arguments[0], "cd") == 0) {
      if (command->single_commands[i]->num_args == 1) {
        chdir(getenv("HOME"));
      }
      else {
        char * argument_one = command->single_commands[i]->arguments[1];
        int change = chdir(argument_one);
        if (change != 0) {
          dprintf(fderr, "cd: can't cd to %s", argument_one);
        };
      }
      break;
    }
    
    ret = fork();
    if (ret == 0) {
      if (strcmp(command->single_commands[i]->arguments[0], "printenv") == 0 ) {
        extern char ** environ;
        int idx = 0;
        while (environ[idx] != NULL) {
          printf("%s\n", environ[idx]);
          idx++;
        }
        exit(0);
      }
      else {
        insert_argument(command->single_commands[i], NULL);
        execvp(command->single_commands[i]->arguments[0],
              command->single_commands[i]->arguments);
        perror("execvp");
        exit(1);
      }
    }
    else if (ret < 0) {
      perror("fork");
      exit(2);
    }
  }

  close(fderr);

  dup2(default_in, 0);
  dup2(default_out, 1);
  dup2(default_err, 2);

  close(default_in);
  close(default_out);
  close(default_err);
  
  if (!command->background) {
    waitpid(ret, 0, 0);
  }
  if (isatty(0)) { 
    print_prompt();
  }
} /* execute_command() */