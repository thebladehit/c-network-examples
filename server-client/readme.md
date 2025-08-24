# Simple TCP echo server

This server receives data from client and send hardcoded "Hello from server" string back.

## Hot to run
Server:
```bash
gcc server.c -o server && ./server
```
> **NOTE:** You must run server before running client.

Client:
```bash
gcc client.c -o client && ./client
```

## References
Here is the guide from which this example was created [click](https://beej.us/guide/bgnet/html/split/system-calls-or-bust.html#system-calls-or-bust)