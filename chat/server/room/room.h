#ifndef ROOM_
#define ROOM_

#include <poll.h>

typedef struct {
  int size;
  int members_count;
  struct pollfd *pfds;
} Room;

Room *createRoom(int room_size);
void addMemberToRoom(Room *room, int fd);
void removeMemberFromRoom(Room *room, int memberIdx);
void increaseMembersCount(Room *room, int new_room_size);

#endif