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

char g_user_pass[MAX_USR_PWD_LEN];
int g_is_a_cgi = 0;

/*
 * Return a string in a format <user>:<password>
 * either from auth.txt or from your implememtation.
 */

char *return_user_pwd_string(void) {
  // Read from ./auth.txt. Don't change this. We will use it for testing
  FILE *fp = NULL;
  char *line = (char *)malloc(sizeof(char) * MAX_USR_PWD_LEN);
  size_t len = 0;

  fp = fopen("./auth.txt", "r");
  if (fp == NULL) {
    perror("couldn't read auth.txt");
    exit(-1);
  }

  if (getline(&line, &len, fp) == -1) {
    perror("couldn't read auth.txt");
    exit(-1);
  }

  sprintf(g_user_pass, "%s", line);

  free(line);

  return g_user_pass;
} /* return_user_pwd_string() */

/*
 * Accept connections one at a time and handle them.
 */

void run_linear_server(acceptor *accept) {
  while (1) {
    socket_t *sock = accept_connection(accept);
    handle(sock);
  }
} /* run_linear_server() */

/*
 * Accept connections, creating a different child process to handle each one.
 */

void run_forking_server(acceptor *accept) {

  while (1) {
    socket_t *sock = accept_connection(accept);
    int pid = fork();
    if (pid == 0) {
      handle(sock);
      exit(0);
    }
    close_socket(sock);
  }
} /* run_forking_server() */

/*
 * Accept connections, creating a new thread to handle each one.
 */

void run_threaded_server(acceptor *accept) {
  // TODO: Add your code to accept and handle connections in new threads
} /* run_threaded_server() */

/*
 * Accept connections, drawing from a thread pool with num_threads to handle the
 * connections.
 */

void run_thread_pool_server(acceptor *accept, int num_threads) {
  // TODO: Add your code to accept and handle connections in threads from a
  // thread pool
} /* run_thread_pool_server() */


/*
 * Handle a CGI request
 */

void handle_cgi_bin(const http_request *request, socket_t *sock) {
  char request_array[2048] = { 0 };

  strncpy(request_array, request->request_uri, strlen(request->request_uri));

  char * script = strtok(request_array, "?");
  char * vars = strtok(NULL, "?");
  // script = "http-root-dir/cgi-bin/calendar";


  int pid = fork();
  if (pid == 0) {

    if (vars != NULL) {
      setenv("QUERY_STRING", vars, 1);
    }
    // else {
    //   setenv("QUERY_STRING", "", 1);
    // }
    setenv("REQUEST_METHOD", "GET", 1);

    dup2(sock->socket_fd, STDOUT_FILENO);
    dup2(sock->socket_fd, STDERR_FILENO);

    char * args[] = {script, NULL};
    execv(script, args);
    exit(1);
  }
  else if (pid > 0) {
    char * string =  "HTTP/1.1 200 Document follows\r\nServer: Server-Type\r\n";
    send(sock->socket_fd, string, strlen(string), 0);
    waitpid(pid, NULL, 0);
    close_socket(sock);
  }
  else {
    perror("fork");
  }
  return;
} /* handle_cgi_bin() */


/*
 * Parse request text into request structure
 */

void parse_request(char * request_text, http_request *request) {
  char *parsed_headers[128] = { NULL };
  char text_array[4096];
  fflush(stdout);
  strncpy(text_array, request_text, strlen(request_text));
  text_array[strlen(request_text)] = '\0';

  char * method = malloc(256);
  strcpy(method, strtok(text_array, " "));
  request->method = method;
  fflush(stdout);
  char * uri = malloc(4096);
  strcpy(uri, strtok(NULL, " "));
  request->request_uri = uri;
  fflush(stdout);
  char * http_version = malloc(256);
  fflush(stdout);
  strcpy(http_version, strtok(NULL, "\r\n"));
  request->http_version = http_version;

  int num_lines = 0;
  char *line = strtok(NULL, "\r\n");

  while (line != NULL) {
    parsed_headers[num_lines++] = line;
    line = strtok(NULL, "\r\n");
  }
  fflush(stdout);

  request->headers = malloc(sizeof(header) * num_lines);

  request->num_headers = num_lines;
  request->query = "";
  request->message_body = "";

  char * key = NULL;
  char * value = NULL;
  char * key_buffer = NULL;
  char * value_buffer = NULL;
  fflush(stdout);
  for (int i = 0; i < num_lines; i++) {
    key_buffer = strtok(parsed_headers[i], ":");
    value_buffer = strtok(NULL, ":");
    if (value_buffer != NULL) {
      value = malloc(strlen(value_buffer) + 1);
      strcpy(value, value_buffer);
    }
    else {
      value = malloc(8);
    }
    key = malloc(strlen(key_buffer) + 1);
    request->headers[i].value = value + 1;
    strcpy(key, key_buffer);
    request->headers[i].key = key;
  }
} /* parse_request() */

/*
 * Creates error response
 */

