#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 256

void *read_thread(void *ptr);
void *write_thread(void *ptr);

int flag;

int main(int argc, char **argv)
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    printf("Client: 1/2\n");

    struct sockaddr_in serv_addr;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("Client: 2/2\n");

    flag = 1;

    pthread_t read, write;
    pthread_create(&read, NULL, read_thread, &sockfd);
    pthread_create(&write, NULL, write_thread, &sockfd);

    pthread_join(read, NULL);
    pthread_join(write, NULL);

    close(sockfd);

    return 0;
}

void *read_thread(void *ptr)
{
    int sockfd = *(int *)ptr;

    char read_buffer[BUFFER_SIZE];

    while (flag)
    {
        read(sockfd, read_buffer, BUFFER_SIZE);
        printf("Client: read message from server\n");
        printf("%s\n", read_buffer);
    }

    return NULL;
}

void *write_thread(void *ptr)
{
    int sockfd = *(int *)ptr;

    char write_buffer[BUFFER_SIZE];

    while (flag)
    {
        fgets(write_buffer, BUFFER_SIZE, stdin);
        write(sockfd, write_buffer, BUFFER_SIZE);
        printf("Client: write to server\n");
        if (write_buffer[0] == '/' && write_buffer[1] == 'q')
        {
            flag = 0;
        }
    }

    return NULL;
}