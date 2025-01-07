
/*
 * CS-252
 * shell.y: parser for shell
 *
 * This parser compiles the following grammar:
 *
 *	cmd [arg]* [> filename]
 *
 * you must extend it to understand the complete shell grammar
 *
 */

%code requires 
{

}

%union
{
  char * string;
}

%token <string> WORD PIPE WORDQUOTE
%token NOTOKEN NEWLINE GREATER LESS TWOGREATER GREATERAMPERSAND GREATERGREATER GREATERGREATERAMPERSAND AMPERSAND

%{

#include <stdbool.h> 
#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <regex.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#include "command.h"
#include "single_command.h"
#include "shell.h"

void yyerror(const char * s);
int yylex();

int g_arguments_capacity = 256;
int g_arguments_inserted = 0;
char ** g_new_arguments = NULL;

%}

%%

goal:
  entire_command_list
  ;

entire_command_list:
     entire_command_list entire_command
  |  entire_command
  ;

entire_command:
     single_command_list io_modifier_list NEWLINE {
      execute_command(g_current_command);
      create_command(g_current_command);
     }
  |  NEWLINE    
  ;

single_command_list:
     single_command_list PIPE single_command
  |  single_command {
  }
  ;

single_command:
    executable argument_list {
      insert_single_command(g_current_command, g_current_single_command);
      g_current_single_command =
        (single_command_t *)malloc(sizeof(single_command_t));
    }
  ;

argument_list:
     argument_list argument 
  | /* can be empty */
  ;

argument:
     WORD {
      parent_expand_wildcard($1);
     }
  ;

executable:
     WORD {
      create_single_command(g_current_single_command);
      insert_argument(g_current_single_command, $1);
     }
  ;

io_modifier_list:
     io_modifier_list io_modifier
  |  /* can be empty */   
  ;

io_modifier:
     GREATER WORD {
      if (g_current_command->out_file != NULL) {
        printf("Ambiguous output redirect.\n");
        exit(1);
      }
      g_current_command->out_file = $2;
     }
     | LESS WORD {
      if (g_current_command->in_file != NULL) {
        printf("Ambiguous output redirect.\n");
        exit(1);
      }
      g_current_command->in_file = $2;
     }
     | TWOGREATER WORD {
      if (g_current_command->err_file != NULL) {
        printf("Ambiguous output redirect.\n");
        exit(1);
      }
      g_current_command->err_file = $2;
     }
     | GREATERAMPERSAND WORD {
      if (g_current_command->out_file != NULL ||
          g_current_command->err_file != NULL) {
        printf("Ambiguous output redirect.\n");
        exit(1);
      }
      g_current_command->out_file = $2;
      g_current_command->err_file = $2;
     }
     | GREATERGREATER WORD {
      if (g_current_command->out_file != NULL) {
        printf("Ambiguous output redirect.\n");
        exit(1);
      }
      g_current_command->out_file = $2;
      g_current_command->append_out = true;
     }
     | GREATERGREATERAMPERSAND WORD {
      if (g_current_command->out_file != NULL ||
          g_current_command->err_file != NULL) {
        printf("Ambiguous output redirect.\n");
        exit(1);
      }
      g_current_command->out_file = $2;
      g_current_command->err_file = $2;
      g_current_command->append_out = true;
      g_current_command->append_err = true;
     }
     | AMPERSAND {
      g_current_command->background = true;
     } 
  ;

%%

void
yyerror(const char * s)
{
  fprintf(stderr,"%s", s);
}

int compare(const void * file1, const void * file2) {
  return strcmp(*(const char **)file1, *(const char **) file2);
}

void parent_expand_wildcard(char * arg) {
  g_arguments_capacity = 256;
  g_arguments_inserted = 0;
  g_new_arguments = (char **) malloc(g_arguments_capacity * sizeof(char *));

  expand_wildcards(NULL, arg);

  qsort(g_new_arguments, g_arguments_inserted, sizeof(char *), compare);
  for (int i = 0; i < g_arguments_inserted; i++) {
    insert_argument(g_current_single_command, strdup(g_new_arguments[i]));
    free(g_new_arguments[i]);
    g_new_arguments[i] = NULL;
  }
  free(g_new_arguments);
  return;
}


