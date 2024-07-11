main: tchat tchat_client

tchat: tchat.o readn.o writen.o
	gcc tchat.o readn.o writen.o -o tchat

tchat.o: tchat.c
	gcc -c tchat.c -Wall -s

tchat_client: tchat_client.o readn.o writen.o
	gcc tchat_client.o readn.o writen.o -o tchat_client

tchat_client.o: tchat_client.c
	gcc -c tchat_client.c -Wall -s

readn.o: readn.c
	gcc -c readn.c -s

writen.o: writen.c
	gcc -c writen.c -s

clean:
	rm -f tchat tchat_client *.o