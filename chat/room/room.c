#include "room.h"
#include <stdlib.h>

Room *createRoom(int room_size) {
  Room *room = malloc(sizeof(Room));

  room->size = room_size;
  room->members_count = 0;
  room->pfds = malloc(sizeof(*(room->pfds)) * room_size);

  return room;
}

void increaseMembersCount(Room *room, int new_room_size) {
  room->size = new_room_size;
  room->pfds = realloc(room->pfds, sizeof(*(room->pfds)) * room->size);
}

void addMemberToRoom(Room *room, int fd) {
  if (room->members_count == room->size) {
    increaseMembersCount(room, room->size * 2);
  }
  room->pfds[room->members_count].fd = fd;
  room->pfds[room->members_count].events = POLLIN;
  room->pfds[room->members_count].revents = 0;

  room->members_count++;
}

void removeMemberFromRoom(Room *room, int memberIdx) {
  if (memberIdx >= room->members_count) {
    return;
  }
  room->pfds[memberIdx] = room->pfds[room->members_count - 1];
  room->members_count--;
}