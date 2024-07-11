#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_CLIENT 16
#define PORT 8080

extern int readn(int fd, void *vptr, size_t n);
extern int writen(int fd, const void *vptr, size_t n);

int main(int argc, char **argv)
{
    int serverfd;
    serverfd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(serverfd, (struct sockaddr *)&address, sizeof(address));

    listen(serverfd, MAX_CLIENT);

    int clientfd;
    socklen_t addrlen = sizeof(address);
    clientfd = accept(serverfd, (struct sockaddr *)&address, &addrlen);

    char buff[256];
    while (1)
    {
        readn(clientfd, buff, sizeof(buff));
        if (!strncmp(buff, "Quit", strlen(buff) - 1))
        {
            break;
        }
    }

    close(clientfd);

    close(serverfd);

    return 0;
}