#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/stat.h>
#include <errno.h>

#include "http_messages.h"

/*
//  * Handle a CGI request
//  */

// void handle_cgi_bin(const http_request *request, socket_t *sock) {
//   char request_array[2048] = { 0 };
  
//   strncpy(request_array, request->request_uri, strlen(request->request_uri));

//   char * script = strtok(request_array, "?");
//   char * vars = strtok(NULL, "?");

//   int pid = fork();

//   char * to_print = "HTTP/1.1 200 Document follows\r\nServer: Server-Type\r\n";
//   dprintf(sock->socket_fd, "%s", to_print);

//   if (pid == 0) {
//     setenv("REQUEST_METHOD", "GET", 1);
//     setenv("QUERY_STRING", vars, 1);
//     dup2(sock->socket_fd, STDIN_FILENO);
//     char * args[] = {script, NULL};
//     execv(script, args);
//   }
//   else {
//     wait(NULL);
//     close_socket(sock);
//   }
//   return;
// } /* handle_cgi_bin() */
