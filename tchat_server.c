#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENT 16
#define PORT 8080
#define BUFFER_SIZE 256

char write_buffer[BUFFER_SIZE];

pthread_mutex_t *mutex;

void *thread(void *ptr)
{
    int clientfd = *(int *)ptr;

    char *read_buffer = malloc(BUFFER_SIZE);
    read(clientfd, read_buffer, BUFFER_SIZE);

    pthread_mutex_lock(mutex);

    memcpy(write_buffer, read_buffer, BUFFER_SIZE);

    // send to all clients

    pthread_mutex_unlock(mutex);

    return NULL;
}

int main(int argc, char **argv)
{
    int serverfd;
    serverfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(serverfd, (struct sockaddr *)&address, sizeof(address));

    listen(serverfd, MAX_CLIENT);

    int clientfd;
    socklen_t addrlen = sizeof(address);

    pthread_mutex_init(mutex, NULL);

    while (1)
    {
        clientfd = accept(serverfd, (struct sockaddr *)&address, &addrlen);

        pthread_t *client_thread;
        pthread_create(client_thread, NULL, thread, &clientfd);
        pthread_detach(*client_thread); // let the thread terminate when client disconnects
    }

    close(serverfd);

    return 0;
}