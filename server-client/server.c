#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
  const char *PORT = "3000";
  const int BACKLOG = 5;

  struct addrinfo hints, *res;
  struct sockaddr_storage their_addr;
  socklen_t addr_size;
  int sock_fd, accepted_fd;

  int status;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 1;
  }

  sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sock_fd == -1) {
    perror("socket");
    freeaddrinfo(res);
    return 1;
  }

  if (bind(sock_fd, res->ai_addr, res->ai_addrlen) == -1) {
    perror("bind");
    close(sock_fd);
    freeaddrinfo(res);
    return 1;
  }

  freeaddrinfo(res);

  if (listen(sock_fd, BACKLOG) == -1) {
    perror("listen");
    close(sock_fd);
    return 1;
  }

  addr_size = sizeof(their_addr);
  accepted_fd = accept(sock_fd, (struct sockaddr *)&their_addr, &addr_size);
  if (accepted_fd == -1) {
    perror("accept");
    close(sock_fd);
    return 1;
  }

  int len_reply;
  char *msg_reply = "Hello from server";
  len_reply = strlen(msg_reply);  
  while(1) {
    int len, bytes_recvd;
    char buf[100];
    len = sizeof(buf);

    bytes_recvd = recv(accepted_fd, &buf, len, 0);
    printf("Bytes received = %i\n", bytes_recvd);
    printf("Message from client: %s\n", buf);

    send(accepted_fd, msg_reply, len_reply, 0);
    sleep(1);
  }

  close(accepted_fd);
  close(sock_fd);

  return 0;
}