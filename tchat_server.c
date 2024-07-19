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

pthread_mutex_t mutex;

int client_list[MAX_CLIENT];
int next_client;

void *thread(void *ptr);
void send_buffer();
void find_next_client();
void remove_client(int clientfd);
int check_command(int clientfd, char *read_buffer);

int main(int argc, char **argv)
{
    int serverfd;
    serverfd = socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(serverfd, (struct sockaddr *)&address, sizeof(address));

    listen(serverfd, MAX_CLIENT);

    int clientfd;
    socklen_t addrlen = sizeof(address);

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < MAX_CLIENT; i++)
    {
        client_list[i] = -1;
    }
    next_client = 0;

    pthread_t client_thread;

    printf("Server: setup done\n");

    while (1)
    {
        clientfd = accept(serverfd, (struct sockaddr *)&address, &addrlen);

        printf("Server: new client %d\n", clientfd);

        if (next_client == MAX_CLIENT)
        {
            printf("Max number of clients reached\n");
            close(clientfd);
            find_next_client();
        }
        else
        {
            client_list[next_client] = clientfd;
            pthread_create(&client_thread, NULL, thread, &clientfd);
            pthread_detach(client_thread); // let the thread terminate when client disconnects
            // find_next_client();
            next_client += 1;
            printf("Server: next client index %d\n", next_client);
        }
    }

    close(serverfd);

    return 0;
}

void *thread(void *ptr)
{
    int clientfd = *(int *)ptr;

    char read_buffer[BUFFER_SIZE];

    while (1)
    {
        read(clientfd, read_buffer, BUFFER_SIZE);

        printf("Server: read message from %d\n", clientfd);

        if (read_buffer[0] == '/')
        {
            if (check_command(clientfd, read_buffer))
            {
                remove_client(clientfd);
                close(clientfd);
                return NULL;
            }
            else
            {
                continue;
            }
        }

        pthread_mutex_lock(&mutex);

        memcpy(write_buffer, read_buffer, BUFFER_SIZE);

        send_buffer();

        pthread_mutex_unlock(&mutex);
    }

    close(clientfd); // for now

    return NULL;
}

void send_buffer() // send buffer to all clients
{
    for (int i = 0; i < MAX_CLIENT; i++)
    {
        if (client_list[i] != -1)
        {
            printf("Server: sent message to %d\n", client_list[i]);
            write(client_list[i], write_buffer, BUFFER_SIZE);
        }
    }
}

void find_next_client()
{
    for (int i = 0; i < MAX_CLIENT; i++)
    {
        if (client_list[i] == -1)
        {
            next_client == i;
            return;
        }
    }

    next_client == MAX_CLIENT;
}

void remove_client(int clientfd)
{
    for (int i = 0; i < MAX_CLIENT; i++)
    {
        if (client_list[i] = clientfd)
        {
            client_list[i] = -1;
            return;
        }
    }
}

int check_command(int clientfd, char *read_buffer)
{
    if (read_buffer[1] == 'q')
    {
        return 1;
    }

    return 0;
}
