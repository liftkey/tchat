#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main(int argc, char **argv)
{
    int clientfd;
    clientfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    connect(clientfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    close(clientfd);

    return 0;
}