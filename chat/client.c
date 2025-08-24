#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

const char *PORT = "3000";
const char *HOST = "127.0.0.1";

void sendMessage(int connection_fd, char *name) {
  char final_msg[256];
  char user_msg[236];

  fgets(user_msg, sizeof(user_msg), stdin);
  snprintf(final_msg, sizeof(final_msg), "%s: %s", name, user_msg);

  size_t len = strlen(final_msg);
  send(connection_fd, final_msg, len, 0);
}

void getMessage(int connection_fd) {
  int nbytes;
  char msg[256];
  if ((nbytes = recv(connection_fd, msg, sizeof(msg - 1), 0)) == -1) {
    perror("recv");
    exit(1);
  }
  printf("%.*s", nbytes, msg);
}

int main() {
  int connection_fd = getConnectionSocket((char *)PORT, (char *)HOST);
  if (connection_fd == -1) {
    fprintf(stderr, "error setuping connection socket\n");
    exit(1);
  }

  char name[20];

  printf("Name (max 19 char)> ");
  scanf("%19s", &name);
  getchar();

  if (!fork()) {
    while(1) {
      sendMessage(connection_fd, name);
    }
  } else {
    while(1) {
      getMessage(connection_fd);
    }
  }

}