void create_error_response(http_response * response) {
  char * reason = malloc(sizeof(char) * 128);
  const char * response_status_reason = status_reason(response->status_code);
  strcpy(reason, response_status_reason);
  response->reason_phrase = reason;
  response->headers = malloc(sizeof(header) * 2);

  response->headers[0].key = malloc(256);
  strcpy(response->headers[0].key, "Content-Type");

  response->headers[0].value = malloc(256);
  strcpy(response->headers[0].value, "text/plain");

  response->headers[1].key = malloc(256);
  strcpy(response->headers[1].key, "Content-Length");

  char * num_characters = malloc(sizeof(char) * 16);
  sprintf(num_characters, "%ld", strlen(response->message_body));
  response->headers[1].value = num_characters;
  response->num_headers = 2;
  if (response->status_code == 401) {
    response->num_headers = 3;
    response->headers = realloc(response->headers, sizeof(header) * 3);
    response->headers[2].key = malloc(256);
    strcpy(response->headers[2].key, "WWW-Authenticate");
    response->headers[2].value = malloc(256);
    strcpy(response->headers[2].value, "Basic realm=\"myhttpd-cs252\"");
  }
} /* create_error_response() */

/*
 * Returns 0 if is a supported html version.
 * Returns 1 if is an html but not supported
 * Returns -1 if is not an html.
 */

int is_html_supported(char * html_text) {

  char html_text_array[2048] = { 0 };

  strncpy(html_text_array, html_text, strlen(html_text));

  html_text_array[strlen(html_text)] = '\0';

  char * html = strtok(html_text_array, "/");
  char * version = strtok(NULL, "/");
  if ((html == NULL) || (strcmp(html, "HTTP") != 0)) {
    return -1;
  }
  if ((version == NULL) || (strcmp(version, "1.1") != 0)) {
    return 1;
  }
  return 0;

} /* is_html_supported() */

/*
 * Returns 0 if is is a cgi-bin request
 * Returns 1 if not.
 */

int is_cgi_bin_request(char * request_text) {
  char request_text_array[2048] = { 0 };

  strncpy(request_text_array, request_text, strlen(request_text));
  request_text_array[strlen(request_text)] = '\0';

  char * current_string = strtok(request_text_array, "/");

  if (strcmp(current_string, "cgi-bin") == 0) {
    return 0;
  }
  current_string = strtok(NULL, "/");
  if ((current_string != NULL) && (strcmp(current_string, "cgi-bin") == 0)) {
    return 0;
  }
  current_string = strtok(NULL, "/");
  if ((current_string != NULL) && (strcmp(current_string, "cgi-bin") == 0)) {
    return 0;
  }
  return 1;
} /* is_cgi_bin_request() */

/*
 * Create response based on the given request
 */