void expand_wildcards(char * prefix, char *suffix) {

  if (suffix[0] == 0) { // Add if no suffix
    if (g_arguments_inserted == g_arguments_capacity) {
      g_arguments_capacity *= 2;
      g_new_arguments = realloc(g_new_arguments, g_arguments_capacity * sizeof(char *));            
    }
    g_new_arguments[g_arguments_inserted++] = strdup(prefix);
    return;
  } 
  if (prefix == NULL && suffix[0] == '/') {
    prefix = strdup("/");
  }
  char new_prefix[1028];
  char * component = malloc(sizeof(char) * 1028);
  char *s = strchr(suffix, '/');

  if (s != NULL) {
    strncpy(component, suffix, s-suffix);
    component[s-suffix] = '\0';
    suffix = s + 1;
  }
  else {
    strcpy(component, suffix);
    suffix = suffix + strlen(suffix);
  }
  if (strchr(component, '*') || strchr(component, '?')) {
    char *regex = (char *) malloc(2 * strlen(component) + 3);
    char *arg_pos = component;
    char *regex_pos = regex;
    *regex_pos++ = '^';
    while (*arg_pos) {
      if (*arg_pos == '*') {
        *regex_pos++ = '.';
        *regex_pos++ = '*';
      }
      else if (*arg_pos == '?') {
        *regex_pos++ = '.'; 
      }
      else if (*arg_pos == '.') {
        *regex_pos++ = '\\';
        *regex_pos++ = '.';
      }
      else {
        *regex_pos++ = *arg_pos;
      }
      arg_pos++;
    }
    *regex_pos++ = '$';
    *regex_pos = '\0'; // match end of line and add null char

    regex_t re;
    int status = regcomp(&re, regex, REG_EXTENDED|REG_NOSUB);
    if (status != 0) {
      perror("compile");
      return;
    }
    char * directory = NULL;
    if (prefix == NULL) {
      directory = strdup(".");
    }
    else {
      if (prefix[0] != '/') {
        char * new_string = malloc(sizeof(char) * 1024);
        new_string = strdup("/");
        directory = strcat(new_string, prefix);
      }
      else {
        directory = prefix;
      }
    }
    DIR *dir = opendir(directory);
    if (dir == NULL) {
      perror("opendir");
      return;
    }

    struct dirent *ent;
    regmatch_t match;

    while ((ent = readdir(dir)) != NULL) {
      if (!regexec(&re, ent->d_name, 1, &match, 0)) { 
        if (ent->d_type == DT_DIR) { // If directory and suffix exists, explore further
          if (suffix != NULL) {
            if (ent->d_name[0] == '.' && component[0] == '.' ||
                ent->d_name[0] != '.' && component[0] != '.') {
                if (prefix && strcmp(prefix, "/") != 0) {
                  sprintf(new_prefix, "%s/%s", prefix, ent->d_name);
                }
                else {
                  sprintf(new_prefix, "%s", ent->d_name);
                }
              expand_wildcards(new_prefix, suffix);
            }
          }
        }
        else {
          if (suffix[0] == 0) { // If file, and suffix is 0, send down to be added.
              if (ent->d_name[0] == '.' && component[0] == '.' ||
                  ent->d_name[0] != '.' && component[0] != '.') {
                if (prefix && strcmp(prefix, "/") != 0) {
                  sprintf(new_prefix, "%s/%s", prefix, ent->d_name);
                }
                else {
                  sprintf(new_prefix, "%s", ent->d_name);
                }
              expand_wildcards(new_prefix, suffix);
            }
          }
        }
      }
    }
    closedir(dir);
  }
  else {
    if (prefix && strcmp(prefix, "/") != 0) {
      sprintf(new_prefix, "%s/%s", prefix, component);
    }
    else {
      sprintf(new_prefix, "%s", component);
    }
    expand_wildcards(new_prefix, suffix);
  }
  return;
}

#if 0
main()
{
  yyparse();
}
#endif
