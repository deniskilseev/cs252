#ifndef COMMAND_H
#define COMMAND_H

#include <stdbool.h>

#include "single_command.h"

// Command Data Structure

typedef struct command {
  char *out_file;
  char *in_file;
  char *err_file;
  bool append_out;
  bool append_err;
  bool background;

  bool ambigious;

  single_command_t **single_commands;
  int num_single_commands;
} command_t;

void create_command(command_t *);
void insert_single_command(command_t *, single_command_t *);
void free_command(command_t *);
void print_command(command_t *);
void execute_command(command_t *);

extern command_t *g_current_command;

#endif