void create_response(http_request *request,
                     http_response *response,
                     socket_t * socket) {

  response->http_version = malloc(16);
  strcpy(response->http_version, "HTTP/1.1");

  // Check documentation for is_html_supported.
  int html_supported = is_html_supported(request->http_version);

  // Branching too see if HTTP version is malformed or not supported.
  if (html_supported == -1) {
    response->status_code = 400;
    response->message_body = malloc(256);
    strcpy(response->message_body, "HTTP version is malformed");
    create_error_response(response);
    return;
  }
  else if (html_supported == 1) {
    response->status_code = 505;
    response->message_body = malloc(256);
    strcpy(response->message_body, "HTTP version is not supported");
    create_error_response(response);
    return;
  }

  // Start of authorization block
  // ******************************************************* //
  int flag_authorized = 0;

  for (int i = 0; i < request->num_headers; i++) {
    if (strcmp(request->headers[i].key, "Authorization") == 0) {
      char arr_for_tokenizing[256] = { 0 };
      strcpy(&arr_for_tokenizing[0], request->headers[i].value);
      strtok(arr_for_tokenizing, " ");
      char * base64_value = strtok(NULL, " ");
      if (strcmp(base64_value, "ZGVuaXM6ZGVuaXM=") == 0) {
        flag_authorized = 1;
      }
      break;
    }
  }

  if (flag_authorized != 1) {
    response->status_code = 401;
    response->message_body = malloc(256);
    strcpy(response->message_body, "Not authorized. Authorize");
    create_error_response(response);
    return;
  }
  // ******************************************************* //
  // End of authorization block

  // If folder, look for index.html. Otherwise, look for file.

  struct stat st = { 0 };

  if (stat(request->request_uri, &st) == 0) {
    if (S_ISDIR(st.st_mode)) {
      char * new_path = malloc(256);
      strcpy(new_path, "");
      strcat(new_path, request->request_uri);
      strcat(new_path, "index.html");
      free(request->request_uri);
      request->request_uri = new_path;
    }
  }

  // All data is stored under appropriate folder.

  char * new_request_uri = malloc(256);
  strcpy(new_request_uri, "");
  if (is_cgi_bin_request(request->request_uri) == 0) {
    strcat(new_request_uri, "http-root-dir");
  }
  else {
    strcat(new_request_uri, "http-root-dir/htdocs");
  }
  strcat(new_request_uri, request->request_uri);
  free(request->request_uri);
  request->request_uri = new_request_uri;

  char * reason = malloc(64);

  // Only GET requests are supported.

  if (strcmp(request->method, "GET") == 0) {
    FILE * file = NULL;
    char buffer[4096] = { 0 };
    strncpy(buffer, request->request_uri, strlen(request->request_uri));
    file = fopen(strtok(buffer, "?"), "rb");
    if (file == NULL) {
      if (errno == EACCES) {
        response->status_code = 403;
        response->message_body = malloc(256);
        strcpy(response->message_body,
               "Page is forbidden for your access. Gain access.");
        create_error_response(response);
      }
      else {
        response->status_code = 404;
        response->message_body = malloc(256);
        strcpy(response->message_body,
               "Wrong URL entered. Page not found.");
        create_error_response(response);
      }
    }
    else {
      if (is_cgi_bin_request(request->request_uri) == 0) {
        fclose(file);
        handle_cgi_bin(request, socket);
        g_is_a_cgi = 1;
        return;
      }
      response->status_code = 200;
      fseek(file, 0, SEEK_END);
      long file_size = ftell(file);
      fseek(file, 0, SEEK_SET);

      char * file_content = malloc(file_size + 1);

      size_t bytes_read = fread(file_content, 1, file_size, file);

      if (bytes_read != file_size) {
        perror("Error reading file");
        fclose(file);
        return;
      }

      file_content[file_size] = '\0';

      fclose(file);

      response->message_body = file_content;

      response->num_headers = 3;
      response->headers = malloc(sizeof(header) * 3);

      response->headers[0].key = malloc(128);
      strcpy(response->headers[0].key, "Connection");

      response->headers[0].value = malloc(128);
      strcpy(response->headers[0].value, "close");

      response->headers[1].key = malloc(128);
      strcpy(response->headers[1].key, "Content-Type");

      const char * document_type_value = document_type(request->request_uri);
      char * document_type_ptr = malloc(
                            strlen(document_type_value) * sizeof(char) + 1
                          );

      strcpy(document_type_ptr, document_type_value);
      response->headers[1].value = document_type_ptr;

      response->headers[2].key = malloc(128);
      strcpy(response->headers[2].key, "Content-Length");

      char * content_length = malloc(256);
      sprintf(content_length, "%ld", file_size);
      response->headers[2].value = content_length;

      const char * response_status_reason = status_reason(
                                              response->status_code
                                            );
      strcpy(reason, response_status_reason);
      response->reason_phrase = reason;
    }
  }
  else {
    response->status_code = 405;
    response->message_body = malloc(256);
    strcpy(response->message_body, "Not supported request.");
    create_error_response(response);
  }
} /* create_response() */

/*
 * Wrapper for writing.
 */

void write_wrapper(socket_t *sock, http_response *response, char * to_string) {
  int message_length = strlen(to_string);
  socket_write(sock, to_string, message_length);
  if (response->status_code == 200) {
    socket_write(
      sock,
      response->message_body,
      atoi(response->headers[2].value)
    );
  }
  else {
    socket_write(
      sock,
      response->message_body,
      atoi(response->headers[1].value)
    );
  }
} /* write_wrapper() */

/*
 * Frees request structure
 */

void free_request(http_request * request) {
  if (request->method != NULL) {
    free(request->method);
  }
  if (request->request_uri != NULL) {
    free(request->request_uri);
  }
  if (request->http_version != NULL) {
    free(request->http_version);
  }
  if (request->num_headers != 0) {
    for (int i = 0; i < request->num_headers; i++) {
      free(request->headers[i].key);
      free(request->headers[i].value - 1);
    }
    free(request->headers);
  }
} /* free_request() */

/*
 * Frees response structure.
 */

void free_response(http_response * response) {
  if (response->http_version != NULL) {
    free(response->http_version);
  }
  if (response->num_headers != 0) {
    for (int i = 0; i < response->num_headers; i++) {
      free(response->headers[i].key);
      free(response->headers[i].value);
    }
    free(response->headers);
  }
  if (response->message_body != NULL) {
    free(response->message_body);
  }
  if (response->reason_phrase != NULL) {
    free(response->reason_phrase);
  }
} /* free_response() */

/*
 * Handle an incoming connection on the passed socket.
 */

void handle(socket_t *sock) {


  http_request request = { 0 };

  int buffer_size = 100000;

  char buffer[buffer_size];

  // char * to_free = buffer; // Points to buffer, free after parsing buffer.
  socket_read(sock, buffer, buffer_size);
  parse_request(buffer, &request);
  print_request(&request);
  http_response response = { 0 };
  create_response(&request, &response, sock);

  if (g_is_a_cgi == 1) {
    g_is_a_cgi = 0;
    return;
  }
  char *to_string = response_string(&response);

  printf("RESPONSE: %s", to_string);

  write_wrapper(sock, &response, to_string);
  fflush(stdout);
  free_response(&response);
  free_request(&request);

  free(to_string);
  to_string = NULL;

  close_socket(sock);
} /* handle() */