#include "room.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>

const char *PORT = "3000";
const int BACKLOG = 10;

void notifyAllMembers(Room* room, char *msg, int nbytes, int sender_fd) {
  for (int i = 1; i < room->members_count; i++) {
    int dest_fd = room->pfds[i].fd;
    if (dest_fd == sender_fd) continue;
    if (send(dest_fd, msg, nbytes, 0) == -1) {
      perror("send");
    }
  }
}

void handleNewConnection(int listener_fd, Room *room) {
  int new_fd;
  struct sockaddr_storage new_addr;
  socklen_t addr_size = sizeof(new_addr);

  new_fd = accept(listener_fd, (struct sockaddr *)&new_addr, &addr_size);
  if (new_fd == -1) {
    perror("accept");
  } else {
    addMemberToRoom(room, new_fd);
    printf("New connection was accepted\n");
  }
}

void handleClientData(int listener_fd, Room *room, int *memberIdx) {
  char buf[256];
  int sender_fd = room->pfds[*memberIdx].fd;
  int nbytes = recv(sender_fd, buf, sizeof(buf), 0);

  if (nbytes <= 0) {
    printf("client socket %d is closed\n", sender_fd);
    close(sender_fd);
    removeMemberFromRoom(room, *memberIdx);
    (*memberIdx)--;
  } else {
    printf("receive from fd %d: %.*s", sender_fd, nbytes, buf);
    notifyAllMembers(room, buf, nbytes, sender_fd);
  }
}

void processConnections(int listener_fd, Room *room) {
  for (int i = 0; i < room->members_count; i++) {
    if (room->pfds[i].revents & (POLLIN | POLLOUT)) {
      if (room->pfds[i].fd == listener_fd) {
        handleNewConnection(listener_fd, room);
      } else {
        handleClientData(listener_fd, room, &i);
      }
    }
  }
}

int main() {
  Room *room = createRoom(5);

  int listener_fd = getListenerSocket((char *)PORT, BACKLOG);
  if (listener_fd == -1) {
    fprintf(stderr, "error setuping listening socket\n");
    exit(1);
  }

  addMemberToRoom(room, listener_fd);

  printf("Server started on %s port: waiting for connections...\n", PORT);

  while(1) {
    int poll_count = poll(room->pfds, room->members_count, -1);
    if (poll_count == -1) {
      perror("poll");
      exit(1);
    }
    processConnections(listener_fd, room);
  }

  return 0;
}