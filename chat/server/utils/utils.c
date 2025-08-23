#define _XOPEN_SOURCE 700

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

int getListenerSocket(char *port, int backlog) {
  int sock_fd, status;
  struct addrinfo hints, *res, *p;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  if ((status = getaddrinfo(NULL, port, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    exit(1);
  }

  int yes = 1;
  for(p = res; p != NULL; p = p->ai_next) {
    sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sock_fd < 0) {
      continue;
    }
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (bind(sock_fd, p->ai_addr, p->ai_addrlen) < 0) {
      close(sock_fd);
      continue;;
    }
    break;
  }

  if (p == NULL) {
    return -1;
  }

  freeaddrinfo(res);

  if (listen(sock_fd, backlog) == -1) {
    return -1;
  }

  return sock_fd;
}