#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFF_SIZE 256

extern int readn(int fd, void *vptr, size_t n);
extern int writen(int fd, const void *vptr, size_t n);

int main(int argc, char **argv)
{
    printf("Welcome to tchat\n");

    int clientfd;
    clientfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    connect(clientfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("Connected to server...\n");

    char buff[BUFF_SIZE];
    while (1)
    {
        printf("Send Message: ");
        fgets(buff, sizeof(buff), stdin);
        send(clientfd, buff, sizeof(buff), 0);
        if (!strncmp(buff, "Quit", strlen(buff) - 1))
        {
            break;
        }
    }

    close(clientfd);

    return 0;
}