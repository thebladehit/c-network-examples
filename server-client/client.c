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
  const char *HOST = "127.0.0.1";

  struct addrinfo hints, *res;
  int sock_fd, status;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(HOST, PORT, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 1;
  }

  sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sock_fd == -1) {
    perror("socket");
    freeaddrinfo(res);
    return 1;
  }

  if (connect(sock_fd, res->ai_addr, res->ai_addrlen) == -1) {
    perror("connect");
    freeaddrinfo(res);
    return 1;
  }

  freeaddrinfo(res);

  char msg_buf[100];
  char msg_buf_recv[100];

  while(1) {
    printf("Your message > ");
    scanf("%s", &msg_buf);
    send(sock_fd, msg_buf, sizeof(msg_buf), 0);
    printf("message sent \n");
    recv(sock_fd, &msg_buf_recv, sizeof(msg_buf_recv), 0);
    printf("Message from server: %s\n", msg_buf_recv);
    sleep(1);
  }

  // close(sock_fd);
  
  return 0;
}