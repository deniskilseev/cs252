#include "http_messages.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/*
 * Return the reason string for a particular status code. You might find this
 * helpful when implementing response_string().
 */

const char *status_reason(int status) {
  switch (status) {
    case 100:
      return "Continue";
    case 101:
      return "Switching Protocols";
    case 200:
      return "OK";
    case 201:
      return "Created";
    case 202:
      return "Accepted";
    case 203:
      return "Non-Authoritative Information";
    case 204:
      return "No Content";
    case 205:
      return "Reset Content";
    case 206:
      return "Partial Content";
    case 300:
      return "Multiple Choices";
    case 301:
      return "Moved Permanently";
    case 302:
      return "Found";
    case 303:
      return "See Other";
    case 304:
      return "Not Modified";
    case 305:
      return "Use Proxy";
    case 307:
      return  "Temporary Redirect";
    case 400:
      return "Bad Request";
    case 401:
      return "Unauthorized";
    case 402:
      return "Payment Required";
    case 403:
      return "Forbidden";
    case 404:
      return "Not Found";
    case 405:
      return "Method Not Allowed";
    case 406:
      return "Not Acceptable";
    case 407:
      return "Proxy Authentication Required";
    case 408:
      return "Request Time-out";
    case 409:
      return "Conflict";
    case 410:
      return "Gone";
    case 411:
      return "Length Required";
    case 412:
      return "Precondition Failed";
    case 413:
      return "Request Entity Too Large";
    case 414:
      return "Request-URI Too Large";
    case 415:
      return  "Unsupported Media Type";
    case 416:
      return "Requested range not satisfiable";
    case 417:
      return "Expectation Failed";
    case 500:
      return "Internal Server Error";
    case 501:
      return "Not Implemented";
    case 502:
      return "Bad Gateway";
    case 503:
      return "Service Unavailable";
    case 504:
      return "Gateway Time-out";
    case 505:
      return "HTTP Version not supported";
    default:
      return "Unknown status";
  }
} /* status_reason() */


/*
 * Return the reason string for a particular status code. You might find this
 * helpful when implementing response_string().
 */

const char *document_type(char * file_name) {
  char path_array[2048];
  strncpy(path_array, file_name, strlen(file_name));
  path_array[strlen(file_name)] = '\0';

  char * prev = NULL;
  char * cut = NULL;
  cut = strtok(path_array, ".");

  // prev will store pointer to the file extension, if any
  while (cut != NULL) {
    prev = cut;
    cut = strtok(NULL, ".");
  }

  if (strcmp(prev, "html") == 0) {
    return "text/html";
  }
  else if (strcmp(prev, "css") == 0) {
    return "text/css";
  }
  else if (strcmp(prev, "gif") == 0) {
    return "image/gif";
  }
  else if (strcmp(prev, "png") == 0) {
    return "image/png";
  }
  else if (strcmp(prev, "jpg") == 0) {
    return "image/jpeg";
  }
  else if (strcmp(prev, "svg") == 0) {
    return "image/svg+xml";
  }
  else {
    return "text/text";
  }
  return "unknown";
} /* document_type() */

/*
 * Create the actual response string to be sent over the socket, based
 * on the parameter.
 */

char *response_string(http_response *response) {
  char message[30000] = { 0 };

  char buffer[128] = { 0 };

  strcat(message, response->http_version); // HTTP/1.1
  strcat(message, " ");

  sprintf(buffer, "%d ", response->status_code); // 200
  strcat(message, buffer);

  for (int i = 0; i < 128; i++) {
    buffer[i] = 0;
  }

  strcat(message, response->reason_phrase); // OK
  strcat(message, " ");
  strcat(message, "\r\n");
  for (int i = 0; i < response->num_headers; i++) {
    strcat(message, response->headers[i].key);
    strcat(message, ": ");
    strcat(message, response->headers[i].value);
    strcat(message, "\r\n");
  }

  strcat(message, "\r\n");

  // strcat(message, response->message_body);

  char *to_string = malloc(sizeof(char) * strlen(&message[0]) + 1);
  strcpy(to_string, message);
  return to_string;
} /* response_string() */

/*
 * Print the request to stdout, useful for debugging.
 */

void print_request(http_request *request) {
  // Magic string to help with autograder

  printf("\\\\==////REQ\\\\\\\\==////\n");

  printf("Method: {%s}\n", request->method);
  printf("Request URI: {%s}\n", request->request_uri);
  printf("Query string: {%s}\n", request->query);
  printf("HTTP Version: {%s}\n", request->http_version);

  printf("Headers: \n");
  for (int i = 0; i < request->num_headers; i++) {
    printf("field-name: %s; field-value: %s\n",
           request->headers[i].key, request->headers[i].value);
  }

  printf("Message body length: %ld\n", strlen(request->message_body));
  printf("%s\n", request->message_body);

  // Magic string to help with autograder

  printf("//==\\\\\\\\REQ////==\\\\\n");
} /* print_request() */
