main: tchat_server tchat_client

tchat_server: tchat_server.o
	gcc tchat_server.o -o tchat_server

tchat_server.o: tchat_server.c
	gcc -c tchat_server.c -pthread

tchat_client: tchat_client.o
	gcc tchat_client.o -o tchat_client

tchat_client.o: tchat_client.c
	gcc -c tchat_client.c

clean:
	rm -f tchat_server tchat_client *.o