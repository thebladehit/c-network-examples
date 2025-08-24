# TCP chat (server / client)
This server uses `poll()` to avoid polling. Server can accept connection, receive data from clients and then send to all connected clients.

## How to run
Server:
```bash
gcc server.c room/room.c utils/utils.c -Iutils -Iroom -o server && ./server
```

> **NOTE:** You must run server before running client.

Client:
```bash
gcc client.c utils/utils.c room/room.c -Iroom  -Iutils -o client && ./client
```

Also you could use `telnet` instead as a client:
```bash
telnet telnet 127.0.0.1 3000
```

## References
Here is the guide from which this example was created [click](https://beej.us/guide/bgnet/html/split/slightly-advanced-techniques.html#poll)