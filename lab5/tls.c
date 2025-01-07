#include "tls.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string.h>

/*
 * Close and free a TLS socket created by accept_tls_connection(). Return 0 on
 * success. You should use the polymorphic version of this function, which is
 * close_socket() in socket.c.
 */

int close_tls_socket(tls_socket *socket) {

  printf("Closing TLS socket fd %d", socket->socket_fd);

  char inet_pres[INET_ADDRSTRLEN];
  if (inet_ntop(socket->addr.sin_family,
                &(socket->addr.sin_addr),
                inet_pres,
                INET_ADDRSTRLEN)) {
    printf(" from %s", inet_pres);
  }
  putchar('\n');
  SSL_free(socket->ssl);
  int status = close(socket->socket_fd);
  free(socket);

  return status;
} /* close_tls_socket() */

/*
 * Read a buffer of length buf_len from the TLS socket. Return the length of the 
 * message on successful completion.
 * You should use the polymorphic version of this function, which is socket_read()
 * in socket.c
 */

int tls_read(tls_socket *socket, char *buf, size_t buf_len) {
  if (buf == NULL){
    return -1;
  }
  SSL_read(socket->ssl, buf, 4096);
  return 0;
} /* tls_read() */

/*
 * Write a buffer of length buf_len to the TLS socket. Return 0 on success. You
 * should use the polymorphic version of this function, which is socket_write()
 * in socket.c
 */

int tls_write(tls_socket *socket, char *buf, size_t buf_len) {
  if (buf == NULL) {
    return -1;
  }
  SSL_write(socket->ssl, buf, buf_len);
  return 0;
} /* tls_write() */

/*
 * Create a new TLS socket acceptor, listening on the given port. Return NULL on
 * error. You should ues the polymorphic version of this function, which is
 * create_socket_acceptor() in socket.c.
 */

tls_acceptor *create_tls_acceptor(int port) {

  tls_acceptor *acceptor = malloc(sizeof(tls_acceptor));

  int sock = 0;
  SSL_CTX *ctx = NULL;

  init_openssl();
  ctx = create_context();
  configure_context(ctx);

  sock = create_socket(port);

  acceptor->addr.sin_family = AF_INET;
  acceptor->addr.sin_port = htons(port);
  acceptor->addr.sin_addr.s_addr = htonl(INADDR_ANY);
  acceptor->ssl_ctx = ctx;
  acceptor->master_socket = sock;

  if (acceptor->master_socket < 0) {
    fprintf(stderr, "Unable to create socket: %s\n", strerror(errno));
    return NULL;
  }

  return acceptor;
} /* create_tls_acceptor() */

/*
 * Accept a new connection from the TLS socket acceptor. Return NULL on error,
 * and the new TLS socket otherwise. You should use the polymorphic version of
 * this function, which is accept_connection() in socket.c.
 */

tls_socket *accept_tls_connection(tls_acceptor *acceptor) {
  printf("accepted1\n");
  struct sockaddr_in addr = { 0 };
  uint len = sizeof(addr);
  SSL *ssl = NULL;
  int client = accept(acceptor->master_socket, (struct sockaddr *) &addr, &len);
  printf("accepted2\n");
  if (client < 0) {
    perror("Unable to accept");
    exit(EXIT_FAILURE);
  }
  tls_socket *socket = malloc(sizeof(tls_socket));
  ssl = SSL_new(acceptor->ssl_ctx);
  SSL_set_fd(ssl, client);
  printf("accepted3\n");
  if (SSL_accept(ssl) <= 0) {
    ERR_print_errors_fp(stderr);
    free(socket);
    printf("SOCKET IS FUCKED\n");
    return NULL;
  }
  socket->socket_fd = client;
  socket->addr = addr;
  socket->ssl = ssl;
  printf("accepted4\n");
  char inet_pres[INET_ADDRSTRLEN];
  if (inet_ntop(addr.sin_family,
                &(addr.sin_addr),
                inet_pres,
                INET_ADDRSTRLEN)) {
    printf("Received a connection from %s\n", inet_pres);
  }
  printf("accepted5\n");
  return socket;
} /* accept_tls_connection() */

/*
 * Close and free the passed TLS socket acceptor. Return 0 on success. You
 * should use the polymorphic version of this function, which is
 * close_socket_acceptor() in socket.c.
 */

int close_tls_acceptor(tls_acceptor *acceptor) {
  printf("Closing socket %d\n", acceptor->master_socket);
  int status = close(acceptor->master_socket);
  free(acceptor);
  return status;
} /* close_tls_acceptor() */


/*
 * Configure a context by specifying the certificate and private key
 */

void configure_context(SSL_CTX *ctx) {
  SSL_CTX_set_ecdh_auto(ctx, 1);

  // Load certificate into context
  if (SSL_CTX_use_certificate_file(ctx, "cert.pem",
      SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  // Add the private key into context
  if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0 ) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }
} /* configure_context() */

/*
 * Create an SSL context
 */

SSL_CTX *create_context() {
  // Method used to create context
  const SSL_METHOD *method;
  SSL_CTX *ctx;

  // Get SSL method, this is a version-flexible method and supports mutiple protocols on the server's side
  method = TLS_server_method();

  // New a context with method created above
  ctx = SSL_CTX_new(method);
  if (!ctx) {
    perror("Unable to create SSL context");
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  return ctx;
} /* create_context() */

/*
 * Create a passive socket and bind it to the specified port
 */

int create_socket(int port) {
  int socket_fd = 0;
  struct sockaddr_in addr = { 0 };

  // Set the IP address and port for this server
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Allocate a socket
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    perror("Unable to create socket");
    exit(EXIT_FAILURE);
  }

  // Bind the socket to the IP address and port
  if (bind(socket_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
    perror("Unable to bind");
    exit(EXIT_FAILURE);
  }

  // Put socket in listening mode and set the 
  // size of the queue of unprocessed connections
  if (listen(socket_fd, 1) < 0) {
    perror("Unable to listen");
    exit(EXIT_FAILURE);
  }

  return socket_fd;
}

/*
 * Call needed SSL init routines
 */

void init_openssl() { 
  SSL_load_error_strings();	
  OpenSSL_add_ssl_algorithms();
}