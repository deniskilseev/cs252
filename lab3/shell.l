
/*
 *
 * CS-252 
 * shell.l: lexical analyzer for shell
 * You have to extend it.
 *
 */

%{

#include "y.tab.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

static  void yyunput (int c,char *buf_ptr  );

void myunputc(int c) {
  unput(c);
}

%}

%option noyywrap

%%

\n {
  return NEWLINE;
}

[ \t] {
  /* Discard spaces and tabs */
}

">" {
  return GREATER;
}

"<" {
  return LESS;
}

"2>" {
  return TWOGREATER;
}

">&" {
  return GREATERAMPERSAND;
}

">>" {
  return GREATERGREATER;
}

">>&" {
  return GREATERGREATERAMPERSAND;
}

"&" {
  return AMPERSAND;
}

"|" {
  return PIPE;
}

\$\([^\t\n]+\) {
  // SUBSHELL
  yylval.string = strdup(yytext);

  char newstring[1024] = "";

  yylval.string = strncpy(newstring, yylval.string+2, strlen(yylval.string) - 3);
  char * exit_string = "\nexit\n";
  strcat(newstring, exit_string);

  int default_in = dup(0);
  int default_out = dup(1);

  int pipein[2];
  int pipeout[2];

  pipe(pipein);
  pipe(pipeout);

  dup2(pipein[0], 0);
  dup2(pipeout[1], 1);

  close(pipein[0]);
  close(pipeout[1]);

  write(pipein[1], newstring, strlen(newstring));
  close(pipein[1]);

  int pid = fork();

  if (pid == 0) {
    char ** args[2] = {"/proc/self/exe", NULL};
    execvp(args[0], args);
    perror("execvpsubshell error");
    exit(1);
  }
  if (pid < 0) {
    perror("fork error");
    exit(2);
  }

  waitpid(pid, 0, 0);
  dup2(default_in, 0);
  dup2(default_out, 1);

  close(default_in);
  close(default_out);
  
  char buffer[2048];
  int we_read = read(pipeout[0], buffer, 2048);
  write(default_out, buffer, strlen(buffer));
  close(pipeout[0]);
  if (we_read > 0) {
    for (int i = we_read-1; i >= 0; i--) {
      if (buffer[i] == '\n') {
        buffer[i] = '\t'; 
      }
      myunputc(buffer[i]);
    }
  }
}

\$\{[^\{\t\n]+\} {
  yylval.string = strdup(yytext);
  char * newstring = malloc(sizeof(newstring) * 1024);
  strncpy(newstring, yylval.string+2, strlen(yylval.string) - 3);
  if (strcmp(newstring, "$") == 0) {
    pid_t current_pid = getpid();
    snprintf(newstring, 1024, "%d", current_pid);
  }
  else if (strcmp(newstring, "SHELL") == 0) {
    char buf[1024];
    readlink("/proc/self/exe", buf, 1024);
    snprintf(newstring, "%s", buf);
  }
  else {
    newstring = getenv(newstring);
  }
  for (int i = strlen(newstring)-1; i >= 0; i--) {
    if (newstring[i] != ' ') {
        myunputc(newstring[i]);
    }
  }
}

[^ \$\t\n<>|&]*\\[^ \t\n]* {
  yylval.string = strdup(yytext);
  int idx = 0;
  char * new_value = malloc(sizeof(char) * 4096);
  int flag_append_next = 0;
  for (int i = 0; i < strlen(yylval.string); i++) {
    if (flag_append_next == 1) {
      new_value[idx] = yylval.string[i];
      flag_append_next = 0;
      idx++;
      continue;
    }
    if (yylval.string[i] == '\\') {
      flag_append_next = 1;
      continue;
    }
    new_value[idx] = yylval.string[i];
    idx++;
  } 
  new_value[idx] = '\0';
  yylval.string = new_value;
  return WORD;
}
["][^ \t\n]+["] {
  yylval.string = strdup(yytext);
  char new_value[2048] = "";
  strncpy(new_value, yylval.string + (size_t)1, (size_t)strlen(yylval.string) - (size_t)2);
  yylval.string = new_value;
  return WORD;
}

[^ \$\<\>\t\n]+ {
  yylval.string = strdup(yytext);
  return WORD;
}



