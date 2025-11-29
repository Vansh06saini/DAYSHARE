For Running Server.c file.

gcc server.c auth.c friends.c post.c feed.c -lws2_32 -o server.exe

./server


For Running client.c file in new terminal.

 gcc client.c -o client -lws2_32
 
 ./client.exe
 
